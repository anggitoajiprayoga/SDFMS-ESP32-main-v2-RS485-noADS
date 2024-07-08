void main_setup() {
  Serial.println("===============SDFMS Module================");
  Serial.println("SYSTEM START");
  Serial.println();
  if (en_fms == "true") {
    setup_canbus();
  }
  if (en_fatiq == "true") {
    setup_fatiq();
  }
  if (en_overspeed == "true") {
    setup_overspeed();
  }
  
  if (adc_kontak == 0) {
    en_rst1 = true;
    Serial.println("kontak off");
  }
  else {
    disp_intro();
    adc_kontak = 1000;
    en_rst = true;
  }
}

void switch_kontak() {
  if (adc_kontak > 100) {
    prev_kontakoff = millis();
    en_rst = true;
  }

  if (adc_kontak > 100 and en_rst1 == true) {
    Serial.println();
    Serial.println("Restart Device off to on");
    Serial.println();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.clearDisplay();
    display.setCursor(10, 15);
    display.println(F("IGNITION"));
    display.setCursor(32, 40);
    display.println(F("ON"));
    display.display();
    refresh_sensor();
    delay(1500);
    display.clearDisplay();
    ESP.restart();
  }
  if (adc_kontak < 50 and en_rst == true) {
    if ((millis() - prev_kontakoff) > 5000) {
      en_rst1 = true;
      Serial.println();
      Serial.println("Restart Device on to off");
      Serial.println();
      display.setTextColor(WHITE);
      display.setTextSize(2);
      display.clearDisplay();
      display.setCursor(10, 15);
      display.println(F("IGNITION"));
      display.setCursor(32, 40);
      display.println(F("OFF"));
      display.display();
      refresh_sensor();
      delay(1500);
      display.clearDisplay();
      ESP.restart();
    }
  }
}

void main_loop() {
  read_pto();
  read_kontak();
  if (en_fms == "true") {
    loop_canbus();
  }
  else {
    no_fms();
  }

  if (en_fatiq == "true" and adc_kontak > 100) {
    programFatigue();
  }
  else {
    kondisiFatigue = "Disable";
    relay_buzzer = "OFF";
    count = 0;
    countFatigue = 0;

    if (digitalRead(FATIGUE_BTN) == HIGH) {
      con++;
      if (con > 30) {
        digitalWrite(buzzer, LOW);
      }
    }
    else {
      con = 0;
      programAlarm("OFF");
    }
  }

  if (en_overspeed == "true" and adc_kontak > 100) {
    loop_overspeed();
  }

  if (adc_kontak > 100) {
    ignition = "ON";
    if (incl_str != "Sensor_Failed" and incl_str != "") {
      formatDisplay(angVal, dispRotate);
      //  bacaTombol();
      if (SELECT_BTN) {
        delay(350);
        menuMainWait();
      }
    }
  }
  else {
    ignition = "OFF";
    memset(data, 0, sizeof(data));
    reset_data();
  }
}
