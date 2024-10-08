float readVoltage() {
  int raw = analogRead(A1);
  float mv = raw * 3.22265;
  return mv / 1000;
}

void setup()
{
  //Setup the local serial interface
  Serial.begin(9600);

  //Setup the wireless interface
  Serial0.begin(115200);
  Serial0.print("Voltage: ");
  Serial0.print(readVoltage());
  Serial0.println("V");
  
  // Initialize the reflectance sensors module
  reflectanceSensors.init();

  // Wait for the user button to be pressed and released
  Serial.println("ready to calibrate");
  Serial0.println("ready to calibrate");
  button.waitForButton();
  Serial.println("calibrating");
  Serial0.println("calibrating");

  // Turn on LED to indicate we are in calibration mode
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line

  // You should update this section to automate the calibration process
  // Edit here------------------

  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  motors.setSpeeds(-200, 200);

  for (int i = 0; i < 50; i++) {
    reflectanceSensors.calibrate();
    delay(10);
  }
  motors.setSpeeds(200, -200);

  for (int i = 0; i < 100; i++) {
    reflectanceSensors.calibrate();
    delay(10);
  }

  motors.setSpeeds(-200, 200);

  for (int i = 0; i < 50; i++) {
    reflectanceSensors.calibrate();
    delay(10);
  }
  motors.setSpeeds(0,0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
  // Wait for the user button to be pressed and released
  button.waitForButton();

  // Finish editing here--------
  Serial.println("calibration complete");
  Serial0.println("calibration complete");

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);

  
  // Wait for the user button to be pressed and released
  Serial.println("ready to begin");
  Serial0.println("ready to begin");
  button.waitForButton();
  Serial.println("start");
  Serial0.println("start");

  // Delay for one second
  delay(1000);
}


void loop()
{
  selectState();
  delay(50);
}
