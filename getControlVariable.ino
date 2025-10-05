/* Passes struct by reference as a const so that the data isn't copied from the stack to the heap, and the const stops K values from being set in this function */
int getControlVariable(const PID_parameters &control_value, int error){
  
  // Propotional
  int proportional = error;
  
  // Integral
  // TO DO
  int integral = 0;
  
  // Derivative
  // TO DO
  int derivative = 0;
  
  int control_var = control_value.K_p*proportional + control_value.K_i*integral + control_value.K_d*derivative;
  
  return control_var;
}
