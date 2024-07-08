#include <esp_task_wdt.h>
#include <SPI.h>
#include <mcp2515.h>
#include <ArduinoJson.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <TimeLib.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <EEPROM.h>
#include <Fonts/FreeMono9pt7b.h>
#include "SPIFFS.h"
#include "FS.h"
#include "SD.h"
#include <Adafruit_ADS1X15.h>

String device_name = "SDFMS ESP - DualCore";
String version = "Firm v2.1";
String type_board = "ESP-Main v2";

#define WDT_TIMEOUT 60
#define RXPin 32
#define TXPin 33
#define GPSBaud 9600

#define de_re 13
#define RX_rs485 26
#define TX_rs485 25

#define CANBUS_CS 4
#define FATIGUE_BTN 34
#define tombolADC 36
#define in_pto 35
#define kontak_on 39
#define addressEepromCN 15
#define addressEeprombrand 30
#define addressEeprombitrate 40
#define RE1 27
#define buzzer 15
#define selenoid 14
#define ads_address 0x48

boolean ENTER_BTN = false;
boolean SELECT_BTN = false;
boolean UP_BTN = false;
boolean DOWN_BTN = false;
int countCN, digit, enCn, con;
String cnNow, cnFix, lockCN;
char saveCNWrite[10], saveCNRead[11];
String cn[19] = {"ADT", "DT", "DA", "DTB", "P", "F", "PPA", "AMM", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

const byte SCREEN_WIDTH = 128;
const byte SCREEN_HEIGHT = 64;

int timeLimit = 300; // ← Fatigue time in second
int timeFatigue = 10; // ← If > 10 second, driver fatigue
int count, countFatigue, counter;
boolean logic = true;
boolean fatigue = false;
unsigned long previousMillisCount = 1000;
unsigned long IntervalCount = 3500; // ← Milisecond of always push button
unsigned long previousMillisFatigue = 1000;

unsigned long PMillisRE1 = 0;
int intervalRE1 = 400;
boolean RE1state = false;

unsigned long PMillisbuzzer = 0;
int intervalbuzzer = 200;
boolean buzzerstate = false;

unsigned long menuStartTime = 0;
byte menuItem = 0;
const int menuTimeout = 10000;
bool precisionMode = false;

String dates, timess;
String TruckID, en_fatiq, en_fms, en_overspeed, date_time, limit_speed, input;
float incl, grde, latitude, longitude, alt, speed;
int heading, sat;
String incl_str, st_str, times, tanggal, tanggalPost;
String Data = "";
String kondisiFatigue = "", dump_status;
char wjam[10], wtanggal[10], xtanggal[10];
const int UTC_offset = 8;

unsigned long intervalL = 3000;  //Send Serial Data
unsigned long previousMillisL = 0;
byte devAddr = 0x68;

int adc_pto, adc_kontak;
bool pto, dmpReady = false;
byte mpuIntStatus;
byte devStatus;
int packetSize;
int fifoCount;
byte fifoBuffer[64];

struct can_frame canMsg;
int canid;
int line = 0;
int data[8];
unsigned long pgnID;
String cn_str;
String brand_str;
String bitrate_str;
String relay_buzzer, relay_dump;
bool enScanBitrate = true;
int countBitrate;
bool statusCanBus_connection, onOTA;
unsigned long lastMillisBitrate, last;

String date_str;
String time_str;
String torque_str;
String rpm_str;

String ignition;
String engHm_str; // Engine Hours Meter
String engEr_str; // Engine Revolution
String clnTemp_str; // Coolant Temperature
String retTemp_str; // Retarder Temperature
String gearPos_str; // Gear Position
String engOilPress_str; // Engine Oil Pressure
String airPress1_str; // Air Pressure 1
String airPress2_str; // Air Pressure 2
String airPress3_str; // Air Pressure 3
String difLockSta_str; // Differential Lock Status
String accPed_str; // Acceleratol Pedal
String retLvlPos_str; // Retarder Level Position
String vehSpeed_str; // Vehicle Speed

String brand, payload, set_bitrate, en_anglewarning, limit_angle;
String engTorque_str, engSpeed_str, tacVehSpeed_str, engLoad_str, actRet_str, brakePedPos_str, fuelRate_str, engClnTemp_str, fuelLvl_str;
String dateGps_str, timeGps_str, lat_str, lng_str, alt_str, spd_str, heading_str, sat_str;
String set_brand, receiveCN, enCn_str, kalibrasi_str;
int  kalibrasi;
float angVal, dispRotate;

byte count_stuck, en_rst = false, en_rst1 = false;
unsigned long lastMillisDebug, prev_stuck, prev_can;
unsigned long pngID;

int xPos, yPos;
int16_t adc0, adc1, adc2, adc3;

String gyro;
int count_rst;
long prev_rst, prev_rs485, prev_fail;
long prev_kalibrasi;
long prev_kontakoff;
bool notif_pto;

bool alarmActive = false;
unsigned long previousMillis_alram = 0;
unsigned long interval_alarm = 0;
byte remainingBeeps = 0;
bool state_calibration, buzzerState = false;
bool eror_canbus, eror_lcd, eror_gps, eror_rs485, eror_communication;

time_t prevDisplay = 0;
MCP2515 mcp2515(4);
TinyGPSPlus gps;
SoftwareSerial serial_gps(RXPin, TXPin);
SoftwareSerial rs485(RX_rs485, TX_rs485);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_ADS1015 ads;

String eror_kode() {
  String state_eror = "";
  DynamicJsonDocument buffer(1024);
  JsonObject obj             = buffer.to<JsonObject>();
  obj["state_canbus"]        = eror_canbus;
  obj["state_lcd"]           = eror_lcd;
  obj["state_gps"]           = eror_gps;
  obj["state_rs485"]         = eror_rs485;
  obj["state_communication"] = eror_communication;
  serializeJson(obj, state_eror);
  return state_eror;
}

void setup() {
  Serial.begin(115200);
  setup_rs485();
  setup_komunikasi();
  SPIFFS.begin();
  read_setting();
  setupOled();
  setupIo();
  read_kontak();
  main_setup();
  setupGps();
  esp_task_wdt_init(WDT_TIMEOUT, true);
  esp_task_wdt_add(NULL);
  delay(150);
}
   
void loop() {
  loop_rs485();
  updateAlarm();
  receive_json();
  main_loop();
  switch_kontak();
  loopGps();
  serial_monitor();
  esp_task_wdt_reset();
  delay(10);
}
