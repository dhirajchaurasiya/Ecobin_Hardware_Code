# ECOBIN 🛠️  
**Hardware Integration for Waste2Worth**  

The hardware counterpart to the [Waste2Worth](https://github.com/dhirajchaurasiya/Waste2Worth) app, enabling IoT-based smart composting monitoring. This repository contains firmware for sensor data collection, device communication, and integration with the Waste2Worth Flutter app.  

---

## 🌟 **Key Features**  
- **Real-time Sensor Monitoring**: Measures temperature, humidity, pH, and gas levels (NH₃/CO₂).  
- **WiFi/Bluetooth Connectivity**: ESP32-based communication with the Waste2Worth app.  
- **Data Logging**: Stores latest compost metrics when offline.  

---

## 🛠️ **Hardware Components**  
| Component | Quantity | Purpose |  
|-----------|----------|---------|  
| **ESP32** | 1 | Main microcontroller (WiFi/BLE) |  
| **DHT11** | 1 | Temperature & humidity sensing |  
| **MQ-135** | 1 | Gas (NH₃/CO₂) detection |  
| **pH Sensor** | 1 | Compost acidity monitoring |  
 

---

## ⚙️ **Software Stack**  
Need these dependencies:
- **PlatformIO**: Core development framework.  
- **Libraries**:  
  - `DHT sensor library` (Temperature/Humidity)    
  - `Esp32 Firebase Client` (Firebase integration)  

---

## 📦 **Setup & Installation**  
### **1. Clone the Repository**  
```bash  
git clone https://github.com/your-username/ECOBIN.git  
cd ECOBIN  
```

## 2. PlatformIO Setup
- Install PlatformIO VSCode extension.
- Open the project folder in VSCode.

## 3. Configure Hardware
Edit these values in the `src/main.cpp` file:

```cpp
#define WIFI_SSID "Your_SSID"  
#define WIFI_PASS "Your_Password"  
#define FIREBASE_HOST "YOUR_FIREBASE_REAL_TIME_DATABASE_URL" // without https:// and trailing slash
#define FIREBASE_AUTH "YOUR_FIREBASE_PROJECT_API_KEY"  
```

## 4. Install the Dependencies
- Import the required libraries by downloading the zip files of the respective directives and place them in the `lib` directory after unzipping.

## 5. Upload to Device
- Connect ESP32 via USB.
- Click Upload in PlatformIO.

## 6. Repository Structure
ECOBIN/  
├── lib/  &nbsp;&nbsp;&nbsp;&nbsp;  # External libraries imported (PlatformIO manages dependencies)  
├── src/  &nbsp;&nbsp;&nbsp;&nbsp;  # Main firmware code  
│   ├── main.cpp  &nbsp;&nbsp;&nbsp;&nbsp;    # Core code    
├── test/         &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    # Unit tests (PlatformIO test framework)  
├── platformio.ini  &nbsp;&nbsp;&nbsp;&nbsp;  # Build configuration  
└── README.md         &nbsp;&nbsp;&nbsp;&nbsp;

## 7. 🔌 Integration with Waste2Worth App
#### Data Flow:
Sensors → ESP32 → (MQTT) → Firebase → Flutter App

#### Protocol: JSON payload format:
```json
{  
  "temperature": 25.6,  
  "humidity": 60,  
  "ph": 6.8,  
  "gas_ppm": 120  
}  
```

## 8. 🚀 Deployment
- Assemble hardware in compost bin (waterproof enclosure recommended).
- Power options:
  - 18650 Li-ion battery (solar-charged)
  - 5V DC adapter