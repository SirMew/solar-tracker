/* Reads sensors and returns the values by reference */

void getSensorReadings(int* sensor_value){
  sensor_value[0] = analogRead(A0); //photoResQ1
  sensor_value[1] = analogRead(A1); //photoResQ2
  sensor_value[2] = analogRead(A2); //photoReQ3
  sensor_value[3] = analogRead(A3); //photoResQ4
}
