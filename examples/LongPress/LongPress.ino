/*****************************************************************************************************
File:             Example04_LongPress.ino
Description:      This example shows the long press function of the touch button
Note:               
******************************************************************************************************/
#include <BMK52M134.h>
BMK52M134 sensor(2, &Wire); //Please uncomment out this line of code if you use Wire on BMduino
//BMK52M134 sensor(22,&Wire1);      //Please uncomment out this line of code if you use Wire1 on BMduino
//BMK52M134 sensor(25,&Wire2);  //Please uncomment out this line of code if you use Wire2 on BMduino

uint8_t sumofmodules;

void setup()
{
  Serial.begin(9600);     //Start serial for output
  sensor.begin();
  sumofmodules = sensor.getNumber();
  if (sumofmodules != 0)
  {
    Serial.print(sumofmodules);
    Serial.print(" modules are ");
    Serial.println("Connected!");
  }
  else
  {
    Serial.println("Not connected. Please check connections.");
  }
}

void loop()
{
  static uint32_t key_start_time, key_current_time, time_out;
  static uint8_t  key_number_last, count;


  if(sensor.getINT() == 0)
  {   

    time_out = millis();
    if(sensor.getKeyValue() != key_number_last)
    {
      key_start_time = millis();
      key_current_time = millis();
      count = 0;
      key_number_last = sensor.getKeyValue();
      Serial.print("Key_number is ");
      Serial.print(sensor.getKeyValue());
      Serial.println();
    }
    else if(key_number_last == sensor.getKeyValue())
    {
      key_current_time = millis();
    }

    if( key_current_time - key_start_time > 3000)
    {
      Serial.print(" and Keep ");
      Serial.print(((key_current_time - key_start_time) / 1000) * (count + 1));
      Serial.println("s");
      key_start_time = millis();
      count++;
    }   
  }
  else
  {
    if( millis() - time_out > 300)
    {
      key_number_last = 0;  
    }
  }
}
