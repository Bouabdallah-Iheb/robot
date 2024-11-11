//////////////////////////**/>- MENU -<\** /////////////////////////////

// Function to initialize the OLED display
void initialize_display() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.invertDisplay(inverted);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
}

bool button(String btn_name) {
  bool Button_Result;

  const char* btn = btn_name.c_str();  // Convert String to const char*

  switch (*btn) {  // Dereference the pointer to get the first character
    case 'u':      // 'u' corresponds to "up"
      Button_Result = digitalRead(Button_Pin_1);
      break;
    case 'd':  // 'd' corresponds to "down"
      Button_Result = digitalRead(Button_Pin_2);
      break;
    case 'e':  // 'e' corresponds to "enter"
      Button_Result = digitalRead(Button_Pin_3);
      break;
    case 'b':  // 'b' corresponds to "back"
      Button_Result = digitalRead(Button_Pin_4);
      break;
  }

  return !Button_Result;  // |I'm using pull-up| return ! (not) to be easier for me to later on (no need to inverted each time i use a button)
}

void menu() {

  if (button("back")) {
    beep();
    subMenu = false;
    ModeUI = true;
    onOff = false;
    display.setTextSize(1);
    display.clearDisplay();
    delay(debounce);
  }

  if (ModeUI) {

    /////////// This is here because moving avrage filter takes too long to get the first true value
    V = batVoltage();
    percentV = calculateBatteryPercentage(V);
    ///////////

    if (button("down") && track_menu_item < 5) {
      beep();
      if (!subMenu) {
        track_menu_item++;
        display.clearDisplay();
      }
      delay(debounce);
    }

    if (button("up") && track_menu_item > 0) {
      beep();
      if (!subMenu) {
        track_menu_item--;
        display.clearDisplay();
      }
      delay(debounce);
    }

    switch (track_menu_item) {
      case 0:
        if (!subMenu) {
          display.drawBitmap(0, 0, bitmap_OLED_UI_1, 128, 64, 1);
          display.display();
        }
        if (subMenu) {
          display.drawBitmap(0, 0, bitmap_Runnig_bmp, 128, 64, 1);
          display.display();
          ModeUI = false;
          onOff = true;
        }
        if (button("entere")) {
          beep();
          subMenu = true;
          display.clearDisplay();
          delay(debounce);
        }
        break;
      case 1:
        if (!subMenu) {
          display.drawBitmap(0, 0, bitmap_OLED_UI_2, 128, 64, 1);
          display.display();
        }
        if (subMenu) {
          displayValues();
        }
        if (button("entere")) {
          beep();
          subMenu = true;
          display.clearDisplay();
          delay(debounce);
        }
        break;
      case 2:
        if (!subMenu) {
          display.drawBitmap(0, 0, bitmap_OLED_UI_3, 128, 64, 1);
          display.display();
        }
        if (subMenu) {
          PID_subMenu();
        }
        if (button("entere")) {
          beep();
          subMenu = true;
          twoOptions = !twoOptions;
          display.clearDisplay();
          delay(debounce);
        }
        break;
      case 3:
        if (!subMenu) {
          display.drawBitmap(0, 0, bitmap_OLED_UI_4, 128, 64, 1);
          display.display();
        }
        if (subMenu) {
          Speed_subMenu();
        }
        if (button("entere")) {
          beep();
          subMenu = true;
          twoOptions = !twoOptions;
          display.clearDisplay();
          delay(debounce);
        }
        break;
      case 4:
        if (!subMenu) {
          display.drawBitmap(0, 0, bitmap_OLED_UI_5, 128, 64, 1);
          display.display();
        }
        if (subMenu) {
          Calib_subMenu();
        }
        if (button("entere")) {
          beep();
          subMenu = true;
          twoOptions = !twoOptions;
          display.clearDisplay();
          delay(debounce);
        }
        break;
      case 5:
        if (!subMenu) {
          display.drawBitmap(0, 0, bitmap_OLED_UI_6, 128, 64, 1);
          display.display();
        }
        if (subMenu) {
          Bat_subMenu();
        }
        if (button("entere")) {
          beep();
          subMenu = true;
          display.clearDisplay();
          delay(debounce);
        }
        break;
    }
  }
}

void PID_subMenu() {
  int kpValue = Kp;  // Create variables to hold the values
  int kdValue = Kd;
  handleSubMenu("KP", kpValue, 4, "KD", kdValue, 5, 2, 0, 255);  // Pass variables and range
  Kp = kpValue;                                                  // Update the original values
  Kd = kdValue;
}

