#include <WiFi.h>
#include <WebServer.h>

// ---------------- Wi-Fi Access Point ----------------
const char *ssid = "Water_Controller";
const char *password = "12345678"; // Change before deployment (minimum 8 characters)

// ---------------- Hardware ----------------
constexpr uint8_t TRIG_PIN = 5;
constexpr uint8_t ECHO_PIN = 18;
constexpr uint8_t RELAY_PIN = 23;
constexpr bool RELAY_ACTIVE_LOW = true;

// ---------------- Tank calibration ----------------
constexpr float TANK_DEPTH_CM = 100.0f; // Sensor to tank bottom when empty
constexpr float FULL_LEVEL_DISTANCE_CM = 10.0f; // Sensor to water at 100%

// The pump starts at/below this level and stops at/above this level.
constexpr float PUMP_START_LEVEL = 20.0f;
constexpr float PUMP_STOP_LEVEL = 90.0f;

constexpr unsigned long SENSOR_INTERVAL_MS = 1500;
constexpr unsigned long ECHO_TIMEOUT_US = 30000;

WebServer server(80);

bool autoMode = true;
bool pumpState = false;
bool sensorValid = false;
float distanceCM = 0.0f;
float waterLevelPercent = 0.0f;
unsigned long lastSensorRead = 0;

void setPump(bool on) {
  pumpState = on;
  pinMode(RELAY_PIN, OUTPUT);
  const bool relayLevel = RELAY_ACTIVE_LOW ? !on : on;
  digitalWrite(RELAY_PIN, relayLevel ? HIGH : LOW);
}

bool readUltrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  if (duration == 0) {
    sensorValid = false;
    return false;
  }

  const float measuredDistance = (duration * 0.0343f) / 2.0f;
  if (measuredDistance < 2.0f || measuredDistance > TANK_DEPTH_CM + 20.0f) {
    sensorValid = false;
    return false;
  }

  distanceCM = measuredDistance;
  const float usableHeight = TANK_DEPTH_CM - FULL_LEVEL_DISTANCE_CM;
  const float waterHeight = TANK_DEPTH_CM - distanceCM;
  waterLevelPercent = (waterHeight / usableHeight) * 100.0f;
  waterLevelPercent = constrain(waterLevelPercent, 0.0f, 100.0f);
  sensorValid = true;
  return true;
}

String jsonData() {
  String json = "{\"valid\":" + String(sensorValid ? "true" : "false");
  json += ",\"distance\":" + String(distanceCM, 1);
  json += ",\"level\":" + String(waterLevelPercent, 1);
  json += ",\"pump\":" + String(pumpState ? "true" : "false");
  json += ",\"auto\":" + String(autoMode ? "true" : "false") + "}";
  return json;
}

const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!doctype html><html><head><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Water Monitor</title><style>
body{font-family:Arial;text-align:center;background:#f4f7f6;margin:20px;color:#253238}.card{max-width:390px;margin:auto;background:#fff;padding:22px;border-radius:14px;box-shadow:0 4px 12px #0002}.level{font-size:44px;font-weight:bold;color:#078ca3;margin:12px}.status{font-weight:bold}.btn{border:0;border-radius:7px;color:#fff;padding:12px 20px;margin:6px;font-size:16px;cursor:pointer}.blue{background:#087dcc}.green{background:#209447}.red{background:#d93645}.muted{color:#777}
</style></head><body><div class="card"><h2>Water Monitor</h2><div id="level" class="level">-- %</div><p>Distance: <span id="distance">--</span> cm</p><p>Mode: <span id="mode" class="status">--</span></p><p>Pump: <span id="pump" class="status">--</span></p><p id="sensor" class="muted">Waiting for sensor...</p><hr><button class="btn blue" onclick="toggleMode()">Switch Mode</button><div id="controls"></div></div>
<script>
async function update(){try{const r=await fetch('/api/status');const d=await r.json();document.querySelector('#level').textContent=d.valid?Math.round(d.level)+' %':'-- %';document.querySelector('#distance').textContent=d.valid?d.distance.toFixed(1):'--';document.querySelector('#mode').textContent=d.auto?'AUTOMATIC':'MANUAL';document.querySelector('#pump').textContent=d.pump?'RUNNING':'STOPPED';document.querySelector('#sensor').textContent=d.valid?'Sensor OK':'Sensor reading unavailable';document.querySelector('#controls').innerHTML=d.auto?'<p class="muted">Manual controls are disabled in automatic mode.</p>':`<button class="btn green" onclick="pump(1)">Turn Pump ON</button><button class="btn red" onclick="pump(0)">Turn Pump OFF</button>`}catch(e){document.querySelector('#sensor').textContent='Connection error'}}
function pump(on){fetch('/api/pump?state='+(on?'on':'off')).then(update)} function toggleMode(){fetch('/api/mode').then(update)} update();setInterval(update,2000);
</script></body></html>)rawliteral";

void handleRoot() { server.send(200, "text/html", HTML_PAGE); }
void handleStatus() { server.send(200, "application/json", jsonData()); }
void handleMode() { autoMode = !autoMode; if (autoMode) setPump(false); server.send(200, "text/plain", "OK"); }
void handlePump() {
  if (!autoMode && server.hasArg("state")) {
    const String state = server.arg("state");
    if (state == "on") setPump(true);
    else if (state == "off") setPump(false);
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  setPump(false); // Safe startup state

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("Connect to Wi-Fi: "); Serial.println(ssid);
  Serial.print("Dashboard: http://"); Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/api/status", handleStatus);
  server.on("/api/mode", handleMode);
  server.on("/api/pump", handlePump);
  server.begin();
}

void loop() {
  server.handleClient();
  const unsigned long now = millis();
  if (now - lastSensorRead >= SENSOR_INTERVAL_MS) {
    lastSensorRead = now;
    const bool readingOK = readUltrasonic();
    if (autoMode && readingOK) {
      if (waterLevelPercent <= PUMP_START_LEVEL && !pumpState) {
        setPump(true);
        Serial.println("Auto: water low, pump started");
      } else if (waterLevelPercent >= PUMP_STOP_LEVEL && pumpState) {
        setPump(false);
        Serial.println("Auto: tank full, pump stopped");
      }
    }
    if (!readingOK) Serial.println("Warning: ultrasonic reading unavailable");
  }
}
