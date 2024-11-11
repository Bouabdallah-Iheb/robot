//////////////////////////**/>- PID & ERROR -<\** /////////////////////////////

void check_Error(void) {
  revSpeed = PWM_Speed / 1.5;
  check_Sensor();
  //----------------------
  if (bin_Sensor == 0b00000001) {
    error = 10.0f;
  } else if (bin_Sensor == 0b00000011) {
    error = 8.0f;
  } else if (bin_Sensor == 0b00000111) {
    error = 6.0f;
  } else if (bin_Sensor == 0b00001111) {
    error = 4.0f;
  } else if (bin_Sensor == 0b00011111) {
    error = 3.0f;
  } else if (bin_Sensor == 0b00011110) {
    error = 2.0f;
  } else if (bin_Sensor == 0b00111110) {
    error = 1.0f;
  } else if (bin_Sensor == 0b00111100) {
    error = 0.0f;
  }  // Center
  else if (bin_Sensor == 0b01111100) {
    error = -1.0f;
  } else if (bin_Sensor == 0b01111000) {
    error = -2.0f;
  } else if (bin_Sensor == 0b11111000) {
    error = -3.0f;
  } else if (bin_Sensor == 0b11110000) {
    error = -4.0f;
  } else if (bin_Sensor == 0b11100000) {
    error = -6.0f;
  } else if (bin_Sensor == 0b11000000) {
    error = -8.0f;
  } else if (bin_Sensor == 0b10000000) {
    error = -10.0f;
  } else if (bin_Sensor == 0b00111111 || bin_Sensor == 0b01111111) {
    revSpeed = PWM_Speed;
    error = 10.0f;
  } else if (bin_Sensor == 0b11111100 || bin_Sensor == 0b11111110) {
    revSpeed = PWM_Speed;
    error = -10.0f;
  }
  //----------------------
}


void pidController(void) {
  // PID calculations
  float P = error;
  float D = error - prevError;
  PID = (Kp * P) + (Kd * D);
  prevError = error;
}

//////////////////////////**/>- PID & ERROR -<\** /////////////////////////////
