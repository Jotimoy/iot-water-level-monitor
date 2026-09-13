# IoT Water Level Monitor & Control System

An intelligent, IoT-based water tank monitoring and automatic pump control system using ESP32, ultrasonic sensors, and a responsive web interface.

## 🌟 Features

- **Real-time Water Level Monitoring**: Ultrasonic sensor measures distance to water surface
- **Automatic Pump Control**: Auto-fill mode with configurable thresholds
- **Manual Override**: Switch between automatic and manual control modes
- **Web-based Dashboard**: Access from any browser on the same WiFi network
- **WiFi Access Point**: ESP32 creates its own WiFi network for easy connectivity
- **Live Updates**: Real-time data refresh every 2 seconds
- **Simple & Reliable**: Minimal dependencies, easy to deploy

## 📋 System Requirements

### Hardware
- **ESP32 Development Board** (e.g., ESP32-WROOM-32)
- **HC-SR04 Ultrasonic Sensor** (water level detection)
- **12V Pump with Relay Module** (or any 5V-controlled pump)
- **Power Supply** (5V for ESP32, 12V for pump)
- **Connecting Wires & Breadboard**
- **USB Cable** (for programming)

### Software
- Arduino IDE 1.8.0+
- ESP32 Board Package (installed via Arduino Board Manager)
- Built-in WiFi and WebServer libraries (included with ESP32 core)

## 🔌 Pin Configuration

| Component | GPIO Pin | Description |
|-----------|----------|-------------|
| Ultrasonic TRIG | GPIO 5 | Trigger pulse for distance measurement |
| Ultrasonic ECHO | GPIO 18 | Echo pulse from sensor |
| Pump Relay | GPIO 19 | Control signal for pump (HIGH = ON, LOW = OFF) |

## 📐 Water Level Thresholds

- **TANK_FULL_CM**: 10 cm - Pump turns OFF when water is 10cm from sensor
- **TANK_EMPTY_CM**: 40 cm - Pump turns ON when water is 40cm from sensor

*Adjust these values based on your tank dimensions and sensor placement*

## 🚀 Getting Started

### 1. Install Arduino IDE & ESP32 Board

1. Download [Arduino IDE](https://www.arduino.cc/en/software)
2. Open Arduino IDE → File → Preferences
3. Add this URL to "Additional Board Manager URLs":
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. Go to Tools → Board Manager → Search "esp32" → Install by Espressif Systems

### 2. Hardware Setup

**Ultrasonic Sensor Wiring:**
```
HC-SR04 VCC  → ESP32 5V
HC-SR04 GND  → ESP32 GND
HC-SR04 TRIG → ESP32 GPIO 5
HC-SR04 ECHO → ESP32 GPIO 18 (via voltage divider if needed)
```

**Pump Control Wiring:**
```
Relay Module IN  → ESP32 GPIO 19
Relay Module VCC → ESP32 5V
Relay Module GND → ESP32 GND
Relay Pump OUT  → 12V Pump
```

### 3. Upload Code

1. Open `water_level_monitor.ino` in Arduino IDE
2. Select **Tools → Board → ESP32 Dev Module**
3. Select correct **COM Port**
4. Click Upload
5. Open Serial Monitor (Baud: 115200) to see connection details

### 4. Connect & Access

1. Once uploaded, the ESP32 creates a WiFi network:
   - **SSID**: `WaterSystem_ESP32`
   - **Password**: `password123`
2. Connect your phone/computer to this network
3. Open browser and navigate to: `192.168.4.1`
4. You should see the Water Pump Control dashboard

## 🎮 Web Interface Guide

### Dashboard Components

**Water Level Display**: Shows current distance in centimeters from sensor to water surface

**Pump Status**: Displays if pump is ON or OFF

**Mode Display**: Shows current operating mode (AUTO or MANUAL)

### Controls

- **Toggle Auto/Manual Mode**: Switch between automatic and manual operation
  - **AUTO**: Pump follows the threshold logic automatically
  - **MANUAL**: You control pump with ON/OFF buttons

- **Turn Pump ON**: Manually activate pump (only works in MANUAL mode)
- **Turn Pump OFF**: Manually deactivate pump (only works in MANUAL mode)

## ⚙️ How It Works

### Auto Mode Logic

```
If (distance >= 40cm) AND (pump is OFF)
  → Turn pump ON (tank is low)

If (distance <= 10cm) AND (pump is ON)
  → Turn pump OFF (tank is full)
```

### Sensor Reading

1. ESP32 sends a 10µs pulse to TRIG pin
2. Ultrasonic sensor emits sound wave
3. ECHO pin receives reflected signal
4. Time difference calculates distance: `distance = duration × 0.034 / 2`
5. Reading updates every 100ms

### Web Server Communication

- **GET /**: Serves the HTML dashboard
- **GET /data**: Returns JSON with current sensor & pump status
- **GET /pump?state=on/off**: Controls pump (manual mode only)
- **GET /mode**: Toggles between AUTO and MANUAL modes

## 🔧 Configuration

Edit these constants in the code to customize:

```cpp
// WiFi Credentials
const char* ssid = "WaterSystem_ESP32";
const char* password = "password123";

// Water Level Thresholds (in cm)
const int TANK_FULL_CM = 10;   // Adjust based on tank height
const int TANK_EMPTY_CM = 40;  // Adjust based on tank height
```

## 🐛 Troubleshooting

| Issue | Solution |
|-------|----------|
| Can't connect to WiFi | Check SSID/password, restart ESP32 |
| No distance reading (999 cm) | Check ultrasonic sensor wiring, test TRIG/ECHO pins |
| Pump doesn't respond | Verify GPIO 19 is connected, check relay module |
| Web page won't load | Ensure you're on the same WiFi network, try 192.168.4.1 |
| Distance readings fluctuate | Normal behavior; use averaging in code if needed |

## 📊 Serial Monitor Output

Example output at startup:
```
Starting Access Point...
Connect to Wi-Fi: WaterSystem_ESP32
IP Address to visit in browser: 192.168.4.1
```

## 🔐 Security Notes

- Change default WiFi password in code
- The web interface has no authentication (add if needed for production)
- WiFi password should be minimum 8 characters
- Consider using WiFi Manager library for easier configuration in production

## 🚀 Future Enhancements

- [ ] Data logging to SD card or cloud database
- [ ] Mobile app integration
- [ ] Email/SMS alerts for tank status
- [ ] Multiple sensor support for large tanks
- [ ] MQTT integration for smart home systems
- [ ] Historical data graphs and analytics
- [ ] Low-power sleep modes for battery operation

## 📝 License

This project is open-source. Feel free to modify and use for personal or commercial projects.

## 🤝 Contributing

Contributions are welcome! Please submit issues or pull requests for improvements.

## 📧 Support

For questions or issues, create an issue on the GitHub repository.

---

**Happy Water Monitoring! 💧**
