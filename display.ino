void setupOled() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    eror_lcd = true;
    Serial.println("LCD Failed");
  }
  else {
    eror_lcd = false;
    Serial.println("LCD Init Success");
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.println();
  display.display();
}

void disp_intro() {
  display_center(device_name, 2000);
  display.clearDisplay();
  display.setCursor(20, 9);
  display.print(F("CN UNIT"));
  display.drawFastHLine(0, 25, 128, SSD1306_WHITE);
  display.setCursor(5, 40);
  display.print(cn_str);
  display.display();
  Serial.println(cn_str);
  delay(2000);
  display_center(version, 2000);
  display.setTextColor(WHITE);
  display.setTextSize(2);
}

void display_center(String text, int jeda) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setFont(&FreeMono9pt7b);
  xPos = (SCREEN_WIDTH - getTextWidth(text)) / 2;
  yPos = (SCREEN_HEIGHT - getTextHeight()) / 2;
  display.setCursor(xPos, yPos);
  display.println(text);
  display.display();
  delay(jeda);
}

void stuck_detect() {
  if ((millis() - prev_stuck) > 500) {
    count_stuck++;
    prev_stuck = millis();
  }

  if (count_stuck > 1) {
    count_stuck = 0;
  }

  if (count_stuck == 1) {
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(2, 2);
    display.print(F("-"));
  }
  else {
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(2, 2);
    display.print(F(" "));
  }
}

void dispMenu(byte itemSelect) {
  display.clearDisplay();
  display.setRotation(0);
  //display.setFont(&FreeMono9pt7b);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(17, 8);
  display.print(F("Normal"));
  display.setCursor(14, 24);
  display.print(F("Presisi"));
  display.setCursor(8, 40);
  display.print(F("Kalibrasi"));
  display.setCursor(75, 8);
  display.print(F("Setting"));
  display.setCursor(75, 24);
  display.print(F("CN Unit"));

  if (itemSelect == 0) {
    display.drawRect(5, 5, 58, 14, SSD1306_WHITE);
  }
  if (itemSelect == 1) {
    display.drawRect(5, 21, 58, 14, SSD1306_WHITE);
  }
  if (itemSelect == 2) {
    display.drawRect(5, 37, 58, 14, SSD1306_WHITE);
  }
  if (itemSelect == 3) {
    display.drawRect(70, 5, 50, 30, SSD1306_WHITE);
  }
  display.display();
}

void dispCalibrate(byte itemSelect) {
  display.clearDisplay();
  display.setFont(&FreeMono9pt7b);
  display.setCursor(2, 14);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println(F("MENU"));
  display.println(F("KALIBRASI"));
  display.setCursor(0, 55);
  display.println(F(" START | X"));
  if (itemSelect == 0) {
    display.drawRect(6, 41, 65, 20, SSD1306_WHITE);
  } else {
    display.drawRect(95, 41, 20, 20, SSD1306_WHITE);
  }
  display.display();
}

void dispSetCN(String dataCN, bool finish) {
  display.clearDisplay();
  display.setFont(&FreeMono9pt7b);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(4, 10);
  display.print(F("MENU SET CN"));
  display.drawFastHLine(0, 16, 128, SSD1306_WHITE);
  display.setCursor(5, 40);
  display.print(dataCN);
  if (finish) {
    display.setFont();
    display.setCursor(14, 53);
    display.print("Change CN Success");
    display.drawRect(2, 50, 125, 13, SSD1306_WHITE);
  }
  display.display();
}

void gagal_config() {
  display.clearDisplay();
  display.setFont(&FreeMono9pt7b);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(2, 15);
  display.print(F("FAILED"));
  display.setCursor(2, 30);
  display.print("SETTING");
  display.setCursor(2, 45);
  display.print("Try Again");
  display.setCursor(2, 60);
  display.print("And Restart");
  display.display();
  delay(5000);
}

void sukses_SetCN(String dataCN) {
  display.clearDisplay();
  display.setFont(&FreeMono9pt7b);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(4, 10);
  display.print(F("Done Save"));
  display.drawFastHLine(0, 16, 128, SSD1306_WHITE);
  display.setCursor(5, 40);
  display.print(dataCN);
  display.setFont();
  display.setCursor(14, 53);
  display.print("Config Success");
  display.drawRect(2, 50, 125, 13, SSD1306_WHITE);
  display.display();
}

