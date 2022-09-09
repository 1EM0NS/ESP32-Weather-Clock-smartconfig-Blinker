# ESP32-Weather-Clock-smartconfig-Blinker



## Software

### 1.Weather-clock

​	调用心知天气API获取当前位置天，私钥、地区更改在show_temp方法内

### 2.Smartconfig

​	ESP一键玄学配网在网络良好的情况下可以快速接收UDP报文，但是玄学的时候非常玄学，想要稳定建议直接烧录ssid与psw。

### 3.Blinker

​	利用Blinker接入米家实现语音联动，代码内还有一个button，不用可以忽略

### 4.Dependency

```
|-- TFT_eSPI @ 2.4.75
|   |-- SPI @ 2.0.0
|   |-- SPIFFS @ 2.0.0
|   |   |-- FS @ 2.0.0
|   |-- FS @ 2.0.0
|   |-- LittleFS @ 2.0.0
|   |   |-- FS @ 2.0.0
|-- IRremoteESP8266 @ 2.8.2
|-- NTPClient @ 3.2.1
|-- ArduinoJson @ 6.19.4
|-- IRsendMeidi
|   |-- IRremoteESP8266 @ 2.8.2
|-- Blinker @ 0.3.9
|   |-- Update @ 2.0.0
|   |-- ESPmDNS @ 2.0.0
|   |   |-- WiFi @ 2.0.0
|   |-- WiFi @ 2.0.0
|   |-- ESP32 BLE Arduino @ 2.0.0
|   |-- EEPROM @ 2.0.0
|   |-- HTTPClient @ 2.0.0
|   |   |-- WiFi @ 2.0.0
|   |   |-- WiFiClientSecure @ 2.0.0
|   |   |   |-- WiFi @ 2.0.0
|   |-- Ticker @ 2.0.0
|   |-- WiFiClientSecure @ 2.0.0
|   |   |-- WiFi @ 2.0.0
|   |-- FS @ 2.0.0
|   |-- SPIFFS @ 2.0.0
|   |   |-- FS @ 2.0.0
|   |-- Ethernet @ 2.0.0
|   |   |-- WiFi @ 2.0.0
|   |-- SPI @ 2.0.0
|-- SPI @ 2.0.0
|-- WiFi @ 2.0.0
```

---



## Hardware

- ESP WROOM 32
- TFT 1.3
- 红外发射模块(不知道空调编码需要购买接收模块)
- 3D打印机(用于打印模型外壳)

 ![img](https://raw.githubusercontent.com/1EM0NS/ESP32-Weather-Clock-smartconfig-Blinker/main/img/1.png)


---

