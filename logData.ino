/* Logs float data to a SD card */
void logData(float dataToWrite){
  String dataString = "";

  dataString = String(dataToWrite);

  //open file
  File dataFile = SD.open("logfile.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    // Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("Error opening logfile.txt.");
  }
}