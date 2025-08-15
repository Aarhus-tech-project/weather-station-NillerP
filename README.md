[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/XBO6NBqk)

# Weather Station Project

This repository contains three main components:

- Arduino: Firmware and libraries for the weather station hardware.
- Mqqt-client: C++ MQTT client for data collection and integration.
- WeatherStationWeb: Web for visualizing the data collected.

## Structure

Arduino/
├── src/ # Arduino source code
├── platformio.ini

Mqqt-client/
├── src/ # C++ source code
├── build/ # Build output
├── CMakeLists.txt # CMake config

## Getting Started

### Arduino

1. Install [PlatformIO](https://platformio.org/).
2. Open the `Arduino/` folder in VSCode.
3. Build and upload using PlatformIO.

## Arduino (PlatformIO) dependencies:

Adafruit BME280 Library
Adafruit Unified Sensor
PubSubClient
ArduinoJson

### Mqqt-client

1. Install CMake and a C++17 compiler (MinGW recommended).
2. Run:
   cd Mqqt-client
   mkdir build
   cd build
   cmake ..
   make
3. Run the executable in `build/HelloWin.exe`.

## Dependencies

- [Standard C++ libraries] <iostream>, <string>, <csignal>
- [nlohmann/json] (JSON parsing library)
- [Mosquitto](https://mosquitto.org/) (MQTT library)
- [libpqxx](https://github.com/jtv/libpqxx) (PostgreSQL C++ library)

### WeaterStationWeb

1. npm Install
2. npm run dev
3. Open [http://localhost:3000](http://localhost:3000) with your browser to see the result.
