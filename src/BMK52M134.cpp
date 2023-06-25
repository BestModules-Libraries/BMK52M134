/*****************************************************************
  File:          BMK52M134.cpp
  Author:        BESTMODULES
  Description:   I2C communication with the BMK52M134
  History：
  V1.0.1  -- initial version;2023-03-31;Arduino IDE : ≥v1.8.16
******************************************************************/
#include "BMK52M134.h"
/*********************************************************************************
  Description: Constructor
  Input:       intPin: INT Output pin connection with Arduino,
                     the INT will be pulled down when an object approaches
              theWire: Wire object if your board has more than one I2C interface
  Output:
  Return:
  Others:
***********************************************************************************/
BMK52M134::BMK52M134(uint8_t intPin, TwoWire *theWire)
{
  _intPin = intPin;
  _wire = theWire;
}

/*********************************************************************************
  Description: This function initalizes the BMK52M134 sensor.
  Input:
  Output:
  Return:
  Others:
***********************************************************************************/
void BMK52M134::begin(uint8_t i2c_addr)
{
  _i2caddr = i2c_addr;
  _wire->begin();
  pinMode(_intPin, INPUT_PULLUP);

  delay(500);

  if (_SumOfModules == 1)
  {
    getNumber();
  }
  setThresholdAll(16);

}

/**********************************************************
Description: get Key Status
Parameters:       
Return:      Returns the INT state  
             0:INT output low level  press
             1:INT output high level   unpress    
Others:      
**********************************************************/
uint8_t BMK52M134::getINT()
{
     return (digitalRead(_intPin));
} 

