//////////////////////////**/>- EEPROM - 24C04 -<\** /////////////////////////////

// Write a byte of data to a specific address in the EEPROM and verify the write
void writeToEEPROM(byte dataAddr, byte dataVal) {
  // Read the current data value from the EEPROM
  byte currVal = readFromEEPROM(dataAddr);

  // Record the start time
  unsigned long startTime = millis();

  // Keep writing until the written value matches the desired value or 1 seconds have passed
  while (dataVal != currVal && (millis() - startTime < 1000)) {  // 1 sec to avoid infinite loop
    Wire.beginTransmission(EEPROM_ADDRESS);
    Wire.write(dataAddr);
    Wire.write(dataVal);
    Wire.endTransmission();
    delay(15);

    // Read the written value from the EEPROM
    currVal = readFromEEPROM(dataAddr);
  }
}

// Read a byte of data from a specific address in the EEPROM
byte readFromEEPROM(byte dataAddr) {
  Wire.beginTransmission(EEPROM_ADDRESS);
  Wire.write(dataAddr);
  Wire.endTransmission();
  delay(10);
  Wire.requestFrom(EEPROM_ADDRESS, 1);

  if (Wire.available()) {
    return Wire.read();
  }

  // Add a default return statement in case Wire.available() is false
  return dataAddr;
}


//////////////////////////**/>- EEPROM - 24C04 -<\** /////////////////////////////
