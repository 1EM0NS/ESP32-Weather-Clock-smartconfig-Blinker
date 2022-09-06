#include <Arduino.h>
#include <SPI.h> 
#include <TFT_eSPI.h>
#include <font.h>
#include <img.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <String.h>
#include "IRsendMeidi.h"
int count =0;
bool WIFI_Status =true;
#define BLINKER_MIOT_AIRCONDITION
// #define BLINKER_PRINT Serial
#define BLINKER_WIFI
#define BLINKER_ESP_SMARTCONFIG
#include <Blinker.h>
unsigned int c, h;
char hot[] = "heater";
char cool[] = "eco";
bool oState = false;
bool hsState = false;
bool vsState = false;
uint8_t setLevel;
const uint8_t IR_LED = 4;        
IRsendMeidi irsendmeidi(IR_LED);
BlinkerButton Button1("btn-667");
unsigned int current_t;
unsigned long epochTime;
unsigned int bg;
unsigned int nw, tw, aw;
bool night;
const char *auth = "  ";  //你的Blinker设备识别码
// const char *ssid = "Mi 10 Ultra"; 
// const char *password = "12345678";
const char *ssid = "网_ap"; 
const char *password = "178490049";
char determineqing[] = "晴";
char determineduoyun[] = "多云";
char determineyin[] = "阴";
char determineyu[] = "雨";
const char *host = "api.seniverse.com"; 
String now_address = "", now_high_tem = "", now_low_tem = "", now_rainfall = "", now_wind_direction = "",
       now_hum = "", now_weather = "";

String t_address = "", t_high_tem = "", t_low_tem = "", t_rainfall = "", t_wind_direction = "",
       t_hum = "", t_weather = "";
String a_address = "", a_high_tem = "", a_low_tem = "", a_rainfall = "", a_wind_direction = "",
       a_hum = "", a_weather = "";

struct timer
{
  String year;
  String month;
  String day;
  String hour;
  unsigned int minutes;
  String weekday;
  unsigned int dayi;
  unsigned int dayii;
};
struct timer n_time;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
TFT_eSPI tft = TFT_eSPI();

// /*******************pictures displaying****************/
// void showImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data);

// void showImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data){
//   int32_t dx = 0;
//   int32_t dy = 0;
//   int32_t dw = w;
//   int32_t dh = h*2;

//   if (x < 0) { dw += x; dx = -x; x = 0; }
//   if (y < 0) { dh += y; dy = -y; y = 0; }

//   if (dw < 1 || dh < 1) return;

//   CS_L;

//   data += dx + dy * w;

//   uint16_t  buffer[PI_BUF_SIZE];
//   uint16_t* pix_buffer = buffer;
//   uint16_t  high,low;

//   tft.setWindow(x, y, x + dw - 1, y + dh - 1);

//   // Work out the number whole buffers to send
//   uint16_t nb = (dw * dh) / (2 * PI_BUF_SIZE);

//   // Fill and send "nb" buffers to TFT
//   for (int32_t i = 0; i < nb; i++) {
//     for (int32_t j = 0; j < PI_BUF_SIZE; j++) {
//       high = pgm_read_word(&data[(i * 2 * PI_BUF_SIZE) + 2 * j + 1]);
//       low = pgm_read_word(&data[(i * 2 * PI_BUF_SIZE) + 2 * j ]);
//       pix_buffer[j] = (high<<8)+low;
//     }
//     tft.pushPixels(pix_buffer, PI_BUF_SIZE);
//   }

//   // Work out number of pixels not yet sent
//   uint16_t np = (dw * dh) % (2 * PI_BUF_SIZE);

//   // Send any partial buffer left over
//   if (np) {
//     for (int32_t i = 0; i < np; i++)
//     {
//       high = pgm_read_word(&data[(nb * 2 * PI_BUF_SIZE) + 2 * i + 1]);
//       low = pgm_read_word(&data[(nb * 2 * PI_BUF_SIZE) + 2 * i ]);
//       pix_buffer[i] = (high<<8)+low;
//     }
//     tft.pushPixels(pix_buffer, np);
//   }

//   CS_H;
// }

