/*****************************************************************************************************
File:             Example03_SetThreshold.ino
Description:      This example set the threshold for different applications. 
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
  /*Set Key_Threshlod*/
  /*Recommand inputs to this function are ints between 16 and 64;The larger the input, the less sensitive the translation */

  //sensor.setThresholdAll(16);
  sensor.setThresholdSingle(1, 25);//Set the threshold of the first module to 25
  sensor.setThresholdSingle(2, 30);//Set the threshold of the second module to 30
  Serial.print("Module1's threshold is ");
  Serial.println(sensor.getThresholdSingle(1));
  Serial.print("Module2's threshold is ");
  Serial.println( sensor.getThresholdSingle(2));
}

void loop()
{
  static uint8_t  key_number_last;
  static uint32_t time_out;
  if(sensor.getINT() == 0)
  {   
    time_out = millis();
    if(sensor.getKeyValue() != key_number_last)
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
    if(millis() - time_out > 100)
    {
      key_number_last = 0;
    }
  }
}
