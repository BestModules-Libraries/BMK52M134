/*****************************************************************************************************
  File:             Example02_detectCurrentTouch.ino
  Description:      This example shows that the same key value fires only once.
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
  static uint8_t key_number_last;
  static uint32_t time_out;
  if (sensor.getINT() == 0)
  {
    time_out = millis();
    if (sensor.getKeyValue() != key_number_last)
    {
      key_number_last = sensor.getKeyValue();
      Serial.print("Key_number is ");
      Serial.print(sensor.getKeyValue());
      Serial.print(" ");
      Serial.println();
    }
  }
  else
  {
    if (millis() - time_out > 200)
    {
      key_number_last = 0;
    }
  }
}
