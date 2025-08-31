/* Writes PWM values to the azimuth or altitude servo motor depending on a char argument */
int servoWrite(int step, char motor){
  unsigned long currentMillis = millis();
  const int interval = 5;
    if ((motor == 'z')){
      AzimuthServo.write(step);
      Serial.print("az- ");
      Serial.println(step);
      if ((currentMillis - g_systick >= interval)){
        g_systick = currentMillis;
        AzimuthServo.write(step);
      }
    }
    else if((motor == 'l')){
      AltitudeServo.write(step);
      Serial.print("al - ");
      Serial.println(step);
      if ((currentMillis - g_systick >= interval)){
        g_systick = currentMillis;
        AltitudeServo.write(step);
      }
    }
    else{
      //code should never reach this point
      Serial.println("ERROR: servoWrite reached impossible else statement");
      //logData("Error: servoWrite reached impossible else statement");
    }
  }