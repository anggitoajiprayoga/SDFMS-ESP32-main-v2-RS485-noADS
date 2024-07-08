void read_setting() {
  cn_str = readFile(SPIFFS, "/cn_str.txt");
  brand = readFile(SPIFFS, "/brand.txt");

  if (cn_str == "" and brand == "") {
    data_default();
  }

  cn_str = readFile(SPIFFS, "/cn_str.txt");
  brand = readFile(SPIFFS, "/brand.txt");
  set_bitrate = readFile(SPIFFS, "/set_bitrate.txt");
  en_fatiq = readFile(SPIFFS, "/en_fatiq.txt");
  en_fms = readFile(SPIFFS, "/en_fms.txt");
  en_overspeed = readFile(SPIFFS, "/en_overspeed.txt");
  limit_speed = readFile(SPIFFS, "/limit_speed.txt");
  en_anglewarning = readFile(SPIFFS, "/en_anglewarning.txt");
  limit_angle = readFile(SPIFFS, "/limit_angle.txt");
  engHm_str = readFile(SPIFFS, "/engHm_str.txt");
}

void data_default() {
  writeFile(SPIFFS, "/cn_str.txt", "SDFMS SS6");
  writeFile(SPIFFS, "/set_bitrate.txt", "125");
  writeFile(SPIFFS, "/brand.txt", "mercy");
  writeFile(SPIFFS, "/en_fatiq.txt", "false");
  writeFile(SPIFFS, "/en_fms.txt", "false");
  writeFile(SPIFFS, "/en_overspeed.txt", "false");
  writeFile(SPIFFS, "/limit_speed.txt", "50");
  writeFile(SPIFFS, "/en_anglewarning.txt", "false");
  writeFile(SPIFFS, "/limit_angle.txt", "4");
  writeFile(SPIFFS, "/engHm_str.txt", "0");
  Serial.println("default done.");
}

String readFile(fs::FS &fs, const char * path) {
  File file = fs.open(path, "r");
  if (!file || file.isDirectory())
  {
    return String();
  }
  String fileContent;
  while (file.available())
  {
    fileContent += String((char)file.read());
  }
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message)
{
  File file = fs.open(path, "w");
  if (!file)
  {
    return;
  }
  file.print(message);
}
