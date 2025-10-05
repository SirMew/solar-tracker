/* Passes struct by reference as a const so that the data isn't copied from the stack to the heap, and the const stops K values from being set in this function */
int getControlVariable(const PID_parameters &control_value, int error, const int upper_time, const int lower_time,  const int old_error){
  
  // Propotional
  int proportional = error;
  
  // Integral
  // A proper integral term should accumulate across calls:
  // static float integral_sum = 0;
  // integral_sum += error * dt;
  int integral = error*(upper_time - lower_time);
  
  // Derivative
  int derivative = (error - old_error)/(upper_time - lower_time);
  
  int control_var = control_value.K_p*proportional + control_value.K_i*integral + control_value.K_d*derivative;
  
  return control_var;
}