/*******************Single pixel display****************/
void showMyFont(int32_t x, int32_t y, const char c[3], uint32_t color)
{
  for (int k = 0; k < 29; k++) 
    if (hanzi[k].Index[0] == c[0] && hanzi[k].Index[1] == c[1] && hanzi[k].Index[2] == c[2])
    {
      tft.drawBitmap(x, y, hanzi[k].hz_Id, hanzi[k].hz_width, hanzi[k].hz_width, color);
    }
}
/*******************sentence display****************/
void showMyFonts(int32_t x, int32_t y, const char str[], uint32_t color, const int b)
{
  int x0 = x;
  for (int i = 0; i < strlen(str); i += 3)
  {
    showMyFont(x0, y, str + i, color);
    x0 += b;
  }
}

void time_init()
{
  timeClient.begin();
  timeClient.setTimeOffset(28800); 
}

void get_time()
{

  timeClient.update();
  delay(100);
  epochTime = timeClient.getEpochTime();
  // Serial.print("Epoch Time: ");
  // Serial.println(epochTime);
  //打印时间

  n_time.hour = timeClient.getHours();
  // Serial.print("Hour: ");
  // Serial.println(currentHour);

  n_time.minutes = timeClient.getMinutes();
  // Serial.print("Minutes: ");
  // Serial.println(currentMinute);

  switch (timeClient.getDay())
  {
  case 1:
    n_time.weekday = "周一";
    break;

  case 2:
    n_time.weekday = "周二";
    break;
  case 3:
    n_time.weekday = "周三";
    break;
  case 4:
    n_time.weekday = "周四";
    break;
  case 5:
    n_time.weekday = "周五";
    break;
  case 6:
    n_time.weekday = "周六";
    break;
  case 7:
    n_time.weekday = "周日";
    break;
  }

  struct tm *ptm = gmtime((time_t *)&epochTime);
  n_time.day = ptm->tm_mday;
  n_time.dayi = (ptm->tm_mday) + 1;
  n_time.dayii = (ptm->tm_mday) + 2;

  n_time.month = ptm->tm_mon + 1;

  n_time.year = ptm->tm_year + 1900;

  if (timeClient.getHours() > 17 || timeClient.getHours() < 6)
    night = true;
}


void smartConfig()
{

Serial.println("\r\nWait for Smartconfig...");

Blinker.begin(auth);
delay(1000);
while (1)
{
tft.print(WiFi.status());
Serial.print(WiFi.status());
delay(1000);
if (WiFi.status()==3)
{
Serial.println("SmartConfig Success");
Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
// Blinker.begin(auth,WiFi.SSID().c_str(),WiFi.psk().c_str());

break;
}
}
}

void connect_wifi()
{
    tft.pushImage(0, 0, 240, 240, start_display);
        smartConfig();
          Serial.println("连接成功");
    Serial.print("IP:");
    Serial.println(WiFi.localIP());
    }


