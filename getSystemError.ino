double getSystemError(double sensor_1, double sensor_2, double sensor_3, double sensor_4, double setpoint){
  double feedback = average(sensor_1, sensor_2) - average(sensor_3, sensor_4);
  double error = setpoint - feedback;
  return error;
}
