/*****************************************************************************************************
  File:             Example01_BasicReading.ino
  Description:      This example initializes the capacitive touch sensor and to stream
                  which pad detects a touch.
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
  uint8_t key_value[5] = {0};
  if (sensor.getINT() == 0)
  {
    sensor.getKeyValueArray(key_value);
    for (uint8_t i = 0; i < sumofmodules; i++)
    {
      Serial.print("Key_value_Module");
      Serial.print(i+1);
      Serial.print(": ");
      Serial.print(key_value[i], BIN);
      Serial.println(" ");
      delay(500);
    }
    Serial.println();
  }
}
