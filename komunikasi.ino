void setup_komunikasi() {
  Serial2.begin(115200);
}

void receive_json() {
  if (Serial2.available()) {
    eror_communication = false;
    Serial.println();
    Serial.println("================= json received =================");
    StaticJsonDocument<1024> doc;
    input = Serial2.readStringUntil('\n');
    Serial.println(input);

    DeserializationError error = deserializeJson(doc, input);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      Serial.println();
    }

    JsonObject obj = doc.as<JsonObject>();

    kalibrasi_str = obj["kalibrasi"].as<String>();
    kalibrasi = kalibrasi_str.toInt();
    enCn_str = obj["en_cn"].as<String>();
    receiveCN = obj["cn"].as<String>();
    set_brand = obj["set_brand"].as<String>();
    String xbrand = obj["brand"].as<String>();
    String xen_fatiq = obj["en_fatiq"].as<String>();
    String xen_fms = obj["en_fms"].as<String>();
    String xen_overspeed = obj["en_overspeed"].as<String>();
    String xdate = obj["date"].as<String>();
    String xtime = obj["time"].as<String>();
    String xlimit_speed = obj["limit_speed"].as<String>();
    String xset_bitrate = obj["set_bitrate"].as<String>();
    String xen_anglewarning = obj["en_anglewarning"].as<String>();
    String xlimit_angle = obj["angle_warning"].as<String>();
    String xrestart = obj["restart"].as<String>();

    if (xrestart != "null") {
      if (xrestart == "1") {
        Serial.println("================");
        Serial.println("Device Restart");
        Serial.println("================");
        refresh_sensor();
        delay(3000);
        ESP.restart();
      }
    }


    if (xbrand != "null" and xbrand != "") {
      brand = xbrand;
      writeFile(SPIFFS, "/brand.txt", xbrand.c_str());
    }
    if (xen_fatiq != "null" and xen_fatiq != "") {
      en_fatiq = xen_fatiq;
      writeFile(SPIFFS, "/en_fatiq.txt", xen_fatiq.c_str());
    }
    if (xen_fms != "null" and xen_fms != "") {
      en_fms = xen_fms;
      writeFile(SPIFFS, "/en_fms.txt", xen_fms.c_str());
    }
    if (xen_overspeed != "null" and xen_overspeed != "") {
      en_overspeed = xen_overspeed;
      writeFile(SPIFFS, "/en_overspeed.txt", xen_overspeed.c_str());
    }
    if (xdate != "null" and xdate != "") {
      dates = xdate;
      date_time = dates + " " + timess;
    }
    if (xtime != "null" and xtime != "") {
      timess = xtime;
      date_time = dates + " " + timess;
    }
    if (xlimit_speed != "null" and xlimit_speed != "") {
      limit_speed = xlimit_speed;
      writeFile(SPIFFS, "/limit_speed.txt", xlimit_speed.c_str());
    }
    if (xen_anglewarning != "null" and xen_anglewarning != "") {
      en_anglewarning = xen_anglewarning;
      writeFile(SPIFFS, "/en_anglewarning.txt", xen_anglewarning.c_str());
    }
    if (xlimit_angle != "null" and xlimit_angle != "") {
      limit_angle = xlimit_angle;
      writeFile(SPIFFS, "/limit_angle.txt", xlimit_angle.c_str());
    }
    if (xset_bitrate != "null" and xset_bitrate != "") {
      if (set_bitrate != xset_bitrate) {
        Serial.println("==========================");
        Serial.println("Setting Bit Rate Available");
        Serial.println("==========================");
        set_bitrate = xset_bitrate;
        writeFile(SPIFFS, "/set_bitrate.txt", xset_bitrate.c_str());
      }
    }

    while (kalibrasi) {
      Serial.println("# Kalibrasi sensor from web server # [success]");
      Serial.println("SEKARANG KALIBRASI...");
      display.clearDisplay();
      display.setFont(&FreeMono9pt7b);
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.setCursor(15, 15);
      display.println(F("Kalibrasi"));
      display.drawFastHLine(0, 25, 128, SSD1306_WHITE);
      display.setCursor(30, 45);
      display.println(F("By Web"));
      display.setCursor(30, 60);
      display.println(F("Server"));
      display.display();
      delay(1000);
      calibration_sensor();
      kalibrasi = 0;
      state_calibration = true;
      prev_kalibrasi = millis();
    }

    while (state_calibration) {
      loop_rs485();
      if (angVal < 1 and angVal > -1 and incl_str != "" and incl_str != "Sensor_Failed") {
        Serial.println("KALIBRASI SELESAI!!!");
        display.clearDisplay();
        display.setCursor(0, 40);
        display.println(F("SUCCESS!!!"));
        display.display();
        delay(1000);
        state_calibration = false;
        prev_kalibrasi = millis();
      }
      else {
        if (millis() - prev_kalibrasi > 15000) {
          if (angVal < 1 and angVal > -1 and incl_str != "" and incl_str != "Sensor_Failed") {
            Serial.println("KALIBRASI SELESAI!!!");
            display.clearDisplay();
            display.setCursor(0, 40);
            display.println(F("SUCCESS!!!"));
            display.display();
            delay(1000);
            state_calibration = false;
            prev_kalibrasi = millis();
          }
          else {
            Serial.println("KALIBRASI GAGAL!!!");
            display.clearDisplay();
            display.setCursor(0, 40);
            display.println(F("FAILED!!!"));
            display.display();
            delay(1000);
            state_calibration = false;
            prev_kalibrasi = millis();
          }
        }
      }
    }

    if (receiveCN != "null") {
      String checkCn = readFile(SPIFFS, "/cn_str.txt");
      if (receiveCN != checkCn) {
        cn_str = receiveCN;
        writeFile(SPIFFS, "/cn_str.txt", receiveCN.c_str());
        String readSpiffs = readFile(SPIFFS, "/cn_str.txt");
        Serial.println(readSpiffs);
      }
    }
    while (enCn_str == "1" and receiveCN != "null") {
      writeFile(SPIFFS, "/cn_str.txt", receiveCN.c_str());
      cn_str = receiveCN;
      sukses_SetCN(cn_str);
      Serial.println();
      Serial.print("# Set CN from ESP32 : ");
      Serial.print(cn_str);
      Serial.println(" #  [success]");
      Serial.println();
      enCn_str = "0";
      delay(3000);
    }
    while (set_brand == "1") {
      writeFile(SPIFFS, "/brand.txt", brand.c_str());
      Serial.println();
      Serial.print("# Set Brand from ESP32 : ");
      Serial.print(brand);
      Serial.println(" #  [success]");
      Serial.println();
      set_brand = "0";
      delay(1000);
    }

    String output;
    serializeJsonPretty(doc, output);
    Serial.println("================= end =================");
    Serial.println();
  }
  else {
    static unsigned long prev_com = 0;
    if ((millis() - prev_com) > 10000) {
      prev_com = millis();
      eror_communication = true;
    }
  }
}
