//////////////////////////**/>- MOTOR DRIVER -<\** /////////////////////////////

void driveMotors(bool on, bool oneDirection) {

  // Applying the result of PID calculation to the driving motors.
  PWM_R = PWM_Speed - PID;
  PWM_L = PWM_Speed + PID;

  // Apply constraints to limit PWM values
  PWM_R = constrain(PWM_R, MIN_SP, MAX_SP);
  PWM_L = constrain(PWM_L, MIN_SP, MAX_SP);

  if (on) {
    if (oneDirection || (error >= -overshoot && error <= overshoot)) {
      // Motors turns at the same forword direction (speed may differ)
      analogWrite(M_L_Channel_0, 0);
      analogWrite(M_L_Channel_1, PWM_L);
      analogWrite(M_R_Channel_2, PWM_R);
      analogWrite(M_R_Channel_3, 0);

    } else if (!oneDirection) {
      if (error <= -overshoot) {  // Turn LEFT
        //PWM_L = revSpeed;
        // Motor LEFT
        analogWrite(M_L_Channel_0, revSpeed);  //there is a wierd bug here
        analogWrite(M_L_Channel_1, 0);
        // Motor RIGHT
        analogWrite(M_R_Channel_2, PWM_R);
        analogWrite(M_R_Channel_3, 0);
      } else if (error >= overshoot) {  // Turn RIGHT
        //PWM_R = revSpeed;
        // Motor LEFT
        analogWrite(M_L_Channel_0, 0);
        analogWrite(M_L_Channel_1, PWM_L);
        // Motor RIGHT
        analogWrite(M_R_Channel_2, 0);
        analogWrite(M_R_Channel_3, revSpeed);
      }
    }
  } else {
    analogWrite(M_L_Channel_0, 0);
    analogWrite(M_L_Channel_1, 0);
    analogWrite(M_R_Channel_2, 0);
    analogWrite(M_R_Channel_3, 0);
  }
}

//////////////////////////**/>- MOTOR DRIVER -<\** /////////////////////////////
