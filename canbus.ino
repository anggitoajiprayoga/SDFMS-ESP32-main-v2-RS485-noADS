void setup_canbus() {
  SPI.begin();
  mcp2515.reset();
  if (set_bitrate == "125") {
    mcp2515.setBitrate(CAN_125KBPS);
  }
  else {
    mcp2515.setBitrate(CAN_250KBPS);
  }

  mcp2515.setNormalMode();

  Serial.println("---------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
  Serial.println("set_bitrate: " + set_bitrate);
  Serial.println("brand: " + brand);
}

void loop_canbus() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    eror_canbus = false;
    pngID = canMsg.can_id;  // ID
    //    Serial.print(pngID, HEX);
    //    Serial.print("  ");
    // Reduce ID To 4 digits (HEX xxxx)
    pngID = pngID << 8;
    pngID = pngID >> 16;
    pgnID = pngID;

    int dataLen = canMsg.can_dlc;
    for (int i = 0; i < canMsg.can_dlc; i++) {
      data[i] = canMsg.data[i];
      //      Serial.print(data[i], HEX);
    }
    //    Serial.println();
    prev_can = millis();
  }
  else {
    if ((millis() - prev_can) > 10000) {
      eror_canbus = true;
      memset(data, 0, sizeof(data));
      reset_data();
      prev_can = millis();
    }
  }

  brand = "scania";
  brand_str = brand;
  if (brand == "mercy") {
    canbus_mercy();
    debug_mercy();
  }
  else if (brand == "volvo") {
    canbus_volvo();
    debug_volvo();
  }
  else if (brand == "scania") {
    canbus_scania();
    debug_scania();
  }
  else {
    no_fms();
  }
}

