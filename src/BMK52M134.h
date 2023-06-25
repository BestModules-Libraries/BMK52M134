/*************************************************
  File:       	    BMK52M134.h
  Author:            	BESTMODULES
  Description:        Define classes and required variables
  History：
  V1.0.1	 -- initial version；2023-03-31；Arduino IDE : ≥v1.8.16
**************************************************/
#ifndef _BMK52M134_H
#define _BMK52M134_H

#include <Arduino.h>
#include <Wire.h>

#define BMK52M134_ADDRESS         0x71
#define MODULE_MID              0x71
#define BROADCAST_ID		    0x00



class BMK52M134
{
  public:
    BMK52M134(uint8_t intPin, TwoWire *theWire = &Wire);
    void begin(uint8_t i2c_addr = BMK52M134_ADDRESS);
    uint8_t getINT();
    uint8_t getNumber();
    void getKeyValueArray(uint8_t key_value[]);
    uint8_t getKeyValue();  
    uint8_t getThresholdSingle(uint8_t sensor_number);
    uint8_t getSleepENSingle(uint8_t sensor_number);
    uint8_t setThresholdAll(uint8_t Threshold);
    uint8_t setThresholdSingle(uint8_t sensor_number, uint8_t Threshold);
    uint8_t setSleepENAll(uint8_t sleepen);
    uint8_t setSleepENSingle(uint8_t sensor_number, uint8_t sleepen);
     
  private:
    //CMD
    const uint8_t _CMD_CHECK_MODULE = 0x01;
    const uint8_t _CMD_KEY_SCAN = 0x02;
    const uint8_t _CMD_SET_THR = 0x03;
    const uint8_t _CMD_GET_THR = 0x04;
    const uint8_t _CMD_SET_SLEEPEN = 0x05;
    const uint8_t _CMD_GET_SLEEPEN = 0x06;
    uint8_t _tx_buf[10] = {0};
    uint8_t _rx_buf[10] = {0};
    uint8_t _SumOfModules = 1;
    TwoWire *_wire = NULL;
    uint8_t _i2caddr;
    uint8_t _intPin;
    bool writeBytes(uint8_t id, uint8_t len, uint8_t *par);
    bool readBytes(uint8_t len, uint8_t *buffer);
};

#endif
