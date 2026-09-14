# Hardware Setup Guide

## 📦 Components Checklist

### Required Components
- [ ] ESP32 Development Board (ESP32-WROOM-32)
- [ ] HC-SR04 Ultrasonic Sensor
- [ ] 5V Relay Module (for pump control)
- [ ] 12V DC Pump (or any 5V-controlled pump)
- [ ] 5V Power Supply (for ESP32)
- [ ] 12V Power Supply (for pump, if required)
- [ ] USB Cable (for programming ESP32)
- [ ] Breadboard & Jumper Wires
- [ ] (Optional) Voltage Divider Resistors (1kΩ and 2kΩ for ECHO pin protection)

## 🔧 Wiring Diagram

### ASCII Reference Diagram
```
┌─────────────────────────────────────────────────────────────┐
│                         ESP32                                │
│                                                              │
│  5V ───────────┬─────────────────────────┬───────────────┐  │
│                │                         │               │  │
│              HC-SR04                  Relay Module        │  │
│              VCC                      VCC                │  │
│                                                           │  │
│  GND ──────────┬─────────────────────────┬───────────────┤  │
│                │                         │               │  │
│              HC-SR04                  Relay Module        │  │
│              GND                      GND                │  │
│                                                           │  │
│  GPIO 5 ─────→ HC-SR04 TRIG                              │  │
│                                                           │  │
│  GPIO 18 ←──── HC-SR04 ECHO                              │  │
│                (via voltage divider)                      │  │
│                                                           │  │
│  GPIO 19 ─────→ Relay Module IN                          │  │
│                                                           │  │
└─────────────────────────────────────────────────────────────┘
```

### Visual Wiring Diagram

![ESP32 Water Level Monitor Wiring Diagram]([https://github.com/Jotimoy/iot-water-level-monitor/blob/main/wiring_diagram.png?raw=true](https://github.com/Jotimoy/iot-water-level-monitor/blob/d220367622f40ab969530ff6122d74695fe53ebb/ESP32%20Water%20Level%20Monitor%20Wiring%20Diagram.png))

*The diagram shows the complete connection between ESP32, HC-SR04 ultrasonic sensor, relay module, and pump power supply.*

## 🔌 Step-by-Step Wiring Instructions

### 1. Power Supply Setup

**ESP32 Power Connection:**
```
Power Supply 5V+ → ESP32 5V Pin
Power Supply GND → ESP32 GND Pin
```

### 2. Ultrasonic Sensor (HC-SR04) Connections

**Pin Mapping:**
| HC-SR04 Pin | ESP32 Pin | Connection |
|-------------|-----------|------------|
| VCC | 5V | Positive Power |
| GND | GND | Ground |
| TRIG | GPIO 5 | Trigger Signal |
| ECHO | GPIO 18 | Echo Signal (via Voltage Divider) |

**Important: ECHO Pin Protection**

The HC-SR04 ECHO pin outputs 5V signal, but ESP32 GPIO accepts max 3.3V. Use a voltage divider:

```
HC-SR04 ECHO ──────┬─────── 1kΩ Resistor ─────┬──── ESP32 GPIO 18
                   │                          │
                   └────── 2kΩ Resistor ──────┴──── GND
```

**Voltage Divider Calculation:**
- Output Voltage = 5V × (2kΩ / (1kΩ + 2kΩ)) = 3.33V (safe for ESP32)

### 3. Relay Module Connections

**Pin Mapping:**
| Relay Pin | ESP32 Pin | Connection |
|-----------|-----------|------------|
| VCC | 5V | Positive Power |
| GND | GND | Ground |
| IN | GPIO 19 | Control Signal |
| COM | Common (Pump Supply) | |
| NO | Normally Open (Pump) | |
| NC | Normally Closed | (Not Used) |

**Pump Connection:**
```
12V Power Supply+ → Relay Module COM
Relay Module NO → 12V Pump +
12V Pump - → Power Supply GND
```

## 📐 Complete Wiring Table

| ESP32 Pin | Signal | Connected To | Wire Color (Suggested) |
|-----------|--------|--------------|------------------------|
| 5V | VCC | HC-SR04 VCC, Relay VCC | Red |
| GND | GND | HC-SR04 GND, Relay GND, Voltage Divider | Black |
| GPIO 5 | TRIG | HC-SR04 TRIG | Yellow |
| GPIO 18 | ECHO | HC-SR04 ECHO (via 1kΩ) | Orange |
| GPIO 19 | PUMP_CTRL | Relay IN | Green |

## 🧪 Pre-Upload Testing

### Visual Inspection
- [ ] All connections are secure and not loose
- [ ] No exposed wires touching each other (short circuit risk)
- [ ] Voltage divider resistors are correctly connected
- [ ] Power supply cables are connected to correct terminals
- [ ] USB cable is connected for programming

### Power Test
1. Connect 5V power supply (do NOT connect pump power yet)
2. Check if ESP32 LED indicator turns on
3. Verify relay module LED indicates correct status
4. No smoke or burning smell!

## 💾 Upload & Testing

1. **Upload Code**: Connect USB, upload `water_level_monitor.ino` via Arduino IDE
2. **Open Serial Monitor**: Baud 115200 to see debug messages
3. **WiFi Connection**: Look for "WaterSystem_ESP32" network
4. **Access Dashboard**: Navigate to `192.168.4.1` in browser
5. **Test Manually**: Wave hand over sensor to test distance reading

## 🔌 Pump Power Connection (Final Step)

**WARNING: High Voltage - Ensure proper connections**

1. Connect 12V power supply to relay module
2. Verify relay clicks when GPIO 19 goes HIGH (in Serial Monitor)
3. Test pump manually in web interface
4. Monitor temperature - relay may get warm

## ⚠️ Safety Precautions

1. **Always disconnect power before making wiring changes**
2. **Use proper gauges wire for pump power (12V lines)**
3. **Test with multimeter** before connecting to power
4. **Keep away from water** during initial testing
5. **Verify polarity** of all power connections
6. **Use fused power supply** for protection
7. **Never test with pump submerged** without proper waterproofing

## 🚨 Common Wiring Issues

| Issue | Cause | Solution |
|-------|-------|----------|
| No distance reading (999cm) | ECHO pin not connected | Check GPIO 18 wiring, verify voltage divider |
| ESP32 won't upload | USB not connected | Use proper USB cable, try different port |
| Relay doesn't click | GPIO 19 not connected | Check relay wiring, test pin with LED |
| Sensor reading errors | Power supply unstable | Use quality 5V supply, add capacitor |
| WiFi won't start | Power issue | Verify 5V supply, check current rating |

## 📊 Testing Checklist

- [ ] Sensor correctly measures distance
- [ ] WiFi connects and dashboard loads
- [ ] Manual mode: buttons control pump
- [ ] Auto mode: pump turns on/off at thresholds
- [ ] Data updates every 2 seconds
- [ ] Relay clicks audibly when pump toggles
- [ ] No overheating of components

## 🔧 Optimization Tips

1. **Add Capacitors**: 10µF near 5V and GND for power stability
2. **Use Shielded Cable**: For ECHO line to reduce noise
3. **Ferrite Toroid**: On power lines to suppress RF interference
4. **Waterproof Case**: Consider IP65 enclosure for real deployment
5. **Cooling**: Add heatsink to relay if running continuously

---

**Setup Complete! Ready to deploy your water monitoring system.** 💧
