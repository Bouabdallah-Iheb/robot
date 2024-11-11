//////////////////////////**/>- BAT RGB BUZZER -<\** /////////////////////////////

float batVoltage(void) {
  // 100 sampales
  static int vReadings[64] = { 0 };  // Array to store voltage readings for moving average
  static int index = 0;              // Index to keep track of the current position in the array

  int vReading = analogRead(Vbat);
  vReading = analogRead(Vbat);
  delay(1);

  // Update the array with the latest reading
  vReadings[index] = vReading;

  // Move to the next position in the array
  index = (index + 1) % 64;

  // Calculate the moving average
  int sum = 0;
  for (int i = 0; i < 64; i++) {
    sum += vReadings[i];
  }
  float movingAvg = static_cast<float>(sum) / 64;

  const float alpha = 0.5;  // Smoothing factor (0 < alpha < 1)
  vReading = (alpha * movingAvg) + ((1 - alpha) * movingAvg);

  // Convert to voltage (12-bit ADC and 3.3 voltage refrence)
  float v = (vReading * 3.3f) / 4095.0f;

  // Scale to Vbat voltage (8.4V Max voltage input and 3.3 max Vout of vlotage divider)
  return ((v * (8.4f / 3.3f)));  //Resistors : (3.3k+2.2k)
}

int calculateBatteryPercentage(float voltage) {
  const float fullVoltage = 8.4f;
  const float emptyVoltage = 7.4f;

  // Ensure that voltage is within the valid range
  voltage = constrain(voltage, emptyVoltage, fullVoltage);

  // Calculate the percentage using linear mapping
  int percentage = map(voltage * 1000, emptyVoltage * 1000, fullVoltage * 1000, 0, 100);

  const float alpha = 0.5;  // Smoothing factor (0 < alpha < 1)
  percentage = (alpha * percentage) + ((1 - alpha) * percentage);

  return percentage;
}

void beep() {
  ledcSetup(BUZZER_CHANNEL, NOTE_C6, 14);
  ledcWrite(BUZZER_CHANNEL, buzzer_volume - 400);
  delay(20);
  //ledcSetup(BUZZER_CHANNEL, 0, 14);
  ledcWrite(BUZZER_CHANNEL, 0);
}

void fadeColor(int channel, int targetIntensity) {
  int currentIntensity = ledcRead(channel);
  if (currentIntensity < targetIntensity) {
    if (ModeUI && (track_menu_item != 1 || !subMenu))  // Stop when this condition
      for (int i = currentIntensity; i <= targetIntensity; ++i) {
        analogWrite(channel, i);
        delayMicroseconds(500);  // Adjust the delay for a smoother fade
      }
  } else {
    if (ModeUI && (track_menu_item != 1 || !subMenu))  // Stop when this condition
      for (int i = currentIntensity; i >= targetIntensity; --i) {
        analogWrite(channel, i);
        delayMicroseconds(500);  // Adjust the delay for a smoother fade
      }
  }
}

void fadeRGB() {
  fadeColor(LEDC_CHANNEL_R, 1023);  // Fade Red to maximum intensity
  fadeColor(LEDC_CHANNEL_R, 0);     // Fade Red to off
  fadeColor(LEDC_CHANNEL_G, 0);     // Fade Green to off
  fadeColor(LEDC_CHANNEL_B, 0);     // Fade Blue to off
  fadeColor(LEDC_CHANNEL_G, 1023);  // Fade Green to maximum intensity
  fadeColor(LEDC_CHANNEL_R, 0);     // Fade Red to off
  fadeColor(LEDC_CHANNEL_G, 0);     // Fade Green to off
  fadeColor(LEDC_CHANNEL_B, 0);     // Fade Blue to off
  fadeColor(LEDC_CHANNEL_B, 1023);  // Fade Blue to maximum intensity
  fadeColor(LEDC_CHANNEL_R, 0);     // Fade Red to off
  fadeColor(LEDC_CHANNEL_G, 0);     // Fade Green to off
  fadeColor(LEDC_CHANNEL_B, 0);     // Fade Blue to off
}

void startingAnimation() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];

    // Set up PWM for the current note
    ledcSetup(BUZZER_CHANNEL, melody[thisNote] * 1.47, 14);
    ledcWrite(BUZZER_CHANNEL, buzzer_volume);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // Turn off PWM
    ledcWrite(BUZZER_CHANNEL, 0);
    delay(10);  // A short delay to allow the PWM to settle before the next note
    if (rgb_tone_toggle) {
      analogWrite(LEDC_CHANNEL_G, 1023);
      analogWrite(LEDC_CHANNEL_B, 0);
      rgb_tone_toggle = false;
    } else {
      analogWrite(LEDC_CHANNEL_G, 0);
      analogWrite(LEDC_CHANNEL_B, 1023);
      rgb_tone_toggle = true;
    }
  }
}

//////////////////////////**/>- BAT RGB BUZZER -<\** /////////////////////////////