void startup_display()
{
  tft.init();
  tft.setSwapBytes(true);
  connect_wifi();
  tft.pushImage(0, 0, 240, 240, success_con);

  delay(500);
}
void start_temp()
{
  // Serial.begin(9600);

  const int httpPort = 80;
  client.connect(host, httpPort);
  delay(100);
  // if (!client.connect(host, httpPort))
  // {
  //   Serial.println("connection failed"); 
  //   return;
  // }
}
void get_temp()
{
  start_temp();
  // URL请求地址
  String url = "/v3/weather/daily.json?key=你的心知天气私钥&location=jiangsusuzhou&language=zh-Hans&unit=c&start=0&days=5";
  //发送网络请求
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(1000);

  String answer;
  while (client.available())
  {
    String line = client.readStringUntil('\r');
    answer += line;
  }
  // client.stop();
  // Serial.println();
  // Serial.println("closing connection");
  String jsonAnswer;
  int jsonIndex;
  for (int i = 0; i < answer.length(); i++)
  {
    if (answer[i] == '{')
    {
      jsonIndex = i;
      break;
    }
  }
  jsonAnswer = answer.substring(jsonIndex);
  // Serial.println();
  // Serial.println("JSON answer: ");
  // Serial.println(jsonAnswer);

  StaticJsonDocument<1536> doc;

  DeserializationError error = deserializeJson(doc, jsonAnswer);

  if (error)
  {
    // Serial.print("deserializeJson() failed: ");
    // Serial.println(error.c_str());
    return;
  }

  JsonObject results_0 = doc["results"][0];

  JsonObject results_0_location = results_0["location"];
  const char *results_0_location_id = results_0_location["id"];                           // "WTTDPCGXTWUS"
  const char *results_0_location_name = results_0_location["name"];                       // "苏州"
  const char *results_0_location_country = results_0_location["country"];                 // "CN"
  const char *results_0_location_path = results_0_location["path"];                       // "苏州,苏州,江苏,中国"
  const char *results_0_location_timezone = results_0_location["timezone"];               // "Asia/Shanghai"
  const char *results_0_location_timezone_offset = results_0_location["timezone_offset"]; // "+08:00"
  unsigned int k = 0;
  for (JsonObject results_0_daily_item : results_0["daily"].as<JsonArray>())
  {

    const char *results_0_daily_item_date = results_0_daily_item["date"];         // "2022-08-22", "2022-08-23", ...
    const char *results_0_daily_item_text_day = results_0_daily_item["text_day"]; // "晴", "多云", "雷阵雨"

    const char *results_0_daily_item_code_day = results_0_daily_item["code_day"];     // "0", "4", "11"
    const char *results_0_daily_item_text_night = results_0_daily_item["text_night"]; // "晴", "雷阵雨", "多云"

    const char *results_0_daily_item_code_night = results_0_daily_item["code_night"];         // "1", "11", "4"
    const char *results_0_daily_item_high = results_0_daily_item["high"];                     // "39", "37", "30"
    const char *results_0_daily_item_low = results_0_daily_item["low"];                       // "28", "27", "27"
    const char *results_0_daily_item_rainfall = results_0_daily_item["rainfall"];             // "0.00", "29.50", "4.23"
    const char *results_0_daily_item_precip = results_0_daily_item["precip"];                 // "0.00", "0.96", "0.49"
    const char *results_0_daily_item_wind_direction = results_0_daily_item["wind_direction"]; // "西南", "东北", ...
    const char *results_0_daily_item_wind_direction_degree = results_0_daily_item["wind_direction_degree"];
    const char *results_0_daily_item_wind_speed = results_0_daily_item["wind_speed"]; // "23.4", "23.4", ...
    const char *results_0_daily_item_wind_scale = results_0_daily_item["wind_scale"]; // "4", "4", "3"
    const char *results_0_daily_item_humidity = results_0_daily_item["humidity"];     // "74", "83", "80"

    if (k == 0)
    {
      now_weather = results_0_daily_item_text_day;
      now_high_tem = results_0_daily_item_high;
      now_low_tem = results_0_daily_item_low;
      now_rainfall = results_0_daily_item_rainfall;
      now_hum = results_0_daily_item_humidity;
      now_address = results_0_location_name;
      now_wind_direction = results_0_daily_item_wind_direction;
      if (strstr(now_weather.c_str(), determineqing) != 0)
      {
        now_weather = "晴";
        nw = 0;
      }
      if (strstr(now_weather.c_str(), determineduoyun) != 0)
      {
        now_weather = "多云";
        nw = 1;
      }
      if (strstr(now_weather.c_str(), determineyin) != 0)
      {
        now_weather = "阴";
        nw = 2;
      }
      if (strstr(now_weather.c_str(), determineyu) != 0)
      {
        now_weather = "雨";
        nw = 3;
      }
    }

    else if (k == 1)
    {
      t_weather = results_0_daily_item_text_day;
      t_high_tem = results_0_daily_item_high;
      t_low_tem = results_0_daily_item_low;
      t_rainfall = results_0_daily_item_rainfall;
      t_hum = results_0_daily_item_humidity;
      t_address = results_0_location_name;
      t_wind_direction = results_0_daily_item_wind_direction;
      if (strstr(t_weather.c_str(), determineqing) != 0)
      {
        t_weather = "晴";
        tw = 0;
      }
      if (strstr(t_weather.c_str(), determineduoyun) != 0)
      {
        t_weather = "多云";
        tw = 1;
      }
      if (strstr(t_weather.c_str(), determineyin) != 0)
      {
        t_weather = "阴";
        tw = 2;
      }
      if (strstr(t_weather.c_str(), determineyu) != 0)
      {
        t_weather = "雨";
        tw = 3;
      }
    }

    else
    {
      a_weather = results_0_daily_item_text_day;
      a_high_tem = results_0_daily_item_high;
      a_low_tem = results_0_daily_item_low;
      a_rainfall = results_0_daily_item_rainfall;
      a_hum = results_0_daily_item_humidity;
      a_address = results_0_location_name;
      a_wind_direction = results_0_daily_item_wind_direction;
      if (strstr(a_weather.c_str(), determineqing) != 0)
      {
        a_weather = "晴";
        aw = 0;
      }
      if (strstr(a_weather.c_str(), determineduoyun) != 0)
      {
        a_weather = "多云";
        aw = 1;
      }
      if (strstr(a_weather.c_str(), determineyin) != 0)
      {
        a_weather = "阴";
        aw = 2;
      }
      if (strstr(a_weather.c_str(), determineyu) != 0)
      {
        a_weather = "雨";
        aw = 3;
      }
    }
    ++k;
  }

  const char *results_0_last_update = results_0["last_update"]; // "2022-08-22T08:00:00+08:00"
}

