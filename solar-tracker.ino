#include <Servo.h>
#include <arduino-timer.h>
#include <SPI.h>
#include <SD.h>

#define AZ_PIN 9
#define AL_PIN 10

// Structures
struct PID_parameters{
  float K_p;
  float K_i;
  float K_d;
};

// Create objects
Servo AzimuthServo;
Servo AltitudeServo;
auto timer = timer_create_default();


// Function prototypes
int average(int val_1, int val_2);
void logData(float dataToWrite);
void servoWrite(int step, char motor);
void getSensorReadings(int*);
int getSystemError(int sensor_1, int sensor_2, int sensor_3, int sensor_4, int setpoint);
int getControlVariable(const PID_parameters &control_value, int error);

// System parameters
const int chipSelect = 4;
unsigned long g_systick = 0;
const long beat = 1000;
int g_az_val = 90;
int g_alt_val =  90;
int g_setpoint_ref = 0;

// Initialise control values
PID_parameters K_azimuth = {1,1,1};
PID_parameters K_altitude = {1,1,1};

// heartbeat ISR
bool heartbeat(void *){
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // toggle the LED
  return true; // repeat? true
}

void setup() {
  delay(3000);
  Serial.begin(115200);
  AzimuthServo.attach(AZ_PIN);  // attaches the servo on pin 9 to the Servo object
  AltitudeServo.attach(AL_PIN);
  pinMode(LED_BUILTIN, OUTPUT); 
  timer.every(500, heartbeat); // schedule timer to tick every 1000ms

  // see if the card is present and can be initialised
  /*if(!SD.begin(chipSelect)){
    Serial.println("Card failed, or not present.");
    while(1);
  }
  Serial.println("Card intialised.");*/
}

void loop() {
  
  int photo_resistor[4];
  getSensorReadings(photo_resistor);

  // Azimuth_reference = (average(photoResQ2,photoResQ3) - average(photoResQ1,photoResQ4)) == 0
  // Altitude_reference = (average(photoResQ1,photoResQ2) - average(photoResQ3,photoResQ4)) == 0
  int azimuth_error = getSystemError(photo_resistor[1],photo_resistor[2],photo_resistor[0],photo_resistor[3], g_setpoint_ref);
  int altitude_error = getSystemError(photo_resistor[0],photo_resistor[1],photo_resistor[2],photo_resistor[3], g_setpoint_ref);
  
  int control_variable_az = getControlVariable(K_azimuth, azimuth_error);
  int control_variable_alt = getControlVariable(K_altitude, altitude_error);

  g_az_val += control_variable_az;
  g_alt_val += control_variable_alt;

  servoWrite(constrain(g_az_val,0,180), 'z');
  servoWrite(constrain(g_alt_val,0,180), 'l');

  timer.tick();
}
