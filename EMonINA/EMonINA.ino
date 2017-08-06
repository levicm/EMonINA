#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

long seconds = 0;
unsigned long timeMillis = 0;
unsigned long lastMillis = 0;
unsigned long samples = 0;

// Valores instantaneos
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;
float lastPower_mW = 0;
float energy = 0;

// Valores acumulados
float totamps = 0;
float avgamps = 0;
float mAphr = 0;
float totwatts = 0;
float avgwatts = 0;
float mWphr = 0;
double mAh = 0;
double mWh = 0;

void setup(void) {
  Serial.begin(115200);
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  ina219.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  Serial.println("Measuring voltage and current with INA219 ...");
  printHeader();
}

void loop(void) {
  timeMillis = millis();
  long elapsedTime = timeMillis - lastMillis;
  seconds = timeMillis / 1000; // convert time to sec
  samples++;

  // Valores instantâneos
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  power_mW = busvoltage * current_mA;

  // Valores acumulados
  totamps = totamps + current_mA;
  avgamps = totamps / samples;
  mAphr = (avgamps * seconds) / 3600;
  totwatts = totwatts + power_mW;
  avgwatts = totwatts / samples;
  mWphr = (avgwatts * seconds) / 3600;

  energy = ((power_mW + lastPower_mW) / 2) * (elapsedTime) / 3600000;
  mWh += energy;

  printValues();
  //printDebug();

  lastPower_mW = power_mW;
  lastMillis = timeMillis;

  delay(10);
}

void printHeader() {
  Serial.print("Millis (ms);"); 
  Serial.print("Time (s);"); 
  Serial.print("Voltage (V);"); 
  Serial.print("Current (mA);"); 
  Serial.print("Load (mA/h);"); 
  Serial.print("Power (mW);"); 
  Serial.print("Energy (mW/h);"); 
  Serial.print("LastEnergy (mW/h);"); 
  Serial.println();
}

void printValues() {
  Serial.print(timeMillis); Serial.print(';'); 
  Serial.print(seconds); Serial.print(';'); 
  Serial.print(busvoltage); Serial.print(';'); 
  Serial.print(current_mA); Serial.print(';'); 
  Serial.print(mAphr); Serial.print(';'); 
  Serial.print(power_mW); Serial.print(';'); 
  Serial.print(mWphr); Serial.print(';'); 
  Serial.print(mWh); Serial.print(';'); 
  Serial.print(energy); Serial.print(';'); 
  Serial.println(""); 
}

void printDebug() {
  Serial.print("Millis:          "); Serial.print(timeMillis); Serial.println(" s");
  Serial.print("Time:          "); Serial.print(seconds); Serial.println(" s");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Load:          "); Serial.print(mAphr); Serial.println(" mA/h");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.print("Energy:        "); Serial.print(mWphr); Serial.println(" mW/h");
  Serial.println(""); 
}