void show_txt(uint16_t x, uint16_t y, uint16_t c, uint8_t s, uint8_t f, const String data)
{
  tft.setCursor(x, y, f);
  tft.setTextColor(c);
  tft.setTextSize(s);
  tft.println(data);
}
void show_weather()
{
  while (now_weather == "")
    get_temp;

  tft.pushImage(0, 0, 240, 240, weatherdisplay);
  tft.pushImage(0, 20, 80, 80, weather[nw], 0x0000);
  tft.pushImage(80, 20, 80, 80, weather[tw], 0x0000);
  tft.pushImage(160, 20, 80, 80, weather[aw], 0x0000);
  showMyFonts(1, 90, now_weather.c_str(), TFT_WHITE, 36);
  showMyFonts(80, 90, t_weather.c_str(), TFT_WHITE, 36);
  showMyFonts(160, 90, a_weather.c_str(), TFT_WHITE, 36);
  show_txt(10, 5, Time_YELLOW, 2, 1, n_time.month + "/" + n_time.day);
  show_txt(95, 5, Time_YELLOW, 2, 1, n_time.month + "/" + n_time.dayi);
  show_txt(175, 5, Time_YELLOW, 2, 1, n_time.month + "/" + n_time.dayii);

  show_txt(15, 140, TFT_WHITE, 2, 1, now_low_tem + ".C");
  show_txt(15, 160, TFT_WHITE, 2, 1, now_high_tem + ".C");
  show_txt(12, 180, TFT_WHITE, 2, 1, now_rainfall + "%");
  show_txt(95, 140, TFT_WHITE, 2, 1, t_low_tem + ".C");
  show_txt(95, 160, TFT_WHITE, 2, 1, t_high_tem + ".C");
  show_txt(92, 180, TFT_WHITE, 2, 1, t_rainfall + "%");
  show_txt(175, 140, TFT_WHITE, 2, 1, a_low_tem + ".C");
  show_txt(175, 160, TFT_WHITE, 2, 1, a_high_tem + ".C");
  show_txt(168, 180, TFT_WHITE, 2, 1, a_rainfall + "%");
  // tft.println(now_wind_direction+" %");
  for (int16_t i = 0; i < 50; i++)
  {
    Blinker.run();
    delay(100);
  }
}

void show_time()
{
  get_time();
  Blinker.run();
  current_t = epochTime;
  tft.pushImage(0, 0, 240, 240, get_time_model);
  if (timeClient.getMinutes() < 10 || timeClient.getMinutes() == 0)
    show_txt(8, 90, Time_YELLOW, 5, 1, n_time.hour + ":" + "0" + n_time.minutes);
  else
    show_txt(8, 90, Time_YELLOW, 5, 1, n_time.hour + ":" + n_time.minutes);

  show_txt(24, 18, Time_YELLOW, 3, 1, n_time.year + "/" + n_time.month + "/" + n_time.day);
  showMyFonts(15, 170, n_time.weekday.c_str(), Time_YELLOW, 50);
  while (epochTime - current_t < 5)
  {
    Blinker.run();

    for (int16_t i = 0; i < 20; i++)
    {
      tft.pushImage(102, 128, 138, 112, capoo[i], 0x0000);
      delay(100);
    }
    get_time();
  }
}

void button1_callback(const String &state)
{
  irsendmeidi.setPowers(1); //打开空调
  delay(5000);
  irsendmeidi.setModes(1); //设置为制冷模式
  delay(5000);
  // irsendmeidi.setTemps(26); //设置温度为26度
  // delay(5000);
}

