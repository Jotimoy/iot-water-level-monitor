# Installation Guide

## Prerequisites

- Windows, macOS, or Linux computer
- ESP32 development board
- USB cable (USB-A to Micro-USB)
- WiFi network or access to hotspot

## Step 1: Install Arduino IDE

### Windows & macOS
1. Download from [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
2. Run the installer and follow the setup wizard
3. Choose installation location and complete installation
4. Launch Arduino IDE

### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install arduino
```

### Verify Installation
- Open Arduino IDE
- You should see the main editor interface with Tools, Sketch, Help menus

## Step 2: Add ESP32 Board Support

### Method 1: Using Board Manager (Recommended)

1. **Open Arduino IDE**
2. Go to **File → Preferences**
3. Find the field "Additional Board Manager URLs"
4. Paste this URL:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
5. Click OK
6. Go to **Tools → Board → Board Manager**
7. Search for "esp32" in the search box
8. Select "esp32 by Espressif Systems" (latest version)
9. Click **Install** and wait for completion (2-5 minutes)
10. Close Board Manager

### Verify ESP32 Board Installation
1. Go to **Tools → Board**
2. You should see many ESP32 options listed
3. Select **"ESP32 Dev Module"**

## Step 3: Clone or Download Repository

### Option A: Using Git
```bash
git clone https://github.com/Jotimoy/iot-water-level-monitor.git
cd iot-water-level-monitor
```

### Option B: Download ZIP
1. Click the green **"Code"** button on GitHub
2. Select **"Download ZIP"**
3. Extract the ZIP file to your desired location
4. Open the extracted folder

## Step 4: Open Project in Arduino IDE

1. Launch Arduino IDE
2. Go to **File → Open**
3. Navigate to `water_level_monitor.ino` in the cloned/extracted folder
4. Click **Open**
5. The sketch should now load in the editor

## Step 5: Configure ESP32 Settings

### Connect ESP32 via USB
1. Connect USB cable to ESP32 and your computer
2. You should see the device in Device Manager (Windows) or `/dev/ttyUSB*` (Linux)

### Configure Arduino IDE
1. Go to **Tools → Board** and select **"ESP32 Dev Module"**
2. Go to **Tools → Port** and select your ESP32's COM port:
   - **Windows**: COM3, COM4, etc.
   - **macOS**: /dev/cu.usbserial-*
   - **Linux**: /dev/ttyUSB0 or similar
3. Go to **Tools → Upload Speed** and select **"115200"**
4. Leave other settings at default

### Verify Board Connection
```
Expected Board Settings:
Board:           ESP32 Dev Module
Port:            COM[X] (or /dev/tty*)
Upload Speed:    115200
Flash Frequency: 80 MHz
Flash Mode:      QIO
Flash Size:      4MB
Partition Scheme: Default 4MB with SPIFFS
```

## Step 6: Upload Code to ESP32

1. **Verify Sketch** (Ctrl+Alt+V / Cmd+Shift+V)
   - Should complete with "Compilation complete" message
2. **Upload Sketch** (Ctrl+U / Cmd+U)
   - IDE will compile and upload
   - You should see "Upload Complete" message

### Troubleshooting Upload Issues

**"Port Not Found" Error**
- Ensure USB cable is connected
- Try different USB port on your computer
- Install USB drivers: [CH340 drivers](https://github.com/WCHSoftware/ch341ser) (if using CH340-based ESP32)

**"Compilation Failed" Error**
- Verify ESP32 board is selected
- Check that all libraries are installed
- Try: **Tools → Board → Board Manager** and reinstall ESP32 core

**"Upload Timeout" Error**
- Reduce upload speed: Tools → Upload Speed → 115200 or lower
- Hold BOOT button on ESP32 during upload
- Check USB cable quality

## Step 7: Verify Installation via Serial Monitor

1. **Open Serial Monitor**: Tools → Serial Monitor (or Ctrl+Shift+M)
2. **Set Baud Rate**: 115200 (bottom-right dropdown)
3. You should see output like:
   ```
   Starting Access Point...
   Connect to Wi-Fi: WaterSystem_ESP32
   IP Address to visit in browser: 192.168.4.1
   ```
4. If you see this, **upload was successful!** ✅

## Step 8: Connect to WiFi Network

### On Your Phone or Computer

1. **Open WiFi Settings**
2. **Look for network**: "WaterSystem_ESP32"
3. **Enter password**: "password123"
4. **Wait for connection** (5-10 seconds)

### Verify Connection
- WiFi signal should show as connected
- Open browser and navigate to: **192.168.4.1**
- You should see the **Water Pump Control Dashboard**

## Step 9: Test the Dashboard

1. **Distance Display**: Should show current distance value in cm
2. **Pump Status**: Should update every 2 seconds
3. **Mode Toggle**: Click to switch AUTO/MANUAL
4. **Pump Buttons**: Test ON/OFF in MANUAL mode
5. **Data Refresh**: All values should update smoothly

## Configuration (Optional)

### Change WiFi Credentials

Edit these lines in `water_level_monitor.ino`:

```cpp
const char* ssid = "WaterSystem_ESP32";       // Change SSID
const char* password = "password123";         // Change password
```

Recompile and upload.

### Change Water Level Thresholds

```cpp
const int TANK_FULL_CM = 10;   // Adjust for your tank
const int TANK_EMPTY_CM = 40;  // Adjust for your tank
```

### Adjust Polling Interval

In the JavaScript section of HTML:
```javascript
setInterval(function() {
  // Currently: 2000 ms = 2 seconds
  // Change to: 1000 ms = 1 second, 5000 ms = 5 seconds, etc.
}, 2000); // <-- Adjust this value
```

## Uninstallation / Cleanup

### Remove ESP32 Board
1. Tools → Board Manager
2. Search "esp32"
3. Select "esp32 by Espressif Systems"
4. Click **Uninstall**

### Delete Project Files
1. Simply delete the cloned/extracted folder
2. Or use: `rm -rf iot-water-level-monitor/` (Linux/macOS)

## Common Installation Problems

| Problem | Solution |
|---------|----------|
| "ESP32 board not found" in Tools → Board | Reinstall ESP32 core via Board Manager |
| USB driver not recognized | Install CH340 drivers or update USB drivers |
| Can't access 192.168.4.1 | Ensure you're connected to "WaterSystem_ESP32" WiFi |
| WiFi network not visible | ESP32 may not have booted; wait 10 seconds and rescan |
| Upload keeps failing | Try lower baud rate (9600) or different USB port |
| Serial Monitor shows garbage | Verify baud rate is 115200 |

## Next Steps

1. ✅ Installation complete!
2. 📖 Read [README.md](README.md) for usage guide
3. 🔧 Follow [HARDWARE_SETUP.md](HARDWARE_SETUP.md) for wiring
4. 🎮 Test the web interface at 192.168.4.1
5. 🚀 Customize settings for your specific tank dimensions

## Support

If you encounter issues:
1. Check the [Troubleshooting](README.md#-troubleshooting) section
2. Review [HARDWARE_SETUP.md](HARDWARE_SETUP.md) for wiring issues
3. Open an issue on GitHub with error details
4. Check Arduino IDE Serial Monitor output for error messages

---

**Installation Guide Complete!** Ready to build your water monitoring system 💧