/********************************************************************
  Description: Gets the number of modules
  Input:       *sumofmodules:  Store sum of modules.
  Output:
  Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE
  Others:
*********************************************************************/
uint8_t BMK52M134::getNumber()
{
  _tx_buf[3] = _CMD_CHECK_MODULE;

  delay(20);
  writeBytes(0x00, 5, _tx_buf);
  delay(30);
  readBytes(6, _rx_buf);

  _SumOfModules = _rx_buf[4];
  return _SumOfModules;
}
/********************************************************************
  Description:  Get key value
  Input:        key_value[]: Sotre key_value;
  Output:
  Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE
  Others:
*********************************************************************/
void BMK52M134::getKeyValueArray(uint8_t key_value[])
{


  _tx_buf[3] = _CMD_KEY_SCAN;

  delay(20);
  writeBytes(BROADCAST_ID, 5, _tx_buf);
  delay(30);

  readBytes(5 + _SumOfModules, _rx_buf);               //read ack

 
  /* 數組幀格式：
    MID | ID | LEN | CMD/STATUS | DATA0~n | CHECHSUM*/
  //DATA0~n:为模块ID 0x01~n 的key_value。
  for (uint8_t i = 0; i < _SumOfModules; i++)
  {
    key_value[i] = _rx_buf[i + 4];
  }

}
/********************************************************************
  Description:  Get key value
  Input:        *key_value: Sotre key_value;
  Output:
  Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE
  Others:
*********************************************************************/
uint8_t BMK52M134::getKeyValue()
{
  uint8_t status;
  uint8_t key_value = 0;

  _tx_buf[3] = _CMD_KEY_SCAN;

  delay(20);
  writeBytes(BROADCAST_ID, 5, _tx_buf);
  delay(30);

  readBytes(5 + _SumOfModules, _rx_buf);               //read ack
  status = _rx_buf[3];
  /* 數組幀格式：
    MID | ID | LEN | CMD/STATUS | DATA0~n | CHECHSUM*/
  //DATA0~n:为模块ID 0x01~n 的key_value。
  for (uint8_t i = 0; i < _SumOfModules; i++)
  {
    if (_rx_buf[i + 4] != 0)
    {
      for (uint8_t j = 0; j < 4; j++)
      {
        if ( _rx_buf[i + 4] & (1 << j) )
        {
          key_value = (j + 1) + 4 * i;
          return key_value;
        }
      }

    }
  }

  return status;
}
/********************************************************************
  Description: Set thresholds for all modules
  Input:       Threshold
  Output:
  Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE
  Others:
*********************************************************************/
uint8_t BMK52M134::setThresholdAll(uint8_t Threshold)
{
  uint8_t status;
  
  for (uint8_t i = 1; i <= _SumOfModules; i++)
  {
    status = setThresholdSingle(i, Threshold);
    if (status != 0)
    {
      return status;
    }
  }

  return status;
}
/********************************************************************
  Description: Sets a threshold for a single module
  Input:       sensor_number: The module number
             Threshold
  Output:
  Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE
  Others:
*********************************************************************/
uint8_t BMK52M134::setThresholdSingle(uint8_t sensor_number, uint8_t Threshold)
{
  uint8_t status;

  _tx_buf[3] = _CMD_SET_THR;
  _tx_buf[4] = Threshold;

  delay(20);
  writeBytes(sensor_number, 6, _tx_buf);
  delay(30);

  readBytes(5, _rx_buf);               //read ack

  status = _rx_buf[3];
  return status;
}
/********************************************************************
  Description: Gets the threshold for a single module
  Input:       sensor_number: The module number
             Threshold
  Output:
  Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE
  Others:
*********************************************************************/
uint8_t BMK52M134::getThresholdSingle(uint8_t sensor_number)
{

  uint8_t Threshold;
  _tx_buf[3] = _CMD_GET_THR;

  delay(20);
  writeBytes(sensor_number, 5, _tx_buf);
  delay(30);

  readBytes(6, _rx_buf);               //read ack

  Threshold = _rx_buf[4];
  return Threshold;
}
/********************************************************************
  Description: The sleep mode of the module was enabled. Procedure After 8 seconds, if no key is pressed, the module will enter the sleep mode.
  Input:       sleepen:SLEEP_MODE_DISABLE、SLEEP_MODE_ENABLE.
  Output:
  Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE
  Others:
*********************************************************************/
uint8_t BMK52M134::setSleepENAll(uint8_t sleepen)
{
  uint8_t status;

  for (uint8_t i = 1; i <= _SumOfModules; i++)
  {
    status = setSleepENSingle(i, sleepen);

    if (status != 0)
    {
      return status;
    }
  }

  return status;
}
/********************************************************************
  Description: The sleep mode of the module was enabled. Procedure After 8 seconds, if no key is pressed, the module will enter the sleep mode.
  Input:       sensor_number: The module number
             sleepen:SLEEP_MODE_DISABLE、SLEEP_MODE_ENABLE.
  Output:
  Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE
  Others:
*********************************************************************/
uint8_t BMK52M134::setSleepENSingle(uint8_t sensor_number, uint8_t sleepen)
{
  uint8_t status;

  _tx_buf[3] = _CMD_SET_SLEEPEN;
  _tx_buf[4] = sleepen;

  delay(20);
  writeBytes(sensor_number, 6, _tx_buf);
  delay(30);

  readBytes(5, _rx_buf);               //read ack

  status = _rx_buf[3];
  return status;
}
/********************************************************************
  Description: Get the SleepEN of module.
  Input:       sensor_number: The module number
             sleepen:SLEEP_MODE_DISABLE、SLEEP_MODE_ENABLE.
  Output:
  Return:      0x00: CMD_TRANSFER_SUCCESS
             0x40: CHECKSUM_ERROR
			 0x80: INSTRUCTION_NOT_SUPPOR
			 0xA0: SLAVE_NO_RESPONSE
  Others:
*********************************************************************/
uint8_t BMK52M134::getSleepENSingle(uint8_t sensor_number)
{

  uint8_t sleepen;
  _tx_buf[3] = _CMD_GET_SLEEPEN;

  delay(20);
  writeBytes(sensor_number, 5, _tx_buf);
  delay(30);

  readBytes(6, _rx_buf);               //read ack

  sleepen = _rx_buf[4];
  return sleepen;
}
/*----------------------- Internal I2C Abstraction -----------------------------*/
/*******************************************************************
  Description: Write data to the module through I2C
  Input:       id: The module number
             len:Length of data to be written
			 buffer: Write to an array of data
  Output:
  Return:      true or false.
  Others:
*******************************************************************/
bool BMK52M134::writeBytes(uint8_t id, uint8_t len, uint8_t *par)
{ /* 數組幀格式：
    MID | ID | LEN | CMD/STATUS | DATA0~n | CHECHSUM*/
  par[0] = MODULE_MID;
  par[1] = id;
  par[2] = len - 3;
  par[len - 1] = 0;

  for (uint8_t i = 0; i < len - 1; i++)
  {
    par[len - 1] += par[i];
  }
  //CheckSum = (MODULE_MID + ID + Len + CMD + Param0 + ...Paramn)

  _wire->beginTransmission(_i2caddr);
  //Serial.print("Send: ");
  for (uint8_t i = 0; i < len; i++)
  {
    _wire->write(par[i]);
    //Serial.print(par[i], HEX);
    //Serial.print(' ');
  }
  //Serial.println();
  if (_wire->endTransmission() == 0)
  {
    return true;
  }

  return false;
}
/**********************************************************
  Description: Read the data of the module through I2C
  Input:       len: The length of the data read
			 buffer: Store the read data
  Output:
  Return:      true or false.
  Others:
**********************************************************/
bool BMK52M134::readBytes(uint8_t len, uint8_t *buffer)
{
  uint8_t CheckSum = 0;

  //Serial.print("ACK: ");

  _wire->requestFrom(_i2caddr, len);

  if ( _wire->available() > 0)
  {
    for (uint8_t i = 0; i < len; i++)
    {
      buffer[i] = _wire->read();
      //Serial.print(buffer[i], HEX);
      //Serial.print(' ');
      if (i < len - 1)
      {
        CheckSum += buffer[i];
      }
    }
  }
  //Serial.println();
  if (CheckSum != buffer[len - 1])
  {
    return false;
  }
  return true;
}
