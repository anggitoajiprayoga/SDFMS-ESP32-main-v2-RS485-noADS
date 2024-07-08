void setup_rs485() {
  rs485.begin(9600);
  pinMode(de_re, OUTPUT);
  digitalWrite(de_re, LOW);
}

void loop_rs485() {
  digitalWrite(de_re, LOW);
  if (rs485.available()) {
    eror_rs485 = false;
    String gyro = rs485.readStringUntil('\n');
    gyro.trim();
    incl_str = gyro;
    //    Serial.println(gyro);
    prev_rs485 = millis();
  }
  else {
    if ((millis() - prev_rs485) > 3000) {
      gyro = "";
      incl_str = "";
      eror_rs485 = true;
    }
  }

  if (number_detect(incl_str)) {
    angVal = incl_str.toFloat();
    angVal = constrain(angVal, -99, 99);
    handle_angle();
    prev_fail = millis();
  }
  else if (incl_str == "") {
    if (state_calibration == false) {
      handle_empty_string();
    }
  }
  else if (incl_str == "Sensor_Failed") {
    handle_sensor_failed();
  }
}

void handle_angle() {
  if (adc_pto > 250) {
    if (angVal > -4 && angVal < 4) {
      digitalWrite(selenoid, LOW);
      relay_dump = "ON";
      dump_status = "Ready";
    }
    else {
      digitalWrite(selenoid, HIGH);
      relay_dump = "OFF";
      dump_status = "Not_Ready";
    }

    if (en_anglewarning == "true") {
      if (angVal <= String("-" + limit_angle).toInt() or angVal >= limit_angle.toInt()) {
        danger_alarm(500);
      }
      else {
        stop_alarm();
      }
    }
  }
  else {
    stop_alarm();
    digitalWrite(selenoid, HIGH);
    relay_dump = "OFF";
    dump_status = "Not_Ready";
  }
}

void handle_empty_string() {
  if ((millis() - prev_fail) > 30000) {
    disp_senfail();
    digitalWrite(selenoid, HIGH);
    incl_str = "Sensor_Failed";
    relay_dump = "OFF";
    dump_status = "Not_Ready";
  }
  else {
    waiting_sensor();
  }
}

void handle_sensor_failed() {
  disp_senfail();
  if ((millis() - prev_rst) > 1000) {
    count_rst++;
    prev_rst = millis();
  }

  if (count_rst > 120) {
    refresh_sensor();
    count_rst = 0;
  }
}

void calibration_sensor() {
  digitalWrite(de_re, HIGH);
  rs485.println("A");
  delay(150);
  rs485.println("A");
  delay(150);
  rs485.println("A");
  delay(150);
  rs485.println("A");
  delay(150);
  rs485.println("A");;
  Serial.println("calibration sensor trigger");
  delay(50);
  digitalWrite(de_re, LOW);
}

void refresh_sensor() {
  digitalWrite(de_re, HIGH);
  rs485.println("B");
  delay(150);
  rs485.println("B");
  delay(150);
  rs485.println("B");
  delay(150);
  rs485.println("B");
  delay(150);
  rs485.println("B");
  Serial.println("Automatic refresh sensor trigger");
  delay(50);
  digitalWrite(de_re, LOW);
}

bool number_detect(const String& str) {
  if (str.length() == 0) {
    return false;
  }

  bool hasDecimal = false;
  for (unsigned int i = 0; i < str.length(); i++) {
    char currentChar = str.charAt(i);

    if (i == 0 && (currentChar == '+' || currentChar == '-')) {
      continue;
    }

    if (currentChar == '.') {
      if (hasDecimal) {
        return false;
      }
      hasDecimal = true;
      continue;
    }

    if (!isdigit(currentChar)) {
      return false;
    }
  }
  return true;
}
