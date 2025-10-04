/* Reads sensors and returns the values */

int getSensorReadings(){
  int sensor_value[4];
  sensor_value[0] = analogRead(A0); //photoResQ1
  sensor_value[1] = analogRead(A1); //photoResQ2
  sensor_value[2] = analogRead(A2); //photoResQ3
  sensor_value[3] = analogRead(A3); //photoResQ4
  return sensor_value;
}
