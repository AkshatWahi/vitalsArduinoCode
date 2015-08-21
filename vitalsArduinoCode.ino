/* Arduino for Kumbha Data collection of Vitals
Shield Used - Cooking Hacks eHealth Platform v2.0
https://www.cooking-hacks.com/documentation/tutorials/ehealth-biometric-sensor-platform-arduino-raspberry-pi-medical

Sensors Used
1- Temperature
2- Skin Response
3- Pulse Oximetry
4- Blood Pressure

Author:- Akshat Wahi
Date Created :- 18 August 2015
Date Modified :- 18 August 2015
*/

#include <eHealth.h>;
#include <PinChangeInt.h>

char input = '\n';
unsigned long current_time;
int cont = 0;

void setup()
{
  Serial.begin(9600);
  delay(100);
  eHealth.initPulsioximeter();
  //Attach the inttruptions for using the pulsioximeter.   
  PCintPort::attachInterrupt(6, readPulsioximeter, RISING);
}

void loop()
{
  switch (input)
  {
    case 't':
      send_temperature();
      break;

    case 's':
      send_skin_reading();
      break;

    case 'p':
      send_pulseox();
      break;

    case 'd':
      //do nothing
      break;

    default:
      //do nothing
      break;

  }
}

void serialEvent()
{
  while (Serial.available())
  {
    input = (char)Serial.read();
  }
  if(input == 's')
  {
    current_time = millis();
  }
}

void send_temperature()
{
  float temp[10], avg_temp = 0.0;
  for (int x = 0; x < 10; x++)
  {
    temp[x] = eHealth.getTemperature();
    delay(20); //delay between each temperature read
  }

  
  for (int x = 0; x < 10; x++)
  {
    avg_temp += temp[x];
  }

  avg_temp /= 10;
  Serial.println(avg_temp);
//  Serial.print("t");
//  Serial.print(avg_temp);
//  Serial.println("#");
}

void send_skin_reading()
{
  float temp[3], avg_cond=0.0, avg_res= 0.0, avg_vol=0.0;
  unsigned long time_diff;
  
  /*
  for(int x=0;x<4;x++)
  {
    temp[0][x] = eHealth.getSkinConductance();
    temp[1][x] = eHealth.getSkinResistance();
    temp[2][x] = eHealth.getSkinConductanceVoltage();
    delay(20);
  }
  
  for(int x=0;x<4;x++)
  {
    avg_cond += temp[0][x];
    avg_res += temp[1][x];
    avg_vol += temp[2][x];
  }
  
  avg_cond /=4;
  avg_res /=4;
  avg_vol /=4;
  */
  time_diff = (unsigned long)(millis() - current_time);
  temp[0] = eHealth.getSkinConductance();
  temp[1] = eHealth.getSkinResistance();
  temp[2] = eHealth.getSkinConductanceVoltage();
  
  Serial.print(time_diff);
  Serial.print(",");
  Serial.print(temp[0]);
  Serial.print(",");
  Serial.print(temp[1]);
  Serial.print(",");
  Serial.println(temp[2]);
  delay(100);
  
//  Serial.print("s");
//  Serial.print(avg_cond);
//  Serial.print("#");
//  Serial.print(avg_res);
//  Serial.print("#");
//  Serial.print(avg_vol);
//  Serial.println("#");
  
}

void send_pulseox()
{
  Serial.print(eHealth.getBPM());
  Serial.print(",");
  Serial.println(eHealth.getOxygenSaturation());
  delay(100);
}


//Include always this code when using the pulsioximeter sensor
//=========================================================================
void readPulsioximeter(){  

  cont ++;

  if (cont == 50) { //Get only of one 50 measures to reduce the latency
    eHealth.readPulsioximeter();  
    cont = 0;
  }
}
