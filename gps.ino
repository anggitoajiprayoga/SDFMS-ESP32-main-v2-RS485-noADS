void setupGps() {
  serial_gps.begin(9600);
}

void loopGps() {
  if (millis() > 5000 and gps.charsProcessed() < 10) {
    Serial.println("No GPS data received: check wiring");
    eror_gps = true;
  }
  else {
    eror_gps = false;
  }

  while (serial_gps.available() > 0) {
    if (gps.encode(serial_gps.read())) {
      if (gps.date.isValid() && gps.time.isValid()) {
        int Year = gps.date.year();
        byte Month = gps.date.month();
        byte Day = gps.date.day();
        byte Hour = gps.time.hour();
        byte Minute = gps.time.minute();
        byte Second = gps.time.second();
        setTime(Hour, Minute, Second, Day, Month, Year);
        adjustTime(UTC_offset * SECS_PER_HOUR);
      }
    }
    // -- Delete this section if not displaying time ------- //
    if (timeStatus() != timeNotSet) {
      if (now() != prevDisplay) {
        prevDisplay = now();
        if (gps.location.isValid()) {
          latitude = gps.location.lat();
          lat_str = String(latitude, 6);
          longitude = gps.location.lng();
          lng_str = String(longitude, 6);
          speed = gps.speed.kmph();
          spd_str = String(speed);
          alt = gps.altitude.meters();
          alt_str = String(alt);
          heading = gps.course.deg();
          heading_str = String(heading);
          sat = gps.satellites.value();
          st_str = String(sat);
        }
        if (gps.date.isValid() && gps.time.isValid()) {
          if (year() >= 2023) {
            sprintf(wjam, "%02d:%02d:%02d", hour(), minute(), second());
            times = String(wjam);
            sprintf(wtanggal, "%02d%02d%02d", day(), month(), year());
            tanggal = String(wtanggal);
            sprintf(xtanggal, "%02d-%02d-%02d", year(), month(), day());
            tanggalPost = String(xtanggal);
          }
        }
      }
    }
  }
}
