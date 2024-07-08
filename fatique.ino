void setup_fatiq() {

}

void programFatigue() {
  if (digitalRead(FATIGUE_BTN) == HIGH) {
    counter = 1;
    unsigned long currentMillis = millis();
    if ((unsigned long)(currentMillis - previousMillisCount) >= IntervalCount) {
      Serial.println(F("Always Push Button"));
      logic = false;
      previousMillisCount = millis();
    }
  }
  else if (digitalRead(FATIGUE_BTN) == LOW) {
    logic = true;
    counter = 0;
  }

  if (counter == 0) {
    countUp(1000);
  }
  else if (counter == 1 && (logic == true || logic == false)) {
    count = 0;
    countFatigue = 0;
  }

  if (count > timeLimit) {
    programAlarm("ON");
    countingFatigue(1000);
  } else if (count <= timeLimit && logic == true) {
    programAlarm("OFF");
  } else if (count <= timeLimit && logic == false) {
    programAlarm("ON");
  }

  if (countFatigue > timeFatigue) {
    fatigue = true;
    kondisiFatigue = "driver_FATIGUE";
  } else if (countFatigue <= timeFatigue) {
    fatigue = false;
    kondisiFatigue = "Driver_not_fatigue";
  }
}

void countingFatigue(int Interval) {
  if ((unsigned long)(millis() - previousMillisFatigue) >= Interval) {
    previousMillisFatigue = millis();
    countFatigue++;
  }
}

void countUp(int Interval) {
  unsigned long currentMillis = millis();
  if ((unsigned long)(currentMillis - previousMillisCount) >= Interval) {
    previousMillisCount = millis();
    count++;
    //    Serial.print(F("Count : "));
    //    Serial.println(count);
  }
}

void programAlarm(String kondisi) {
  if (kondisi == "ON") {
    relay_buzzer = "ON";
    digitalWrite(buzzer, LOW);
  }
  if (kondisi == "OFF") {
    relay_buzzer = "OFF";
    digitalWrite(buzzer, HIGH);
  }
}

void danger_alarm(int timex) {
  interval_alarm = timex;
  alarmActive = true;
  previousMillis_alram = millis();
  buzzerState = true; // Mulai dengan buzzer menyala
  digitalWrite(buzzer, LOW); // Nyalakan buzzer
}

void stop_alarm() {
  alarmActive = false;
  digitalWrite(buzzer, HIGH); // Matikan buzzer
}

void updateAlarm() {
  if (alarmActive) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis_alram >= interval_alarm) {
      previousMillis_alram = currentMillis;
      buzzerState = !buzzerState;
      digitalWrite(buzzer, buzzerState ? LOW : HIGH);
    }
  }
}
