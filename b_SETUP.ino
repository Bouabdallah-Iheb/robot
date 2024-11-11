//////////////////////////**/>- SETUP -<\** /////////////////////////////

void setup() {

#if DEBUG == 1
  Serial.begin(115200);  // Start serial communication at 115200 baud
  delay(10);
#endif

  // Initialize I2C communication with OLED display
  Wire.begin();

  // Initialize OLED display
  initialize_display();

  // 4051 : select pins
  for (int i = 0; i < 3; i++) {
    pinMode(selectPins[i], OUTPUT);
    digitalWrite(selectPins[i], LOW);
  }

  // Buttons PIN as input
  pinMode(Button_Pin_1, INPUT_PULLUP);
  pinMode(Button_Pin_2, INPUT_PULLUP);
  pinMode(Button_Pin_3, INPUT_PULLUP);
  pinMode(Button_Pin_4, INPUT_PULLUP);

  // Motor PIN as output
  pinMode(Motor_L, OUTPUT);
  pinMode(Motor_R, OUTPUT);
  pinMode(Motor_L_PWM, OUTPUT);
  pinMode(Motor_R_PWM, OUTPUT);

  // configure PWM
  ledcSetup(M_L_Channel_0, freq, resolution);
  ledcAttachPin(Motor_L_PWM, M_L_Channel_0);
  ledcSetup(M_L_Channel_1, freq, resolution);
  ledcAttachPin(Motor_L, M_L_Channel_1);
  //
  ledcSetup(M_R_Channel_2, freq, resolution);
  ledcAttachPin(Motor_R_PWM, M_R_Channel_2);
  ledcSetup(M_R_Channel_3, freq, resolution);
  ledcAttachPin(Motor_R, M_R_Channel_3);

  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(buzzer_pin, HIGH);  //plus of buzzer is directly connected to 3.3V

  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  // Setup LEDC for PWM control
  ledcSetup(LEDC_CHANNEL_R, 19531, 10);
  ledcAttachPin(R_PIN, LEDC_CHANNEL_R);
  ledcSetup(LEDC_CHANNEL_G, 19531, 10);
  ledcAttachPin(G_PIN, LEDC_CHANNEL_G);
  ledcSetup(LEDC_CHANNEL_B, 19531, 10);
  ledcAttachPin(B_PIN, LEDC_CHANNEL_B);

  ledcSetup(BUZZER_CHANNEL, 2441, 14);
  ledcAttachPin(buzzer_pin, BUZZER_CHANNEL);

  // LOGO
  display.drawBitmap(0, 0, bitmap_logo, 128, 64, 1);
  display.display();
  //delay(1000);
  startingAnimation();
  //display.clearDisplay();

  /*
  //####################################################
  writeToEEPROM(0, byte(PWM_Speed));
  writeToEEPROM(1, byte(MAX_SP));
  writeToEEPROM(2, byte(HighIRValue / 16));
  writeToEEPROM(3, byte(LowIRValue / 16));
  writeToEEPROM(4, byte(Kp));
  writeToEEPROM(5, byte(Kd));
  //####################################################
  */
  // Read the stored data from EEPROM
  PWM_Speed = int(readFromEEPROM(0));
  MAX_SP = int(readFromEEPROM(1));
  HighIRValue = int((readFromEEPROM(2) * 16));
  LowIRValue = int((readFromEEPROM(3) * 16));
  Kp = float(readFromEEPROM(4));
  Kd = float(readFromEEPROM(5));

  /*
  if (digitalRead(Button_Pin_3) == LOW) {  //You need to hold Enter button to start Blynk
    display.clearDisplay();
    display.drawBitmap(0, 0, bitmap_load_wifi, 128, 64, 1);
    display.display();
    while (!Blynk.connected()) {
      Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
      Blynk.config(BLYNK_AUTH_TOKEN);
      Blynk.connect();
    }
    if (Blynk.connected())
      BlynkOn = true;
  }
*/

  ///////////////////////// >>TASKS<< /////////////////////////
  xTaskCreatePinnedToCore(
    Task1code,  /* Function to implement the task */
    "RGB_Fade", /* Name of the task */
    10000,      /* Stack size in words */
    NULL,       /* Task input parameter */
    0,          /* Priority of the task */
    NULL,       /* Task handle. */
    0);
  /* Core where the task should run */  //By default Arduino IDE runs code at core 1.

  xTaskCreatePinnedToCore(
    Task2code, /* Function to implement the task */
    "Menu",    /* Name of the task */
    10000,     /* Stack size in words */
    NULL,      /* Task input parameter */
    0,         /* Priority of the task */
    NULL,      /* Task handle. */
    0);
  /* Core where the task should run */  //By default Arduino IDE runs code at core 1.
  ///////////////////////// >>TASKS<< /////////////////////////

  display.clearDisplay();
}

//////////////////////////**/>- SETUP -<\** /////////////////////////////