void Speed_subMenu() {
  int pwmSpeedValue = PWM_Speed;  // Create variables to hold the values
  int maxSPValue = MAX_SP;
  handleSubMenu("B", pwmSpeedValue, 0, "M", maxSPValue, 1, 3, 0, 255);  // Pass variables and range
  PWM_Speed = pwmSpeedValue;                                            // Update the original values
  MAX_SP = maxSPValue;
}

void Calib_subMenu() {
  int highIRValue = HighIRValue;  // Create variables to hold the values
  int lowIRValue = LowIRValue;
  handleSubMenu("H", highIRValue, 2, "L", lowIRValue, 3, 4, 0, 4095);  // Pass variables and range
  HighIRValue = highIRValue;                                           // Update the original values
  LowIRValue = lowIRValue;
}

void Bat_subMenu() {
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(18, 8);
  display.print("%");
  display.setCursor(64, 8);
  display.print(percentV);
  display.setCursor(18, 40);
  display.print("V");
  display.setCursor(64, 40);
  display.print(V);
  display.display();
}

void handleSubMenu(String label1, int& parameter1, int EEPORM_addressParameter1, String label2, int& parameter2, int EEPORM_addressParameter2, int numDigits, int minValue, int maxValue) {
  int xCoordinate = 0;

  if (numDigits == 2) {
    xCoordinate = 80;
  } else if (numDigits == 3) {
    xCoordinate = 60;
  } else if (numDigits == 4) {
    xCoordinate = 40;
  }

  display.setTextSize(3);

  if (twoOptions) {
    display.drawBitmap(0, 0, bitmap_twoOptions_down, 128, 64, 1);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.setCursor(12, 4);
    display.print(label1);
    display.setCursor(xCoordinate, 4);
    display.print(parameter1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(12, 38);
    display.print(label2);
    display.setCursor(xCoordinate, 38);
    display.print(parameter2);
    display.display();
    if (button("up")) {
      if (parameter1 < maxValue) {
        parameter1++;  // Increment parameter1
        if (maxValue <= 255)
          writeToEEPROM(EEPORM_addressParameter1, byte(parameter1));
        else
          writeToEEPROM(EEPORM_addressParameter1, byte(parameter1 / 16));
        delay(debounce);
      }
    }

    if (button("down")) {
      if (parameter1 > minValue) {
        parameter1--;  // Decrement parameter1
        if (maxValue <= 255)
          writeToEEPROM(EEPORM_addressParameter1, byte(parameter1));
        else
          writeToEEPROM(EEPORM_addressParameter1, byte(parameter1 / 16));
        delay(debounce);
      }
    }
  } else {
    display.drawBitmap(0, 0, bitmap_twoOptions_up, 128, 64, 1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(12, 4);
    display.print(label1);
    display.setCursor(xCoordinate, 4);
    display.print(parameter1);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.setCursor(12, 38);
    display.print(label2);
    display.setCursor(xCoordinate, 38);
    display.print(parameter2);
    display.display();
    if (button("up")) {
      if (parameter2 < maxValue) {
        parameter2++;  // Increment parameter2
        if (maxValue <= 255)
          writeToEEPROM(EEPORM_addressParameter2, byte(parameter2));
        else
          writeToEEPROM(EEPORM_addressParameter2, byte(parameter2 / 16));
        delay(debounce);
      }
    }

    if (button("down")) {
      if (parameter2 > minValue) {
        parameter2--;  // Decrement parameter2
        if (maxValue <= 255)
          writeToEEPROM(EEPORM_addressParameter2, byte(parameter2));
        else
          writeToEEPROM(EEPORM_addressParameter2, byte(parameter2 / 16));
        delay(debounce);
      }
    }
  }
}

// Subroutine for displaying sensors in binary
void printBinaryByte(byte value) {
  for (byte mask = 0x80; mask; mask >>= 1) {  //0x80 : 8 IR //0x10 : 5 IR
    display.print((mask & value) ? '1' : '0');
  }
}
void displayValues() {
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(18, 8);
  printBinaryByte(bin_Sensor);
  display.setCursor(8, 30);
  display.print("R");
  display.setCursor(36, 30);
  display.print(PWM_R);
  display.setCursor(8, 48);
  display.print("L");
  display.setCursor(36, 48);
  display.print(PWM_L);
  // just for displaying the actual handling is by motor driver function
  if ((error <= -overshoot) || (error >= overshoot)) {
    display.setCursor(82, 48);
    display.print(revSpeed);
    display.display();
  } else {
    display.setCursor(82, 48);
    display.print("ONE");
    display.display();
  }
  display.display();
}

//////////////////////////**/>- MENU -<\** /////////////////////////////
