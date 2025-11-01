/* Reads sensors and returns the values by reference */

void getSensorReadings(double* sensor_value){
  sensor_value[0] = static_cast<double>(analogRead(A0)); //photoResQ1
  sensor_value[1] = static_cast<double>(analogRead(A1)); //photoResQ2
  sensor_value[2] = static_cast<double>(analogRead(A2)); //photoReQ3
  sensor_value[3] = static_cast<double>(analogRead(A3)); //photoResQ4
}
