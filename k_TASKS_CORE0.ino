//////////////////////////**/>- TASKS_CORE0 -<\** /////////////////////////////

////////////////-CODE RUNS AT CORE 0-//////////////////
void Task1code(void* parameter) {
  for (;;) {
    if (ModeUI && (track_menu_item != 1 || !subMenu)) {
      fadeRGB();
    } else if (!ModeUI || (track_menu_item == 1 && subMenu)) {  //Runnig mode or Sensor reading menu
      if (error < 0.0f) {
        analogWrite(LEDC_CHANNEL_R, 1023);
        analogWrite(LEDC_CHANNEL_G, 0);
        analogWrite(LEDC_CHANNEL_B, 0);
      } else if (error > 0.0f) {
        analogWrite(LEDC_CHANNEL_R, 0);
        analogWrite(LEDC_CHANNEL_G, 0);
        analogWrite(LEDC_CHANNEL_B, 1023);
      } else {
        analogWrite(LEDC_CHANNEL_R, 0);
        analogWrite(LEDC_CHANNEL_G, 1023);
        analogWrite(LEDC_CHANNEL_B, 0);
      }
    }
  }
}

void Task2code(void* parameter) {
  for (;;) {
    menu();
  }
}
////////////////-CODE RUNS AT CORE 0-//////////////////

//////////////////////////**/>- TASKS_CORE0 -<\** /////////////////////////////