void menuMainWait() {
  menuItem = 0;
  dispMenu(menuItem);
  menuStartTime = millis();
  while (millis() - menuStartTime <= menuTimeout) {
    bacaTombol();
    if (SELECT_BTN) {
      delay(200);
      menuStartTime = millis();
      menuItem++;
      if (menuItem >= 4) {
        menuItem = 0;
        dispMenu(menuItem);
      } else {
        dispMenu(menuItem);
      }
      switch (menuItem) {
        case 0 :
          Serial.println("NORMAL");
          break;
        case 1 :
          Serial.println("PRESISI");
          break;
        case 2 :
          Serial.println("KALIBRASI");
          break;
        case 3 :
          Serial.println("SETTING CN UNIT");
      }
    }
    if (ENTER_BTN) {
      delay(200);
      menuStartTime = millis();
      if (menuItem == 0) {
        precisionMode = false;
        break;
      }
      if (menuItem == 1) {
        precisionMode = true;
        break;
      }
      if (menuItem == 2) {
        menuCalibrateWait();
        break;
      }
      if (menuItem == 3) {
        menuSetCN();
        break;
      }
    }
  }
}

void menuSetCN() {
  countCN = 0;
  cnNow = "";
  digit = 0;
  cnFix = "";
  dispSetCN(">", false);
  menuStartTime = millis();  // Reset menu start timer
  while (millis() - menuStartTime <= menuTimeout) {
    bacaTombol();
    if (UP_BTN) {
      delay(350);
      menuStartTime = millis();
      countCN++;
      if (countCN >= 18) countCN = 0;
      cnNow = cn[countCN];
      Serial.println(cnNow);
      dispSetCN(("> " + cnNow), false);
    }
    else if (DOWN_BTN) {
      delay(350);
      menuStartTime = millis();
      countCN--;
      if (countCN < 0) countCN = 18;
      cnNow = cn[countCN];
      Serial.println(cnNow);
      dispSetCN(("> " + cnNow), false);
    }
    else if (SELECT_BTN) {
      delay(350);
      menuStartTime = millis();
      digit++;
      if (digit > 8) digit = 0;
      cnFix = cnFix + cnNow;
      Serial.print("CN Sementara : ");
      Serial.println(cnFix);
      dispSetCN(cnFix, false);
    }
    else if (ENTER_BTN) {
      delay(350);
      menuStartTime = (millis() + 9000);
      lockCN = cnFix;
      Serial.println("Change CN Success");
      Serial.print("CN Unit : "); Serial.println(lockCN);
      dispSetCN(lockCN, true);
      TruckID = lockCN;
      writeFile(SPIFFS, "/cn_str.txt", lockCN.c_str());
      Serial2.println("{\"en_cn\":\"1\",\"cn\":\"" + lockCN + "\"}");
      Serial.println(lockCN);
    }
  }
  dispSetCN(lockCN, true);
  delay(2000);
}

void menuCalibrateWait() {
  menuItem = 1;
  dispCalibrate(menuItem);
  menuStartTime = millis();  // Reset menu start timer
  while (millis() - menuStartTime <= menuTimeout) {
    bacaTombol();
    if (SELECT_BTN) {
      delay(350);  // Debounce
      if (menuItem == 1) {
        menuItem = 0;
        Serial.println("START KALIBRASI");
        dispCalibrate(menuItem);
      } else {
        menuItem = 1;
        Serial.println("X KALIBRASI");
        dispCalibrate(menuItem);
      }
    }
    if (ENTER_BTN) {
      delay(350);  //Debounce
      if (menuItem == 0) {
        Serial.println("SEKARANG KALIBRASI...");
        display.clearDisplay();
        display.setCursor(0, 40);
        display.println(F("KALIBRASI.."));
        display.display();
        calibration_sensor();
        Serial.println("KALIBRASI SELESAI!!!");
        display.clearDisplay();
        display.setCursor(0, 40);
        display.println(F("SELESAI!!!"));
        display.display();
        delay(500);
        break;
      }
      if (menuItem == 1) {
        break;
      }
    }
  }
}

