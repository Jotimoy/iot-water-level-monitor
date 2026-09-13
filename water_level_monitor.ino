#include <WiFi.h>
#include <WebServer.h>

// --- Wi-Fi Credentials (Access Point) ---
const char* ssid = "WaterSystem_ESP32";
const char* password = "password123"; // Minimum 8 characters

// --- Pin Definitions ---
const int TRIG_PIN = 5;
const int ECHO_PIN = 18;
const int PUMP_PIN = 19; 

// --- System Variables ---
long duration;
int distance;
bool pumpState = false;
bool autoMode = true; 

// --- Water Level Thresholds (in cm) ---
const int TANK_FULL_CM = 10;  // Turn OFF pump when water is 10cm from sensor
const int TANK_EMPTY_CM = 40; // Turn ON pump when water is 40cm from sensor

WebServer server(80);

// --- HTML & CSS for the Web App ---
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Water Level Monitor</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; background-color: #f4f4f4; color: #333; }
    h1 { color: #007BFF; }
    .card { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); display: inline-block; margin-bottom: 20px; }
    .data { font-size: 2rem; font-weight: bold; margin: 10px 0; }
    .btn { padding: 15px 30px; font-size: 1.2rem; border: none; border-radius: 5px; cursor: pointer; margin: 10px; color: white; width: 80%; max-width: 300px;}
    .btn-on { background-color: #28a745; }
    .btn-off { background-color: #dc3545; }
    .btn-auto { background-color: #ffc107; color: black; }
  </style>
</head>
<body>
  <h1>Water Pump Control</h1>
  
  <div class="card">
    <h3>Water Level (Distance)</h3>
    <div class="data" id="distanceVal">-- cm</div>
  </div>
  <br>
  <div class="card">
    <h3>Pump Status: <span id="pumpStatusVal">--</span></h3>
    <h3>Mode: <span id="modeVal">--</span></h3>
    <button class="btn btn-auto" onclick="toggleMode()">Toggle Auto/Manual Mode</button><br>
    <button class="btn btn-on" onclick="controlPump('on')">Turn Pump ON</button><br>
    <button class="btn btn-off" onclick="controlPump('off')">Turn Pump OFF</button>
  </div>

  <script>
    // Fetch data every 2 seconds without reloading the page
    setInterval(function() {
      fetch('/data').then(response => response.json()).then(data => {
        document.getElementById("distanceVal").innerText = data.distance + " cm";
        document.getElementById("pumpStatusVal").innerText = data.pump ? "ON" : "OFF";
        document.getElementById("modeVal").innerText = data.auto ? "AUTO" : "MANUAL";
      });
    }, 2000);

    function controlPump(action) {
      fetch('/pump?state=' + action);
    }
    
    function toggleMode() {
      fetch('/mode');
    }
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW); // Ensure pump is off on startup

  // Start ESP32 in Access Point Mode
  Serial.println("Starting Access Point...");
  WiFi.softAP(ssid, password);
  Serial.print("Connect to Wi-Fi: ");
  Serial.println(ssid);
  Serial.print("IP Address to visit in browser: ");
  Serial.println(WiFi.softAPIP());

  // Define Web Server Routing
  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });

  server.on("/data", []() {
    String json = "{\"distance\":" + String(distance) + 
                  ",\"pump\":" + String(pumpState) + 
                  ",\"auto\":" + String(autoMode) + "}";
    server.send(200, "application/json", json);
  });

  server.on("/pump", []() {
    if (!autoMode) { // Only allow manual control if Auto Mode is OFF
      if (server.arg("state") == "on") pumpState = true;
      else if (server.arg("state") == "off") pumpState = false;
      digitalWrite(PUMP_PIN, pumpState ? HIGH : LOW);
    }
    server.send(200, "text/plain", "OK");
  });

  server.on("/mode", []() {
    autoMode = !autoMode;
    server.send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
  server.handleClient(); // Listen for HTTP requests from phone

  // 1. Read Ultrasonic Sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  if (duration == 0) distance = 999; // Error reading
  else distance = duration * 0.034 / 2;

  // 2. Handle Auto Mode Logic
  if (autoMode) {
    if (distance >= TANK_EMPTY_CM && !pumpState) {
      pumpState = true;
      digitalWrite(PUMP_PIN, HIGH);
    } 
    else if (distance <= TANK_FULL_CM && pumpState) {
      pumpState = false;
      digitalWrite(PUMP_PIN, LOW);
    }
  }
  
  delay(100); // Short delay for stability
}
