/****
 * Reading from a Vishay NTCLE100E3 Thermistor
 * TEMPERATURE 
 * Code for thermistor readings taken from 
 * https://www.eprojectszone.com/measuring-temperature-with-an-arduino-and-a-thermistor/
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
 *
 * CANSAT SINESTUDIOS TEAM - IES Villaverde
 */

/**
 * Thermistor variables block
 */
// pin defs
int temperaturePin = 1;
// variables
int R1=10000; // the resistor from voltage divider
float Bt=3977.0;// B parameter
float Tc=0;// temperature in celsius

/**
 * Pressure sensor variables block
 */
#define kpa2atm 0.00986923267
// pin defs
int pressurePin = 0;
// variables
int val;
float pkPa; // pressure in kPa
float pAtm; // pressure in Atm

//Main variables
int secondsCounter=0; 

void setup(void) {
 
 Serial.begin(9600);
 Serial.print("Misión Primaria CANSAT SinEstudios Team hoja de datos.");
 Serial.print("\n");
 Serial.print("Current Time:");
 Serial.print(millis());
 Serial.print("\n");
 Serial.print("Second;Temperature;Pressure\n");

}

void loop(void) { 

  //Calls temperatue/pressure read function every second
  float TAnalogRead;
  float PAnalogRead;
  float temp;
  float pressure;

   if (((millis()) % 1000) == 0)
  {
    TAnalogRead = analogRead(temperaturePin);
    PAnalogRead = analogRead(pressurePin);
    temp = updatetemp(TAnalogRead);
    pressure = updatePressure(PAnalogRead);
    //Serial.print(millis());
    //Serial.print(";");
    Serial.print(secondsCounter);
    Serial.print(";");
    Serial.print(temp);
    Serial.print(";");
    Serial.print(pressure);
    Serial.print("\n");
    secondsCounter++;
  }

} 
  
float updatetemp(float TAnalogRead)// function which computes temperature
{
  float Vout=TAnalogRead*(5/1023.0);// transform the analog read in voltage
  float Rt=R1*Vout/(5.0-Vout);// the resistance of thermistor from the voltage divider
                              //this value changes with temperature
  float a1=1/298.15;//298.155 represent 25 degrees celsius in kelvin
                     //and 1/298.15 represent 1/To in equation
  float b1=1/Bt;// 1/Btsecond parameter in equation
  float c1=log(Rt/10000.0);//third parameter, 50000 ohms are Ro at zero degrees celsius
  float y1=a1+b1*c1;// the result of equation
  float  T=1/y1;// temperature in kelvin
        Tc=T-273.15;//temperature in celsius
        return Tc;
        Serial.println(Tc);// display temperature in serial monitor
}

float updatePressure(float PAnalogRead)// function wich computes pressure
{
  pkPa = (PAnalogRead/1023.0+0.095)/0.009;
  pAtm = kpa2atm*pkPa;
  return pAtm;
}