void formatDisplay(double angleVal, byte dispRotate) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setFont();
  display.setRotation(dispRotate);
  stuck_detect();
  if (precisionMode == false) {
    if (dispRotate == 0 || dispRotate == 2) {
      // Horizontal
      display.setCursor(25, 10);
    } else {
      display.setCursor(3, 30);
    }
    display.setTextSize(5);
    //display.println(round(abs(angleVal)));
    display.print(int(angleVal));
    int16_t x = display.getCursorX();
    int16_t y = display.getCursorY();
    int8_t r = 5;

    x += 4;
    y -= 2;

    display.fillCircle(x, y, r, SSD1306_WHITE);
  }
  else {
    if (dispRotate == 0 || dispRotate == 2) {
      display.setTextSize(4);
      display.setCursor(20, 10);
    } else {
      display.setTextSize(2);
      display.setCursor(6, 40);
    }
    display.println(abs(angleVal), 1);
  }
  if (round(abs(angleVal)) <= 1) {
    display.setTextSize(1);
    display.setTextColor(BLACK, WHITE);
    if (dispRotate == 0 || dispRotate == 2) {
      display.setCursor(37, 50);
      display.println(F("   LEVEL  "));
    } else {
      display.setCursor(0, 90);
      display.println(F("   PLUMB  "));
    }
  }
  display.setTextColor(WHITE, BLACK);
  display.display();
}

void disp_senfail() {
  if (adc_kontak > 100) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setFont(&FreeMono9pt7b);
    display.clearDisplay();
    display.setCursor(32, 15);
    display.println("SENSOR");
    display.setCursor(32, 40);
    display.println("FAILED");
    display.display();
  }
}

void waiting_sensor() {
  if (adc_kontak > 100) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setFont(&FreeMono9pt7b);
    display.clearDisplay();
    display.setCursor(32, 15);
    display.println("Waiting");
    display.setCursor(32, 40);
    display.println("Sensor");
    display.display();
  }
}

int getTextWidth(String text) {
  int16_t x, y;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x, &y, &w, &h);
  return w;
}

int getTextHeight() {
  int16_t x, y;
  uint16_t w, h;
  display.getTextBounds("A", 0, 0, &x, &y, &w, &h); // Menggunakan huruf 'A' untuk mendapatkan tinggi teks
  return h;
}

void drawLargeDegreeSymbol(int x, int y) {
  // Baris pertama
  display.drawPixel(x + 3, y, SSD1306_WHITE);
  display.drawPixel(x + 4, y, SSD1306_WHITE);
  display.drawPixel(x + 5, y, SSD1306_WHITE);

  // Baris kedua
  display.drawPixel(x + 2, y + 1, SSD1306_WHITE);
  display.drawPixel(x + 6, y + 1, SSD1306_WHITE);

  // Baris ketiga
  display.drawPixel(x + 1, y + 2, SSD1306_WHITE);
  display.drawPixel(x + 7, y + 2, SSD1306_WHITE);

  // Baris keempat
  display.drawPixel(x, y + 3, SSD1306_WHITE);
  display.drawPixel(x + 8, y + 3, SSD1306_WHITE);

  // Baris kelima
  display.drawPixel(x, y + 4, SSD1306_WHITE);
  display.drawPixel(x + 8, y + 4, SSD1306_WHITE);

  // Baris keenam
  display.drawPixel(x, y + 5, SSD1306_WHITE);
  display.drawPixel(x + 8, y + 5, SSD1306_WHITE);

  // Baris ketujuh
  display.drawPixel(x + 1, y + 6, SSD1306_WHITE);
  display.drawPixel(x + 7, y + 6, SSD1306_WHITE);

  // Baris kedelapan
  display.drawPixel(x + 2, y + 7, SSD1306_WHITE);
  display.drawPixel(x + 6, y + 7, SSD1306_WHITE);

  // Baris kesembilan
  display.drawPixel(x + 3, y + 8, SSD1306_WHITE);
  display.drawPixel(x + 4, y + 8, SSD1306_WHITE);
  display.drawPixel(x + 5, y + 8, SSD1306_WHITE);
}
