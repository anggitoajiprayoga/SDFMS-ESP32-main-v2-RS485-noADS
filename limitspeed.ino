void setup_overspeed() {

}

void loop_overspeed() {
  if (vehSpeed_str.toInt() > limit_speed.toInt()) {
    danger_alarm(500);
  }
  else {
    stop_alarm();
  }
}
