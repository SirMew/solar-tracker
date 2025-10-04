int getSystemError(int sensor_1, int sensor_2, int sensor_3, int sensor_4, int setpoint){
  int feedback = average(sensor_1, sensor_2) - average(sensor_3, sensor_4);
  int error = setpoint - feedback;
  return error;
}