void miotPowerState(const String &state)
{
  BLINKER_LOG("need set power state: ", state);

  if (state == BLINKER_CMD_ON)
  {
    irsendmeidi.setPowers(1); //打开空调
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();
    oState = true;
  }
  else if (state == BLINKER_CMD_OFF)
  {
    irsendmeidi.setPowers(0); //关闭空调
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();
    oState = false;
  }
}
void miotVSwingState(const String &state)
{
  BLINKER_LOG("need set VSwing state: ", state);
  // vertical-swing

  if (state == BLINKER_CMD_ON)
  {
    irsendmeidi.setSwingUD(1);
    BlinkerMIOT.vswing("on");
    BlinkerMIOT.print();

    vsState = true;
  }
  else if (state == BLINKER_CMD_OFF)
  {
    irsendmeidi.setSwingUD(0);
    BlinkerMIOT.vswing("off");
    BlinkerMIOT.print();
    vsState = false;
  }
}
void miotLevel(uint8_t level)
{
  switch (level)
  {
  case 1:
    irsendmeidi.setFanSpeeds(3);
    break;
  case 2:
    irsendmeidi.setFanSpeeds(4);
  case 3:
    irsendmeidi.setFanSpeeds(5);
    break;
  }
  BLINKER_LOG("need set level: ", level);
  // 0:AUTO MODE, 1-3 LEVEL

  BlinkerMIOT.level(level);
  BlinkerMIOT.print();
}
void miotMode(const String &mode, const String &state)
{

  if (strstr(mode.c_str(), cool) != 0)
    irsendmeidi.setModes(1);
  if (strstr(mode.c_str(), hot) != 0)
    irsendmeidi.setModes(2);
  // eco ECO节能模式
  // anion 负离子
  // heater 辅热功能
  // dryer 干燥功能
  // sleep 睡眠模式
  // soft 柔风功能
  // uv UV杀菌
  // unsb un-straight-blowing 防直吹

  BLINKER_LOG("need set mode: ", mode, ", state:", state);

  BlinkerMIOT.mode(mode, state);
  BlinkerMIOT.print();
}
void miotTemp(uint8_t temp)
{
  BLINKER_LOG("need set temp: ", temp);
  irsendmeidi.setTemps(temp);
  BlinkerMIOT.temp(temp);
  BlinkerMIOT.print();
}

void setup()
{Serial.begin(9600);
BLINKER_DEBUG.stream(Serial);

BLINKER_DEBUG.debugAll();
  irsendmeidi.begin_2();                                     //初始化
  irsendmeidi.setZBPL(38);                                   //设置红外载波频率，单位kHz,不调用此函数则默认38，由于未知原因，我设置为40，示波器测得频率为38左右，当发送信号后没反应时，尝试更改此值。
  irsendmeidi.setCodeTime(500, 1600, 550, 4400, 4400, 5220); //设置信号的高低电平占比，分别为标记位，1位，0位，前导码低电平，前导码高电平，间隔码高电平
                                                             //不调用此函数默认为（500,1600,550,4400,4400,5220）

  startup_display();
  BlinkerMIOT.attachPowerState(miotPowerState);
  BlinkerMIOT.attachVSwing(miotVSwingState);
  BlinkerMIOT.attachLevel(miotLevel);
  BlinkerMIOT.attachMode(miotMode);
  BlinkerMIOT.attachTemp(miotTemp);
  Button1.attach(button1_callback);

  time_init();
  get_time();
  get_temp();
  get_time();
  get_temp();
  // show_time();
  // // showMyFonts(92,11,now_address.c_str(),TFT_GREENYELLOW,38);
  // // showMyFonts(92,50,now_weather.c_str(),TFT_GREENYELLOW,38);
  // show_weather();
}

void loop()
{
  show_time();
  if (n_time.minutes == 0)
    get_temp();
  show_weather();
  Blinker.run();
  // get_time();
  //   if (epochTime-current_t>5)
  //      {current_t = epochTime;
  //       get_time();
  //       get_temp();
  //       show_bg();
  //       show_time();
  //       show_weather();
  //       }
  // tft.fillScreen(TFT_GREEN);

  // tft.fillScreen(TFT_RED);

  // tft.fillScreen(TFT_WHITE);

  // tft.fillScreen(TFT_BLUE);

  // tft.fillRect(30, 200, 180, 30,TFT_GREEN);
}