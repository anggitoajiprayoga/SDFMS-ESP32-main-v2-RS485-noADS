void serial_monitor() {
  unsigned long currentMillis = millis();
  if ((unsigned long)(currentMillis - previousMillisL) >= intervalL) {
    previousMillisL = millis();
    Serial.println();
    Serial.println("CN : " + cn_str);
    Serial.println("brand : " + brand);
    Serial.println("Latitude : " + lat_str);
    Serial.println("Longitude : " + lng_str);
    Serial.println("Altitude : " + alt_str + " mdpl");
    Serial.println("Speed : " + spd_str + " kmph");
    Serial.println("Heading : " + heading_str);
    Serial.println("ST : " + st_str);
    Serial.println("Kemiringan : " + incl_str + "°");
    Serial.println("angVal : " + String(angVal) + "°");
    Serial.println("Date_gps : " + tanggalPost);
    Serial.println("Time_gps : " + times);
    Serial.println("date_time RTC : " + date_time);
    Serial.println("Fatigue : " + kondisiFatigue);
    Serial.println("value_pto: " + String(pto) + " | adc_pto: " + String(adc_pto));
    Serial.println("en_fatiq: " + en_fatiq + " | en_fms: " + en_fms + " | en_overspeed: " + en_overspeed + " | limit_speed: " + limit_speed);
    Serial.println("button_fatigue: " + String(digitalRead(FATIGUE_BTN)));
    Serial.print(F("Fatigue duration: "));
    Serial.println(countFatigue);
    Serial.println("fatique count: " + String(count));
    Serial.println("adc_kontak: " + String(adc_kontak));
    Serial.println("paket: " + input);

    Serial.println();
    Serial.println("================ eror send ================");
    String state_eror = eror_kode();
    Serial.println(state_eror);
    Serial2.println(state_eror);
    Serial.println("================ end ================");
    Serial.println();
    delay(500);
    Serial.println();
    Serial.println("================ payload send ================");
    Serial.println(payload);
    Serial2.println(payload);
    Serial.println("================ end ================");
    Serial.println();
  }
}

void debug_scania() {
  if (millis() - lastMillisDebug >= 3000) {
    lastMillisDebug = millis();
    Serial.println();
    Serial.println("### Data [CAN-BUS] [" + brand_str + "] " + bitrate_str);
    Serial.println("Date_unit: " + date_str);
    Serial.println("Time_unit: " + time_str);
    Serial.println("Engine Torque: " + engTorque_str + "%");
    Serial.println("Engine Speed: " + engSpeed_str + " RPM");
    Serial.println("Tachogr. Vehicle Speed: " + tacVehSpeed_str + "km/h");
    Serial.println("Accelerator Pedal Position: " + accPed_str);
    Serial.println("Engine Load At Current Speed: " + engLoad_str + "%");
    Serial.println("Actual Retarder: " + actRet_str + "%");
    Serial.println("Brake Pedal Position: " + brakePedPos_str);
    Serial.println("Service Brake Air Pressure: ");
    Serial.println("  - Air Pressure Brake 1: " + airPress1_str + " Bar");
    Serial.println("  - Air Pressure Brake 2: " + airPress2_str + " Bar");
    Serial.println("  - Air Pressure Brake 3: " + airPress3_str + " Bar");
    Serial.println("Fuel Rate: " + fuelRate_str + " L/h");
    Serial.println("Engine Hours Meter: " + engHm_str);
    Serial.println("Coolant Temperature: " + engClnTemp_str + "°C");
    Serial.println("Fuel Level: " + fuelLvl_str + "%");
    Serial.println("Vehicle Speed: " + vehSpeed_str + "km/h");
    Serial.println("---------- ---------- ---------- ---------- ----------");
  }
}

void debug_mercy() {
  if (millis() - lastMillisDebug >= 3000) {
    lastMillisDebug = millis();
    Serial.println("### Data [CAN-BUS] [" + brand_str + "] " + bitrate_str);
    Serial.println("Engine Torque: " + engTorque_str + "%");
    Serial.println("Engine Speed: " + engSpeed_str + " RPM");
    Serial.println("Tachogr. Vehicle Speed: " + tacVehSpeed_str + "km/h");
    Serial.println("Accelerator Pedal Position: " + accPed_str);
    Serial.println("Engine Load At Current Speed: " + engLoad_str + "%");
    Serial.println("Actual Retarder: " + actRet_str + "%");
    Serial.println("Brake Pedal Position: " + brakePedPos_str);
    Serial.println("Service Brake Air Pressure: ");
    Serial.println("  - Air Pressure Brake 1: " + airPress1_str + " Bar");
    Serial.println("  - Air Pressure Brake 2: " + airPress2_str + " Bar");
    Serial.println("Fuel Rate: " + fuelRate_str + " L/h");
    Serial.println("Engine Hours Meter: " + engHm_str);
    Serial.println("Coolant Temperature: " + engClnTemp_str + "°C");
    Serial.println("Fuel Level: " + fuelLvl_str + "%");
    Serial.println("Vehicle Speed: " + vehSpeed_str + "km/h");
    Serial.println("---------- ---------- ---------- ---------- ----------");
  }
}

void debug_volvo() {
  if (millis() - lastMillisDebug >= 3000) {
    lastMillisDebug = millis();
    Serial.println();
    Serial.println("### Data [CAN-BUS] [" + brand_str + "] " + bitrate_str);
    Serial.println("Engine Torque: " + engTorque_str + "%");
    Serial.println("Engine Speed: " + engSpeed_str + " RPM");
    Serial.println("Tachogr. Vehicle Speed: " + tacVehSpeed_str + "km/h");
    Serial.println("Accelerator Pedal Position: " + accPed_str);
    Serial.println("Engine Load At Current Speed: " + engLoad_str + "%");
    Serial.println("Actual Retarder: " + actRet_str + "%");
    Serial.println("Brake Pedal Position: " + brakePedPos_str);
    Serial.println("Service Brake Air Pressure: ");
    Serial.println("  - Air Pressure Brake 1: " + airPress1_str + " Bar");
    Serial.println("  - Air Pressure Brake 2: " + airPress2_str + " Bar");
    Serial.println("Fuel Rate: " + fuelRate_str + " L/h");
    Serial.println("Engine Revolution: " + engEr_str);
    Serial.println("Engine Hours Meter: " + engHm_str);
    Serial.println("Coolant Temperature: " + engClnTemp_str + "°C");
    Serial.println("Fuel Level: " + fuelLvl_str + "%");
    Serial.println("Vehicle Speed: " + vehSpeed_str + "km/h");
    Serial.println("---------- ---------- ---------- ---------- ----------");
  }
}
