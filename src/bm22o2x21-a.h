/*************************************************
File:       		BM22O2x21_A.h
Author:         BESTMODULES
Description:    BM32O2031-A library for the light efficiency configuration
History：		    -
V1.0.0	 -- initial version；2023-02-14；Arduino IDE : v1.8.16

**************************************************/

#ifndef BM22O2X21_A_h
#define BM22O2X21_A_h

#include "Arduino.h"

#define   BM22O2X21_A_BAUD  115200
#define   SUCCESS           0
#define   FAIL              1
#define   Timeout           7
#define	  LOWVOLTAGE		2
#define	  STALL				3
#define	  ANGLE				4
#define	  CHECKSUM			5
#define	  NOTSUPPORT	    6
typedef enum 
{
    CMD_RESET = 0x00,
	CMD_STANDBY = 0x01,
    CMD_SYNCACTION = 0x02,
    CMD_GETFWVER = 0x03,
    CMD_DELAY = 0x20,
    CMD_SETEID = 0x80,

    LE_SERVO_SETROTATE = 0x08,
    LE_SERVO_SETPOSTIME,
}COMMAND_E;

class BM22O2421_A
{
public:
    BM22O2421_A(HardwareSerial *theSerial = &Serial1);
	
    void begin(uint32_t baud = BM22O2X21_A_BAUD);
    void standby();
    uint8_t reset(uint8_t EID, bool radio = FALSE);
    void syncaction();
    uint16_t getFWVer( uint8_t EID);	
    uint8_t setEID(uint8_t EID);

    uint8_t setRoate( uint8_t EID ,int16_t Rotating_Speed, uint16_t Rotation_Time, bool syncaction = FALSE);
	uint8_t setPosTime(uint8_t EID,int16_t Position, uint16_t Moving_Time,  bool syncaction = FALSE);
private:
    uint8_t _mid;
    uint8_t _eid;

    HardwareSerial *_serial = NULL;
    void writeBytes(uint16_t writedata[], uint8_t len);
    void writeCommand(uint8_t cmd, bool radio);
    void sendData(uint8_t buf[], uint8_t len);
    uint8_t readBytes(uint8_t rBuf[],uint8_t rlen,uint8_t waitTime = 3);	
};

class BM22O2321_A
{
public:
    BM22O2321_A(HardwareSerial *theSerial = &Serial1);
	
    void begin(uint32_t baud = BM22O2X21_A_BAUD);
    void standby();
    uint8_t reset(uint8_t EID, bool radio = FALSE);
    void syncaction();
    uint16_t getFWVer( uint8_t EID);	
    uint8_t setEID(uint8_t EID);

    uint8_t setRoate(uint8_t EID ,int16_t Rotating_Speed, uint16_t Rotation_Time,  bool syncaction = FALSE);
	uint8_t setPosTime( uint8_t EID,int16_t Position, uint16_t Moving_Time, bool syncaction = FALSE);
private:
    uint8_t _mid;
    uint8_t _eid;

    HardwareSerial *_serial = NULL;
    void writeBytes(uint16_t writedata[], uint8_t len);
    void writeCommand(uint8_t cmd, bool radio);
    void sendData(uint8_t buf[], uint8_t len);
    uint8_t readBytes(uint8_t rBuf[],uint8_t rlen,uint8_t waitTime = 3);  
};

class BM22O2221_A
{
public:
    BM22O2221_A(HardwareSerial *theSerial = &Serial1);
	
    void begin(uint32_t baud = BM22O2X21_A_BAUD);
    void standby();
    uint8_t reset(uint8_t EID, bool radio = FALSE);
    void syncaction();
    uint16_t getFWVer(uint8_t EID);	
    uint8_t setEID(uint8_t EID);

	uint8_t setPosTime(uint8_t EID,int16_t Position, uint16_t Moving_Time,  bool syncaction = FALSE);
private:
    uint8_t _mid;
    uint8_t _eid;

    HardwareSerial *_serial = NULL;
    void writeBytes(uint16_t writedata[], uint8_t len);
    void writeCommand(uint8_t cmd, bool radio);
    void sendData(uint8_t buf[], uint8_t len);
    uint8_t readBytes(uint8_t rBuf[],uint8_t rlen,uint8_t waitTime=3);	
};

class BM22O2121_A
{
public:
    BM22O2121_A(HardwareSerial *theSerial = &Serial1);	
    	
    void begin(uint32_t baud = BM22O2X21_A_BAUD);
    void standby();
    uint8_t reset(uint8_t EID, bool radio = FALSE);
    void syncaction();
    uint16_t getFWVer(uint8_t EID);	
    uint8_t setEID(uint8_t EID);

	uint8_t setPosTime(uint8_t EID,int16_t Position, uint16_t Moving_Time,  bool syncaction = FALSE);
private:
    uint8_t _mid;
    uint8_t _eid;

    HardwareSerial *_serial = NULL;

    void writeCommand(uint8_t cmd, bool radio);
    void writeBytes(uint16_t writedata[], uint8_t len);
    void sendData(uint8_t buf[], uint8_t len);
    uint8_t readBytes(uint8_t rBuf[],uint8_t rlen,uint8_t waitTime=30);	
};
#endif
