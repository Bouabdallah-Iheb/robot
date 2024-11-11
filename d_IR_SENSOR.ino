//////////////////////////**/>- IR SENSORS -<\** /////////////////////////////

void selectMultiplexerPin(byte pin) {
  for (int i = 0; i < 3; i++) {
    if (pin & (1 << i))
      digitalWrite(selectPins[i], HIGH);
    else
      digitalWrite(selectPins[i], LOW);
  }
}

void read_Sensor(void) {
  for (byte pin = 0; pin <= IR - 1; pin++) {
    selectMultiplexerPin(pin);                 //--> Select one at a time
    Sensor_ADC_0[pin] = analogRead(AN_Input);  //--> read X
    /*
    Serial.print("IR num : ");
    Serial.print(pin);
    Serial.print(" ");
    Serial.println(Sensor_ADC_0[pin]);
    delay(250);
    */
  }
}

void check_Sensor(void) {
  read_Sensor();  // read_Sensor() subroutine

  // Specifies a bit_Sensor array for HIGH or LOW
  for (byte i = 0; i <= IR - 1; i++) {
    if (Sensor_ADC_0[i] > MidIRValue) {
      bit_Sensor[i] = HIGH;
    } else {
      bit_Sensor[i] = LOW;
    }
  }
  // Inserting the bit_Sensor array into the bin_Sensor binary
  for (byte i = 0; i <= IR - 1; i++) {
    bitWrite(bin_Sensor, i, bit_Sensor[IR - 1 - i]);
  }
}

//////////////////////////**/>- IR SENSORS -<\** /////////////////////////////
