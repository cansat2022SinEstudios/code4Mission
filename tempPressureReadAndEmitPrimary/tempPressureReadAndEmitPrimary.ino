/****
 * PRIMARY MISSION
 * 
 * TEMPERATURE 
 * Reading from a Vishay NTCLE100E3 Thermistor
 * Code for thermistor readings taken from 
 * https://www.eprojectszone.com/measuring-tmperature-with-an-arduino-and-a-thermistor/
 * Thermistor spec https://www.mouser.es/datasheet/2/427/VISH_S_A0012091193_1-2572478.pdf
 *
 * PRESSURE
 * MPX1145a sensor
 * https://oz9aec.net/hardware/arduino/mpx4115a-pressure-sensor-with-arduino
 * Sensor spec https://www.nxp.com/files-static/sensors/doc/data_sheet/MPX4115.pdf
 *
 *
 * EMIT/RECEIVE
 * APC220
 * https://www.famosastudio.com/download/datasheet/APC220_Manual_en.pdf
 *
 * 
 * 
 * CANSAT SINESTUDIOS TEAM - IES Villaverde
 * 
 */


/**
 * Primary Mission
 * Thermistor variables block
 */
// pin defs
int primaryMissionTmpPin = 1;
// variables
int R1=9790; // the resistor from voltage divider
float Bt=3977.0;// B parameter
float Tc=0;// tmperature in celsius

/**
 * Primary Mission
 * Pressure sensor variables block
 */
#define kpa2atm 0.00986923267
// pin defs
int primaryMissionPressurePin = 0;
// variables
int val;
float pkPa; // pressure in kPa
float pAtm; // pressure in Atm

//Main variables
int secondsCounter = 0; 

void setup(void) {
 
 Serial.begin(9600);
 Serial.print("Team;Second;Temperature;Pressure;Altitude\n");
}

void loop(void) { 

  //Calls tmperatue/pressure read function every second
  float TAnalogRead;
  float PAnalogRead;
  float tmp;
  float pressure;

   if (((millis()) % 1000) == 0)
  {
    TAnalogRead = analogRead(primaryMissionTmpPin);
    PAnalogRead = analogRead(primaryMissionPressurePin);
    tmp = updatePrimaryMissionTmp(TAnalogRead);
    pressure = updatePrimaryMissionPressure(PAnalogRead);
    float altitude = getISAAltitudeFromPression(pressure); //https://es.abcdef.wiki/wiki/Pressure_altitude
    Serial.print("sinestudios"); //Team name
    Serial.print("\t");
    Serial.print(secondsCounter);
    Serial.print("\t");
    Serial.print(tmp); // send temperature to serial port for telemetry - 
    Serial.print("\t");
    Serial.print(pressure); // send pressure to serial port for telemetry - 
    Serial.print("\t");
    Serial.print(altitude); // send altitude-pression
    Serial.print("\n"); 
    secondsCounter++;
  }

} 

  
float updatePrimaryMissionTmp(float TAnalogRead)// function which computes tmperature
{
  float Vout=TAnalogRead*(5/1023.0);// transform the analog read in voltage
  float Rt=R1*Vout/(5.0-Vout);// the resistance of thermistor from the voltage divider
                              //this value changes with tmperature
  float a1=1/298.15;//298.155 represent 25 degrees celsius in kelvin
                     //and 1/298.15 represent 1/To in equation
  float b1=1/Bt;// 1/Btsecond parameter in equation
  float c1=log(Rt/10000.0);//third parameter, 50000 ohms are Ro at zero degrees celsius
  float y1=a1+b1*c1;// the result of equation
  float  T=1/y1;// tmperature in kelvin
        Tc=T-273.15;//tmperature in celsius
        return Tc;
        Serial.println(Tc);
}

float updatePrimaryMissionPressure(float PAnalogRead)// function wich computes pressure
{
  pkPa = (PAnalogRead/1023.0+0.095)/0.009;
  //pAtm = kpa2atm*pkPa;
  //return pAtm;
  return pkPa*10.0;
}

float getISAAltitudeFromPression(float pressure)
{
  float h=(145366.45*(1-pow(pressure/1013.25,0.190284)))/3.28084;
  return h;
}
