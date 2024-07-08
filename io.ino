void setupIo() {
  pinMode(in_pto, INPUT);
  pinMode(kontak_on, INPUT);
  pinMode(tombolADC, INPUT);
  pinMode(FATIGUE_BTN, INPUT_PULLUP);
  //pinMode(RE1, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(selenoid, OUTPUT);

  // digitalWrite(RE1, LOW);
  digitalWrite(buzzer, LOW);
  digitalWrite(selenoid, LOW);
  delay(200);
  //digitalWrite(RE1, HIGH);
  digitalWrite(buzzer, HIGH);
  digitalWrite(selenoid, HIGH);
  delay(200);

//  if (!ads.begin(ads_address)) {
//    Serial.println("Failed to initialize ADS.");
//  }
}

void bacaTombol() {
  int adc  = analogRead(tombolADC);
  //Serial.println(adc);
  delay(50);
  if (adc >= 150 && adc < 350) {
    ENTER_BTN = true;
    Serial.println("ENTER_BTN");
  }
  else if (adc >= 350 && adc < 550) {
    SELECT_BTN = true;
    Serial.println("SELECT_BTN");
  }
  else if (adc >= 750 && adc < 980) {
    DOWN_BTN = true;
    Serial.println("DOWN_BTN");
  }
  else if (adc >= 980) {
    UP_BTN = true;
    Serial.println("UP_BTN");
  }
  else {
    ENTER_BTN = false;
    SELECT_BTN = false;
    UP_BTN = false;
    DOWN_BTN = false;
  }
}

void read_pto() {
  adc_pto = analogRead(in_pto);
//  adc1 = ads.readADC_SingleEnded(1);
//  adc_pto = adc1;

  if (adc1 > 250) {
    pto = 1;
  }
  else {
    pto = 0;
  }

  if (pto == 1 and notif_pto == false) {
    if (adc_kontak > 100) {
      notif_pto = true;
      display.setTextColor(WHITE);
      display.setTextSize(2);
      display.clearDisplay();
      display.setCursor(32, 15);
      display.println(F("PTO"));
      display.setCursor(32, 40);
      display.println(F("ON"));
      display.display();
      delay(1500);
    }
  }

  else if (pto == 0 and notif_pto == true) {
    if (adc_kontak > 100) {
      notif_pto = false;
      display.setTextColor(WHITE);
      display.setTextSize(2);
      display.clearDisplay();
      display.setCursor(32, 15);
      display.println(F("PTO"));
      display.setCursor(32, 40);
      display.println(F("OFF"));
      display.display();
      delay(1500);
    }
  }
}

void read_kontak() {
//  adc2 = ads.readADC_SingleEnded(3);
    adc_kontak = analogRead(kontak_on);
//  adc_kontak = adc2;

  if (adc_kontak < 0) {
    adc_kontak = 0;
  }
}
