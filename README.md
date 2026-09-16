# IoT Water Level Monitor & Automatic Pump Controller

An ESP32-based water-tank monitor with an ultrasonic sensor, an active-low relay, and a responsive browser dashboard. The ESP32 creates its own Wi-Fi access point, so no router or cloud service is required.

## Improvements in the current version

- Displays **water level percentage** as well as sensor distance.
- Uses non-blocking `millis()` scheduling for regular sensor reads and web requests.
- Rejects missing or out-of-range ultrasonic readings instead of starting the pump on bad data.
- Uses separate start/stop thresholds (20% and 90%) to prevent rapid relay switching.
- Forces the pump to a safe OFF state at startup and when switching back to automatic mode.
- Provides a JSON status endpoint at `/api/status` and a mobile-friendly dashboard.
- Keeps manual pump controls disabled while automatic mode is active.

## Hardware

- ESP32 development board
- HC-SR04 ultrasonic sensor (use a voltage divider on ECHO; ESP32 GPIO is 3.3 V)
- Active-low 5 V relay module
- Pump and an appropriate, isolated power supply

> **Safety:** Never power a pump directly from an ESP32 GPIO. Use a correctly rated relay or motor driver, a common ground where required by the relay module, fusing, and proper electrical isolation. Mains-voltage wiring must be performed by a qualified person.

## Pin configuration

| Component | ESP32 pin | Notes |
|---|---:|---|
| Ultrasonic TRIG | GPIO 5 | Output |
| Ultrasonic ECHO | GPIO 18 | Input; step down 5 V ECHO to 3.3 V |
| Relay IN | GPIO 23 | Active-low by default |

Connect sensor GND and relay GND to ESP32 GND. Connect the pump through the relay's correctly rated COM/NO terminals and use a separate pump supply.

## Calibration and control

Edit the constants near the top of `water_level_monitor.ino`:

```cpp
constexpr float TANK_DEPTH_CM = 100.0f;
constexpr float FULL_LEVEL_DISTANCE_CM = 10.0f;
constexpr float PUMP_START_LEVEL = 20.0f;
constexpr float PUMP_STOP_LEVEL = 90.0f;
```

- `TANK_DEPTH_CM`: distance from the sensor to the tank bottom when empty.
- `FULL_LEVEL_DISTANCE_CM`: distance from the sensor to the water at the desired full level.
- The pump starts at or below 20% and stops at or above 90%.
- Change `RELAY_ACTIVE_LOW` to `false` for an active-high relay module.

The level formula maps the empty distance to 0% and the configured full distance to 100%.

## Upload and use

1. Install Arduino IDE and the ESP32 board package by Espressif.
2. Open `water_level_monitor.ino`.
3. Select **ESP32 Dev Module**, the correct port, and upload.
4. Open Serial Monitor at **115200 baud**.
5. Connect a phone or computer to:
   - SSID: `Water_Controller`
   - Password: `12345678`
6. Open `http://192.168.4.1` in a browser.

Change the default access-point password before using the device in a real installation.

## Web API

| Endpoint | Method | Description |
|---|---|---|
| `/` | GET | Dashboard |
| `/api/status` | GET | JSON sensor, level, mode, and pump state |
| `/api/mode` | GET | Toggle automatic/manual mode |
| `/api/pump?state=on` | GET | Turn on in manual mode |
| `/api/pump?state=off` | GET | Turn off in manual mode |

Example status response:

```json
{"valid":true,"distance":46.2,"level":59.8,"pump":false,"auto":true}
```

## Troubleshooting

- **Dashboard unavailable:** reconnect to the ESP32 access point and browse to `192.168.4.1`.
- **Sensor reading unavailable:** check TRIG/ECHO wiring, common ground, sensor alignment, and the ECHO voltage divider.
- **Pump logic is reversed:** verify the relay type and change `RELAY_ACTIVE_LOW`.
- **Level is inaccurate:** measure the empty and full distances and update the calibration constants.
- **Relay chatters:** verify the start/stop thresholds are sufficiently separated and that the sensor is not aimed at turbulence.

## License

Open-source project. Modify and use it for personal or commercial projects at your own risk.
