# 🌍 IoT-Based Air Quality Monitoring System

An efficient and reliable IoT system designed to monitor air quality in various environments using ESP8266, DHT-11, and MQ-135 sensors. This project provides real-time insights via a web dashboard, with visual alerts through LEDs and buzzers for immediate on-site feedback.

---

## 📷 System Overview

This project enables users to:

- Monitor **temperature**, **humidity**, and **air pollutants**
- Receive **real-time alerts** for critical pollution levels
- Access data via a **remote dashboard**
- Get **visual and sound-based feedback** (LEDs and buzzers)

---

## 🧱 System Architecture

The architecture follows a **client-server model**:

- **ESP8266 (NodeMCU)**: Microcontroller to read and send data
- **DHT-11 Sensor**: Reads temperature and humidity
- **MQ-135 Sensor**: Measures harmful gases like CO₂, NH₃, NOₓ, etc.
- **Cloud Database**: Stores real-time and historical data
- **Dashboard Interface**: Web or mobile dashboard for users to monitor readings remotely

> The architecture is modular and easily scalable by integrating additional sensors or features.

---

## 🔁 Algorithm & Process Flow

1. **Initialization**
   - Connect ESP8266 to Wi-Fi
   - Initialize DHT-11 and MQ-135 sensors

2. **Data Acquisition**
   - Read environmental data from sensors

3. **Data Processing**
   - Compare readings with threshold values
   - Activate buzzer or LED alerts if pollution is high

4. **Data Storage**
   - Push sensor data to a cloud database

5. **Display Update**
   - Update the dashboard with real-time and historical data

6. **Remote Monitoring**
   - Allow users to access dashboard from any device

---

## 🔧 Hardware & Software Requirements

### Hardware
- ESP8266 (NodeMCU)
- DHT-11 Temperature & Humidity Sensor
- MQ-135 Gas Sensor
- Buzzer
- LED Indicators
- Jumper Wires
- Breadboard / PCB

### Software
- Arduino IDE
- Firebase / Thingspeak / Blynk (for database and dashboard)
- HTML/CSS/JS (for custom dashboard, optional)
- Internet connection for ESP8266

---

## 📊 System Diagram
![image](https://github.com/user-attachments/assets/883f96ee-822a-4a42-8c34-854571ac7821)

*Figure 1: Environment Air Quality Detection System*

---

## 🔁 Flowchart

![image](https://github.com/user-attachments/assets/6f66b41d-1d2c-4980-afa7-e494314566fd)
 
If you'd like, we can help design a flowchart using tools like Lucidchart, Draw.io, or Mermaid.js.

---

## 🚀 Getting Started

1. Clone this repo:
   ```bash
   git clone https://github.com/yourusername/air-quality-monitoring-iot.git
   cd air-quality-monitoring-iot