void print_hex() {
  for (int i = 0; i < 8; i++) {
    Serial.print(data[0], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void canbus_volvo() {
  if (pgnID == 0xF004) {
    /* ### Torque & RPM ### */
    unsigned int d1 = data[2] - 125;
    float power = d1;
    unsigned int r1 = data[3];
    unsigned int r2 = data[4];
    float rpm = (r1 / 8) + ((r2 * 256) / 8);

    engTorque_str = String(power);
    engSpeed_str = String(rpm);
  }

  if (pgnID == 0xFE6C) {
    /* ### Tachogr. Vehicle Speed ### */
    float speed1 = data[6];
    float speed2 = data[7];
    float total = (speed1 / 256) + ((speed2 * 256) / 256);
    tacVehSpeed_str = String(total);
  }

  if (pgnID == 0xF003) {
    /* ### Accelerator Pedal Position 1 ### */
    float accPed = data[1] / 2.5;
    float engLoad = data[2];
    accPed_str = String(accPed);
    engLoad_str = String(engLoad);
  }

  if (pgnID == 0xF000) {
    /* ### Actual Retarder ### */
    float actRet = data[1] - 125;
    actRet_str = String(abs(actRet));
  }

  if (pgnID == 0xF001) {
    /* ### Brake Pedal Position ### */
    float brakePedPos = data[1] / 2.5;
    brakePedPos_str = String(brakePedPos);
  }

  if (pgnID == 0xFEAE) {
    /* ### Air Pressure ### */
    float air3 = data[1];
    float air1 = data[2];
    float air2 = data[3];
    airPress3_str = String(air3 * 0.078125);
    airPress1_str = String(air1 * 0.078125);
    airPress2_str = String(air2 * 0.078125);
  }

  if (pgnID == 0xFEF2) {
    /* ### Fuel Rate ### */
    float fuel1 = data[0];
    float fuel2 = data[1];
    float total = (fuel1 / 20) + ((fuel2 * 256) / 20);
    fuelRate_str = String(total);
  }

  if (pgnID == 0xFEE5) {
    /* ### Engine Revolution ### */
    float er1 = data[4];
    float er2 = data[5];
    float er3 = data[6];
    float er4 = data[7];
    float total = er1 / 0.001 + (er2 * 256) / 0.001 + (er3 * 65536) / 0.001 + (er4 * 16777216) / 0.001;
    engEr_str = String(total);
  }

  if (pgnID == 0xFEEE) {
    /* ### Coolant Temperature ### */
    int engCoolantTemp = data[0] - 40;
    engClnTemp_str = String(engCoolantTemp);
  }

  if (pgnID == 0xFEFC) {
    /* ### Fuel Level ### */
    float fuel = data[1] / 2.5;
    fuelLvl_str = String(fuel);
  }

  if (pgnID == 0xFEF1) {
    /* ### Vehicle Speed ### */
    float speed1 = data[1];
    float speed2 = data[2];
    float total = (speed1 / 256) + ((speed2 * 256) / 256);
    vehSpeed_str = String(total);
  }

  if (pgnID == 0xFEE5) {
    /* ### Engine Hours Meter ### */
    float hm1 = data[0];
    float hm2 = data[1];
    float hm3 = data[2];
    float hm4 = data[3];
    float total = hm1 / 20 + (hm2 * 256) / 20 + (hm3 * 65536) / 20 + (hm4 * 16777216) / 20;
    String xengHm_str = String(total - 1);
    if (xengHm_str != "") {
      engHm_str = xengHm_str;
      writeFile(SPIFFS, "/engHm_str.txt", xengHm_str.c_str());
    }
  }

  //  payload = "{\"en_cn\":\"0\",\"cn\":\"" + TruckID + "\",\"brand\":\"" + brand_str + "\",\"date_gps\":\"" + tanggalPost + "\",\"time_gps\":\"" + times + "\",\"lat\":\"" + lat_str + "\",\"lng\":\"" + lng_str + "\",\"alt\":\"" + alt_str + "\",\"spd\":\"" + spd_str + "\",\"heading\":\"" + heading_str + "\",\"agl\":\"" + incl_str + "\",\"st\":\"" + st_str + "\",\"fatigue\":\"" + kondisiFatigue +
  //            "\",\"engTorque\":\"" + engTorque_str + "\",\"engSpeed\":\"" + engSpeed_str + "\",\"tacVehSpeed\":\"" + tacVehSpeed_str + "\",\"accPed\":\"" + accPed_str + "\",\"engLoad\":\"" + engLoad_str + "\",\"actRet\":\"" + actRet_str + "\",\"brakePedPos\":\"" + brakePedPos_str + "\",\"airPress1\":\"" + airPress1_str + "\",\"airPress2\":\"" + airPress2_str + "\",\"fuelRate\":\"" +
  //            fuelRate_str + "\",\"engHm\":\"" + engHm_str + "\",\"engEr\":\"" + engEr_str + "\",\"engClnTemp\":\"" + engClnTemp_str + "\",\"fuelLvl\":\"" + fuelLvl_str + "\",\"vehSpeed\":\"" + vehSpeed_str + "\",\"adc_pto\":\"" + String(String(analogRead(35)) + "(" + String(pto) + ")") + "\",\"relay_buzzer\":\"" + relay_buzzer + "\",\"relay_dump\":\"" + relay_dump + "\",\"dump_status\":\"" + dump_status + "\"}";

  StaticJsonDocument<1024> buffer;
  JsonObject obj = buffer.to<JsonObject>();

  payload = "";
  obj["en_cn"] = "0";
  obj["cn"] = cn_str;
  obj["brand"] = brand;
  obj["date_gps"] = tanggalPost;
  obj["time_gps"] = times;
  obj["lat"] = lat_str;
  obj["lng"] = lng_str;
  obj["alt"] = alt_str;
  obj["spd"] = spd_str;
  obj["heading"] = heading_str;
  obj["agl"] = incl_str;
  obj["st"] = st_str;
  obj["fatigue"] = kondisiFatigue;
  obj["engHm"] = engHm_str;
  obj["vehSpeed"] = vehSpeed_str;
  obj["engSpeed"] = engSpeed_str;
  obj["engTorque"] = engTorque_str;
  obj["tacVehSpeed"] = tacVehSpeed_str;
  obj["accPed"] = accPed_str;
  obj["engLoad"] = engLoad_str;
  obj["actRet"] = actRet_str;
  obj["brakePedPos"] = brakePedPos_str;
  obj["airPress1"] = airPress1_str;
  obj["airPress2"] = airPress2_str;
  obj["fuelRate"] = fuelRate_str;
  obj["engEr"] = engEr_str;
  obj["engClnTemp"] = engClnTemp_str;
  obj["fuelLvl"] = fuelLvl_str;
  obj["adc_pto"] = String(pto);
  obj["relay_buzzer"] = relay_buzzer;
  obj["relay_dump"] = relay_dump;
  obj["dump_status"] = dump_status;
  obj["ignition"] = ignition;

  serializeJson(obj, payload);
}

void canbus_mercy() {
  if (pgnID == 0xF004) {
    /* ### Torque & RPM ### */
    unsigned int d1 = data[2] - 125;
    float power = d1;
    unsigned int r1 = data[3];
    unsigned int r2 = data[4];
    float rpm = (r1 / 8) + ((r2 * 256) / 8);

    engTorque_str = String(power);
    engSpeed_str = String(rpm);
  }

  if (pgnID == 0xFE6C) {
    /* ### Tachogr. Vehicle Speed ### */
    float speed1 = data[6];
    float speed2 = data[7];
    float total = (speed1 / 256) + ((speed2 * 256) / 256);
    tacVehSpeed_str = String(total);
  }

  if (pgnID == 0xF003) {
    /* ### Accelerator Pedal Position 1 ### */
    float accPed = data[1] / 2.5;
    float engLoad = data[2];
    accPed_str = String(accPed);
    engLoad_str = String(engLoad);
  }

  if (pgnID == 0xF000) {
    /* ### Actual Retarder ### */
    float actRet = data[1] - 125;
    actRet_str = String(abs(actRet));
  }

  if (pgnID == 0xF001) {
    /* ### Brake Pedal Position ### */
    float brakePedPos = data[1] / 2.5;
    brakePedPos_str = String(brakePedPos);
  }

  if (pgnID == 0xFEAE) {
    /* ### Air Pressure ### */
    float air3 = data[1];
    float air1 = data[2];
    float air2 = data[3];
    airPress3_str = String(air3 * 0.078125);
    airPress1_str = String(air1 * 0.078125);
    airPress2_str = String(air2 * 0.078125);
  }

  if (pgnID == 0xFEF2) {
    /* ### Fuel Rate ### */
    float fuel1 = data[0];
    float fuel2 = data[1];
    float total = (fuel1 / 20) + ((fuel2 * 256) / 20);
    fuelRate_str = String(total);
  }

  if (pgnID == 0xFEE5) {
    /* ### Engine Hours Meter ### */
    float hm1 = data[0];
    float hm2 = data[1];
    float hm3 = data[2];
    float hm4 = data[3];
    float total = hm1 / 20 + (hm2 * 256) / 20 + (hm3 * 65536) / 20 + (hm4 * 16777216) / 20;
    String xengHm_str = String(total - 1);
    if (xengHm_str != "") {
      engHm_str = xengHm_str;
      writeFile(SPIFFS, "/engHm_str.txt", xengHm_str.c_str());
    }
  }

  if (pgnID == 0xFEEE) {
    /* ### Coolant Temperature ### */
    int engCoolantTemp = data[0] - 40;
    engClnTemp_str = String(engCoolantTemp);
  }

  if (pgnID == 0xFEFC) {
    /* ### Fuel Level ### */
    float fuel = data[1] / 2.5;
    fuelLvl_str = String(fuel);
  }

  if (pgnID == 0xFEF1) {
    /* ### Vehicle Speed ### */
    float speed1 = data[1];
    float speed2 = data[2];
    float total = (speed1 / 256) + ((speed2 * 256) / 256);
    vehSpeed_str = String(total);
  }

  //  payload = "{\"en_cn\":\"0\",\"cn\":\"" + TruckID + "\",\"brand\":\"" + brand_str + "\",\"date_gps\":\"" + tanggalPost + "\",\"time_gps\":\"" + times + "\",\"lat\":\"" + lat_str + "\",\"lng\":\"" + lng_str + "\",\"alt\":\"" + alt_str + "\",\"spd\":\"" + spd_str + "\",\"heading\":\"" + heading_str + "\",\"agl\":\"" + incl_str + "\",\"st\":\"" + st_str + "\",\"fatigue\":\"" + kondisiFatigue +
  //            "\",\"engTorque\":\"" + engTorque_str + "\",\"engSpeed\":\"" + engSpeed_str + "\",\"tacVehSpeed\":\"" + tacVehSpeed_str + "\",\"accPed\":\"" + accPed_str + "\",\"engLoad\":\"" + engLoad_str + "\",\"actRet\":\"" + actRet_str + "\",\"brakePedPos\":\"" + brakePedPos_str + "\",\"airPress1\":\"" + airPress1_str + "\",\"airPress2\":\"" + airPress2_str + "\",\"fuelRate\":\"" + fuelRate_str +
  //            "\",\"engHm\":\"" + engHm_str + "\",\"engClnTemp\":\"" + engClnTemp_str + "\",\"fuelLvl\":\"" + fuelLvl_str + "\",\"vehSpeed\":\"" + vehSpeed_str + "\",\"adc_pto\":\"" + String(String(analogRead(35)) + "(" + String(pto) + ")") + "\",\"relay_buzzer\":\"" + relay_buzzer + "\",\"relay_dump\":\"" + relay_dump + "\",\"dump_status\":\"" + dump_status + "\"}";

  StaticJsonDocument<1024> buffer;
  JsonObject obj = buffer.to<JsonObject>();

  //  payload = "";
  //  obj["en_cn"] = "0";
  //  obj["cn"] = "DA1234";
  //  obj["brand"] = "mercy";
  //  obj["date_gps"] = "2024-03-04";
  //  obj["time_gps"] = "20:34:26";
  //  obj["lat"] = "30403.400391";
  //  obj["lng"] = "0.000000";
  //  obj["alt"] = "68.00";
  //  obj["spd"] = "0.00";
  //  obj["heading"] = "0";
  //  obj["agl"] = "-66.79";
  //  obj["st"] = "12";
  //  obj["engHm"] = "12345.6";
  //  obj["vehSpeed"] = "50.7";
  //  obj["engSpeed"] = "1234";
  //  obj["fatigue"] = "Driver not fatigue";
  //  obj["engTorque"] = "123";
  //  obj["tacVehSpeed"] = "12.3";
  //  obj["accPed"] = "100";
  //  obj["engLoad"] = "1234";
  //  obj["actRet"] = "112";
  //  obj["brakePedPos"] = "123";
  //  obj["airPress1"] = "12.4";
  //  obj["airPress2"] = "1234";
  //  obj["fuelRate"] = "100";
  //  obj["engClnTemp"] = "1233";
  //  obj["fuelLvl"] = "100";
  //  obj["adc_pto"] = String(pto);
  //  obj["relay_buzzer"] = "OFF";
  //  obj["relay_dump"] = "OFF";
  //  obj["dump_status"] = "not ready";

  payload = "";
  obj["en_cn"] = "0";
  obj["cn"] = cn_str;
  obj["brand"] = brand;
  obj["date_gps"] = tanggalPost;
  obj["time_gps"] = times;
  obj["lat"] = lat_str;
  obj["lng"] = lng_str;
  obj["alt"] = alt_str;
  obj["spd"] = spd_str;
  obj["heading"] = heading_str;
  obj["agl"] = incl_str;
  obj["st"] = st_str;
  obj["engHm"] = engHm_str;
  obj["vehSpeed"] = vehSpeed_str;
  obj["engSpeed"] = engSpeed_str;
  obj["fatigue"] = kondisiFatigue;
  obj["engTorque"] = engTorque_str;
  obj["tacVehSpeed"] = tacVehSpeed_str;
  obj["accPed"] = accPed_str;
  obj["engLoad"] = engLoad_str;
  obj["actRet"] = actRet_str;
  obj["brakePedPos"] = brakePedPos_str;
  obj["airPress1"] = airPress1_str;
  obj["airPress2"] = airPress2_str;
  obj["fuelRate"] = fuelRate_str;
  obj["engClnTemp"] = engClnTemp_str;
  obj["fuelLvl"] = fuelLvl_str;
  obj["adc_pto"] = String(pto);
  obj["relay_buzzer"] = relay_buzzer;
  obj["relay_dump"] = relay_dump;
  obj["dump_status"] = dump_status;
  obj["ignition"] = ignition;

  serializeJson(obj, payload);
}

void canbus_scania() {
  if (pgnID == 0xFEE6) {
    /* ### Date/Time ECU ### */
    unsigned int offset_mnt = data[6] - 68;
    unsigned int offset_jam = data[7] - 126;
    unsigned int Second = data[0] / 4;
    unsigned int Minute = data[1] + offset_mnt;
    unsigned int Hour = data[2] + offset_jam;
    unsigned int Month = data[3];
    unsigned int Days = data[4] / 4;
    unsigned int Year = data[5] + 1985;
    Serial.println("offset_mnt: " + String(offset_mnt));
    Serial.println("offset_jam: " + String(offset_jam));

    //Add leading 0's to date month and time and build up a time/date string
    date_str = "";
    if (Days < 10)
      date_str = "0";
    date_str += String(Days) + "/";
    if (Month < 10)
      date_str = date_str + "0";
    date_str += String(Month) + "/" + String(Year);

    time_str = "";
    if (Hour < 10)
      time_str += "0";
    time_str += String(Hour) + ":";
    if (Minute < 10)
      time_str += "0";
    time_str += String(Minute) + ":";
    if (Second < 10)
      time_str += "0";
    time_str += String(Second);
    engSpeed_str = String(Second);
  }

  if (pgnID == 0xF004) {
    /* ### Torque & RPM ### */
    unsigned int d1 = data[2] - 125;
    float power = d1;
    unsigned int r1 = data[3];
    unsigned int r2 = data[4];
    float rpm = (r1 / 8) + ((r2 * 256) / 8);

    engTorque_str = String(power);
    engSpeed_str = String(rpm);
  }

  if (pgnID == 0xFF35) {
    unsigned int state = data[1];
    Serial.println("state_pto: " + String(state));

    if ((state, HEX) == 0x0E) {
      Serial.println("PTO AKTIF");
    }
  }

  if (pgnID == 0xFE6C) {
    /* ### Tachogr. Vehicle Speed ### */
    float speed1 = data[6];
    float speed2 = data[7];
    float total = (speed1 / 256) + ((speed2 * 256) / 256);
    tacVehSpeed_str = String(total);
  }

  if (pgnID == 0xF003) {
    /* ### Accelerator Pedal Position 1 ### */
    float accPed = data[1] / 2.5;
    float engLoad = data[2];
    accPed_str = String(accPed);
    engLoad_str = String(engLoad);
  }

  if (pgnID == 0xF000) {
    /* ### Actual Retarder ### */
    float actRet = data[1] - 125;
    actRet_str = String(abs(actRet));
  }

  if (pgnID == 0xF001) {
    /* ### Brake Pedal Position ### */
    float brakePedPos = data[1] / 2.5;
    brakePedPos_str = String(brakePedPos);
  }

  if (pgnID == 0xFEAE) {
    /* ### Air Pressure ### */
    float air3 = data[1];
    float air1 = data[2];
    float air2 = data[3];
    airPress3_str = String(air3 * 0.078125);
    airPress1_str = String(air1 * 0.078125);
    airPress2_str = String(air2 * 0.078125);
  }

  if (pgnID == 0xFEF2) {
    /* ### Fuel Rate ### */
    float fuel1 = data[0];
    float fuel2 = data[1];
    float total = (fuel1 / 20) + ((fuel2 * 256) / 20);
    fuelRate_str = String(total);
  }

  if (pgnID == 0xFEE5) {
    /* ### Engine Hours Meter ### */
    float hm1 = data[0];
    float hm2 = data[1];
    float hm3 = data[2];
    float hm4 = data[3];
    float total = hm1 / 20 + (hm2 * 256) / 20 + (hm3 * 65536) / 20 + (hm4 * 16777216) / 20;
    String xengHm_str = String(total - 1);
    if (xengHm_str != "") {
      engHm_str = xengHm_str;
      writeFile(SPIFFS, "/engHm_str.txt", xengHm_str.c_str());
    }
  }

  if (pgnID == 0xFEEE) {
    /* ### Coolant Temperature ### */
    int engCoolantTemp = data[0] - 40;
    engClnTemp_str = String(engCoolantTemp);
  }

  if (pgnID == 0xFEFC) {
    /* ### Fuel Level ### */
    float fuel = data[1] / 2.5;
    fuelLvl_str = String(fuel);
  }

  if (pgnID == 0xFEF1) {
    /* ### Vehicle Speed ### */
    float speed1 = data[1];
    float speed2 = data[2];
    float total = (speed1 / 256) + ((speed2 * 256) / 256);
    vehSpeed_str = String(total);
  }

  //  payload = "{\"en_cn\":\"0\",\"cn\":\"" + cn_str + "\",\"brand\":\"" + brand_str + "\",\"date_gps\":\"" + tanggalPost + "\",\"time_gps\":\"" + times + "\",\"lat\":\"" + lat_str + "\",\"lng\":\"" + lng_str + "\",\"alt\":\"" + alt_str + "\",\"spd\":\"" + spd_str + "\",\"heading\":\"" + heading_str + "\",\"agl\":\"" + incl_str + "\",\"st\":\"" + st_str + "\",\"fatigue\":\"" + kondisiFatigue +
  //            "\",\"engTorque\":\"" + engTorque_str + "\",\"engSpeed\":\"" + engSpeed_str + "\",\"tacVehSpeed\":\"" + tacVehSpeed_str + "\",\"accPed\":\"" + accPed_str + "\",\"engLoad\":\"" + engLoad_str + "\",\"actRet\":\"" + actRet_str + "\",\"brakePedPos\":\"" + brakePedPos_str + "\",\"airPress1\":\"" + airPress1_str + "\",\"airPress2\":\"" + airPress2_str + "\",\"airPress3\":\"" + airPress3_str +
  //            "\",\"fuelRate\":\"" + fuelRate_str + "\",\"engHm\":\"" + engHm_str + "\",\"engClnTemp\":\"" + engClnTemp_str + "\",\"fuelLvl\":\"" + fuelLvl_str + "\",\"vehSpeed\":\"" + vehSpeed_str + "\",\"adc_pto\":\"" + String(String(analogRead(35)) + "(" + String(pto) + ")") + "\",\"relay_buzzer\":\"" + relay_buzzer + "\",\"relay_dump\":\"" + relay_dump + "\",\"dump_status\":\"" + dump_status + "\"}";

  StaticJsonDocument<1024> buffer;
  JsonObject obj = buffer.to<JsonObject>();

  payload = "";
  obj["en_cn"] = "0";
  obj["cn"] = cn_str;
  obj["brand"] = brand;
  obj["date_gps"] = tanggalPost;
  obj["time_gps"] = times;
  obj["lat"] = lat_str;
  obj["lng"] = lng_str;
  obj["alt"] = alt_str;
  obj["spd"] = spd_str;
  obj["heading"] = heading_str;
  obj["agl"] = incl_str;
  obj["st"] = st_str;
  obj["engHm"] = engHm_str;
  obj["vehSpeed"] = vehSpeed_str;
  obj["engSpeed"] = engSpeed_str;
  obj["fatigue"] = kondisiFatigue;
  obj["engTorque"] = engTorque_str;
  obj["tacVehSpeed"] = tacVehSpeed_str;
  obj["accPed"] = accPed_str;
  obj["engLoad"] = engLoad_str;
  obj["actRet"] = actRet_str;
  obj["brakePedPos"] = brakePedPos_str;
  obj["airPress1"] = airPress1_str;
  obj["airPress2"] = airPress2_str;
  obj["airPress3"] = airPress3_str;
  obj["fuelRate"] = fuelRate_str;
  obj["engClnTemp"] = engClnTemp_str;
  obj["fuelLvl"] = fuelLvl_str;
  obj["adc_pto"] = String(pto);
  obj["relay_buzzer"] = relay_buzzer;
  obj["relay_dump"] = relay_dump;
  obj["dump_status"] = dump_status;
  obj["ignition"] = ignition;

  serializeJson(obj, payload);
}

void reset_data() {
  engHm_str = "";
  vehSpeed_str = "";
  engSpeed_str = "";
  kondisiFatigue = "";
  engTorque_str = "";
  tacVehSpeed_str = "";
  accPed_str = "";
  engLoad_str = "";
  actRet_str = "";
  brakePedPos_str = "";
  airPress1_str = "";
  airPress2_str = "";
  airPress3_str = "";
  fuelRate_str = "";
  engClnTemp_str = "";
  fuelLvl_str = "";
  engEr_str = "";
}

void no_fms() {
  //  payload = "{\"en_cn\":\"0\",\"cn\":\"" + cn_str + "\",\"brand\":\"" + brand_str + "\",\"date_gps\":\"" + tanggalPost + "\",\"time_gps\":\"" + times + "\",\"lat\":\"" + lat_str + "\",\"lng\":\"" + lng_str + "\",\"alt\":\"" + alt_str + "\",\"spd\":\"" + spd_str + "\",\"heading\":\"" + heading_str + "\",\"agl\":\"" + incl_str + "\",\"st\":\"" + st_str + "\",\"fatigue\":\"" + kondisiFatigue +
  //            "\",\"adc_pto\":\"" + String(String(analogRead(35)) + "(" + String(pto) + ")") + "\",\"relay_buzzer\":\"" + relay_buzzer + "\",\"relay_dump\":\"" + relay_dump + "\",\"dump_status\":\"" + dump_status + "\"}";

  StaticJsonDocument<1024> buffer;
  JsonObject obj = buffer.to<JsonObject>();

  payload = "";
  obj["en_cn"] = "0";
  obj["cn"] = cn_str;
  obj["brand"] = brand;
  obj["date_gps"] = tanggalPost;
  obj["time_gps"] = times;
  obj["lat"] = lat_str;
  obj["lng"] = lng_str;
  obj["alt"] = alt_str;
  obj["spd"] = spd_str;
  obj["heading"] = heading_str;
  obj["agl"] = incl_str;
  obj["st"] = st_str;
  obj["fatigue"] = kondisiFatigue;
  obj["adc_pto"] = String(pto);
  obj["relay_buzzer"] = relay_buzzer;
  obj["relay_dump"] = relay_dump;
  obj["dump_status"] = dump_status;
  obj["ignition"] = ignition;

  serializeJson(obj, payload);
}
