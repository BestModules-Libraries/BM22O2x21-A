 /*************************************************
File:       		BM22O2x21_A.cpp
Author:         BESTMODULES
Description:    BM22O2321_A library for the lighting effect configuration
History：		    -
V1.0.0	 -- initial version；2023-02-14；Arduino IDE : v1.8.16

**************************************************/

#include "bm22O2x21-a.h"

 
 /*************************************************
Description:        Initialize a member variable
Parameters:         *theSerial : point of Serial(default : Serial1)
Return:             -
Others:             -
*************************************************/
BM22O2421_A::BM22O2421_A(HardwareSerial *theSerial) 
{
    _serial = theSerial;
    _mid = 0x10;
    _eid = 0;
}

/*************************************************
Description:        Enable the Serial port of the Module
Parameters:         dwBaudRate : Baud rate(default : 115200)
Return:             -
Others:             -
*************************************************/
void BM22O2421_A::begin(uint32_t baud)
{
   _serial->begin(baud, SERIAL_9N1);
}

/*************************************************
Description:       Set the Servo into standby mode
Parameters:        
Return:             -
Others:             -
*************************************************/
void BM22O2421_A::standby()
{
    writeCommand(CMD_STANDBY, 1);
    delay(10);
}

/*************************************************
Description:        Reset module
Parameters:         radio : Radio control, TRUE or FALSE(defaul)
                    EID: moudle EID
Return:             Communication status  0:Success 1:Fail   
Others:             -
*************************************************/
uint8_t BM22O2421_A::reset(uint8_t EID, bool radio)
{
    uint8_t buf[10];
    _eid = EID;
    writeCommand(CMD_RESET, radio);
    delay(10);
    if(radio) 
    {
        return SUCCESS;
    }
    else
    {
      if(readBytes(buf,4)==SUCCESS)
      {          
        if(buf[2]==0)
        {
          return SUCCESS;
        }
        else 
        {
          return FAIL;
        }        
      }
       else 
       {
        return FAIL;
       }
    }
}

/*************************************************
Description:        Sync action module, using with delay command
Parameters:         -
Return:             -
Others:             -
*************************************************/
void BM22O2421_A::syncaction()
{
    writeCommand(CMD_SYNCACTION, 1);
    delay(10);
}

/**********************************************************
Description:       Get the version information 
Parameters:        EID:module EID.
Return:            Communication status  0:fail version:version information   
Others:            -
**********************************************************/
uint16_t BM22O2421_A::getFWVer(uint8_t EID)
{
    uint8_t buf[10];
    uint16_t version;
	  _eid = EID;
    writeCommand(CMD_GETFWVER, 0);
    delay(10);
    if(readBytes(buf,6)==SUCCESS)
    {
       if(buf[2]==0x00)
      {     
       version = (buf[4] << 8) | buf[3];
       return version;
      }
      else
      {
         return 0; 
      }
    }
    else
    {
      return 0;  
    }
}
/*************************************************
Description:        Set up module _eid
Parameters:         EID : module EID
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM22O2421_A::setEID(uint8_t EID)
{
    uint8_t buf[10], eidTemp;
    eidTemp = _eid;
    _eid = EID;
    writeCommand(CMD_SETEID + (EID & 0x0f), 1);
    delay(10);
    if(readBytes(buf,4)==SUCCESS)
    {  
       if(buf[2]==0x00)
      {
       return SUCCESS;
      }
      else
      {
         return FAIL; 
      }
    }
    else
     {
         return FAIL; 
     }
    _eid = eidTemp;
}

/*************************************************
Description:        Servo Rotate Effect
Parameters:         EID : module eid
                    Rotating_Speed : -1000~10000 
                    Rotation_Time : 0~5000
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             0x00-success
                    0x01-read data fail
                    0x02-Low voltage protection
                    0x04-Locked rotor protection
                    0x08-Angle error
                    0x40-CS error
                    0x80-Non support command received  
Others:             -
*************************************************/
uint8_t BM22O2421_A::setRoate(uint8_t EID,int16_t Rotating_Speed, uint16_t Rotation_Time, bool syncaction)
{
    uint8_t buf[10];
	  _eid = EID;
    buf[0] = LE_SERVO_SETROTATE;
    if(syncaction) 
    {
        buf[0] += CMD_DELAY;
    }
    buf[1] = Rotating_Speed;
    buf[2] = Rotating_Speed >> 8;
    buf[3] = Rotation_Time;
    buf[4] = Rotation_Time >> 8;
    sendData(buf, 5);
    delay(10);
    if(readBytes(buf,4)==SUCCESS)
    {
      if(buf[2]==0x00)
      {
       return SUCCESS;
      }
      else
      {
         return buf[2]; 
      }
    }
    else
    {
      if(EID==0)
      {
        return SUCCESS;
      }
      else
      {
       return FAIL;        
      }
    }
}
/*************************************************
Description:        Servo Position Effect
Parameters:         EID : module eid
                    Position : 0~240 
                    Moving_Time : 0~5000
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             0x00-success
                    0x01-read data fail
                    0x02-Low voltage protection
                    0x04-Locked rotor protection
                    0x08-Angle error
                    0x40-CS error
                    0x80-Non support command received   
Others:             -
*************************************************/
uint8_t BM22O2421_A::setPosTime(uint8_t EID,int16_t Position, uint16_t Moving_Time,  bool syncaction)
{
    uint8_t buf[10];
	  _eid = EID;
    buf[0] = LE_SERVO_SETPOSTIME;
    if(syncaction) 
    {
        buf[0] += CMD_DELAY;
    }
    buf[1] = Position;
    buf[2] = Position >> 8;
    buf[3] = Moving_Time;
    buf[4] = Moving_Time >> 8;
    sendData(buf, 5);
    delay(10);
    if(readBytes(buf,4)==SUCCESS)
    { 
       if(buf[2]==0x00)
      {
       return SUCCESS;
      }
      else
      {
         return buf[2]; 
      }
    }
    else
    {
      if(EID==0)
      {
        return SUCCESS;
      }
      else
      {
       return FAIL;        
      } 
    }
}
/*************************************************
Description:        Sending system commands
Parameters:         cmd : command
                    radio : Radio control
Return:             -
Others:             -
*************************************************/
void BM22O2421_A::writeCommand(uint8_t cmd, bool radio)
{
    uint16_t mid;
    uint8_t Tlen_eid, instr, check_sum;
    uint16_t senddata[5];

    if(radio) 
    {
        mid = 0x100;
        Tlen_eid = 2<<4;
    }
    else 
    {
        mid = 0x100 + _mid;
        Tlen_eid = (2<<4) + (_eid & 0x0f);
    }
    instr = cmd;
    check_sum = ~(mid + Tlen_eid + instr);
    
    senddata[0]= mid;
    senddata[1]= Tlen_eid;
    senddata[2]= instr;
    senddata[3]= (uint8_t)check_sum;
    writeBytes(senddata,5);
}
/*************************************************
Description:        writeBytes
Parameters:         writedata : Data to be written.
                    len : Data length.
Return:             -
Others:             -
*************************************************/
void BM22O2421_A::writeBytes(uint16_t writedata[], uint8_t len)
{
     while(_serial->available() > 0)
    {
       _serial->read(); 
    }
    _serial->SetRxStatus(DISABLE);
    _serial->SetTxStatus(ENABLE);
    for(uint8_t i =0;i<len; i++)
    {
      _serial->write(writedata[i]); 
    }
    _serial->flush();
    _serial->SetTxStatus(DISABLE);
    _serial->SetRxStatus(ENABLE);
}

/*************************************************
Description:        Configuration of Servo effect
Parameters:         buf[] : point of Servo effect command 
                    len : Length of Servo effect command
Return:             -
Others:             -
*************************************************/
void BM22O2421_A::sendData(uint8_t buf[], uint8_t len)
{
    uint16_t mid;
    uint8_t i,y=0, Tlen_eid, TLen1, check_sum;
    uint16_t senddata[255];
  
    mid = 0x100 + _mid;
    if(len > 14)
    {
        Tlen_eid = (_eid & 0x0f);
        TLen1 = len + 1;
    }
    else
    {
        Tlen_eid = ((len + 1)<<4) + (_eid & 0x0f);
        TLen1 = 0;
    }
    check_sum = mid + Tlen_eid + TLen1;
    senddata[y] = mid;
    y++;
    senddata[y] = Tlen_eid;
    y++;
    if(TLen1) 
    {
        senddata[y] = TLen1;
         y++;
    }
    for(i = 0; i < len; i++)
    {
      senddata[y]=buf[i];
      y++;
      check_sum += buf[i];
    }
    senddata[y] = (uint8_t)~(check_sum);
    writeBytes(senddata,y+1);
}

/**********************************************************
Description: get response
Parameters:       rBuf[] : Variables for storing buf data
                  rlen:The length of the data where the data is stored
                  waitTime:Timeout
Return:      Read status  0:Success 1:Fail 7:Timeout error    
Others:      
**********************************************************/
 uint8_t BM22O2421_A::readBytes(uint8_t rBuf[],uint8_t rlen,uint8_t waitTime)
 {
    uint8_t cnt = 0;
    uint8_t i ,delayCnt =0;
    uint8_t check = 0;
    for (cnt = 0; cnt < rlen; cnt++)
    {
      while (_serial->available() == 0)
      {
        delay(1);
        delayCnt++;
        if (delayCnt > waitTime)
        {
          return Timeout; // Timeout error
        }
      }
     rBuf[cnt] = _serial->read(); 
    }
    
    for(i = 0; i < cnt - 1; i++)
    {
        check += rBuf[i];
    }
    if((check) == (uint8_t)(~rBuf[cnt-1]))
    {
        if(((rBuf[0] & 0xff) == _mid)&&((rBuf[1] & 0x0f) == _eid))
        {                        
				return SUCCESS;
        }
           
	      else  
	      {
	        return FAIL;
	      }		                
    }
    else
    { 
       return FAIL; 
    }

 }
/*************************************************
Description:        Initialize a member variable
Parameters:              *theSerial : point of Serial(default : Serial1)
Return:             -
Others:             -
*************************************************/
BM22O2321_A::BM22O2321_A(HardwareSerial *theSerial) 
{
    _serial = theSerial;
    _mid = 0x1E;
    _eid = 0;
}

/*************************************************
Description:        Enable the Serial port of the Module
Parameters:         baud : Baud rate(default : 115200)
Return:             -
Others:             -
*************************************************/
void BM22O2321_A::begin(uint32_t baud)
{
   _serial->begin(baud, SERIAL_9N1);
}
/*************************************************
Description:       Set the Servo into standby mode
Parameters:         -
Return:             -
Others:             -
*************************************************/
void BM22O2321_A::standby()
{
    writeCommand(CMD_STANDBY, 1);
    delay(10);
}

/*************************************************
Description:        Reset module
Parameters:         
                    EID: moudle eid.
                    radio : Radio control, TRUE or FALSE(defaul)
Return:             Communication status  0:Success 1:Fail   
Others:             -
*************************************************/
uint8_t BM22O2321_A::reset(uint8_t EID, bool radio)
{
    uint8_t buf[10];
	  _eid = EID;
    writeCommand(CMD_RESET, radio);
    delay(10);
    if(radio) 
    {
        return SUCCESS;
    }
    else
    {
      if(readBytes(buf,4)==SUCCESS)
      {          
        if(buf[2]==0)
        {
          return SUCCESS;
        }
        else 
        {
          return FAIL;
        }        
      }
       else 
       {
        return FAIL;
       }
    }
}

/*************************************************
Description:        Sync action module, using with delay command
Parameters:         -
Return:             -
Others:             -
*************************************************/
void BM22O2321_A::syncaction()
{
    writeCommand(CMD_SYNCACTION, 1);
    delay(10);
}

/**********************************************************
Description:       Get the version information 
Parameters:        EID: moudle eid.
Return:            Communication status  0:Success 1:Fail   
Others:            -
**********************************************************/
uint16_t BM22O2321_A::getFWVer(uint8_t EID)
{
    uint8_t buf[10];
    uint16_t version;
	  _eid = EID;
    writeCommand(CMD_GETFWVER, 0);
    delay(10);
    if(readBytes(buf,6)==SUCCESS)
    {
       if(buf[2]==0x00)
      {     
       version = (buf[4] << 8) | buf[3];
       return version;
      }
      else
      {
         return 0; 
      }
    }
    else
    {
      return 0;  
    }
}
/*************************************************
Description:        Set up module _eid
Parameters:             EID: moudle eid.
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM22O2321_A::setEID(uint8_t EID)
{
    uint8_t buf[10], eidTemp;
    bool ret;

    eidTemp = _eid;
    _eid = EID;
    writeCommand(CMD_SETEID + (EID & 0x0f), 1);
    if(readBytes(buf,4)==SUCCESS)
    {  
       if(buf[2]==0x00)
      {
       return SUCCESS;
      }
      else
      {
         return FAIL; 
      }
    }
    else
     {
         return FAIL; 
     }
    _eid = eidTemp;
}

/*************************************************
Description:        Servo Rotate Effect
Parameters:              
        EID: moudle eid.
        Rotating_Speed : -1000~10000 
        Rotation_Time : 0~5000
         syncaction : delayed operation control, TRUE or FALSE(default)
Return:  	    0x00-success
                    0x01-read data fail
                    0x02-Low voltage protection
                    0x04-Locked rotor protection
                    0x08-Angle error
                    0x40-CS error
                    0x80-Non support command received    
Others:             -
*************************************************/
uint8_t BM22O2321_A::setRoate(uint8_t EID,int16_t Rotating_Speed, uint16_t Rotation_Time, bool syncaction)
{
    uint8_t buf[10];
	_eid = EID;
    buf[0] = LE_SERVO_SETROTATE;
    if(syncaction) 
    {
        buf[0] += CMD_DELAY;
    }
    buf[1] = Rotating_Speed;
    buf[2] = Rotating_Speed >> 8;
    buf[3] = Rotation_Time;
    buf[4] = Rotation_Time >> 8;
    sendData(buf, 5);
    delay(10);
    if(readBytes(buf,4)==SUCCESS)
    {
      if(buf[2]==0x00)
      {
       return SUCCESS;
      }
      else
      {
         return buf[2]; 
      }
    }
    else
    {
      if(EID==0)
      {
        return SUCCESS;
      }
      else
      {
       return FAIL;        
      }
    }
}
/*************************************************
Description:        Servo Position Effect
Parameters:              
                     EID: moudle eid.
                     Position : 0~240 
                    Moving_Time : 0~5000
                     syncaction : delayed operation control, TRUE or FALSE(default)
Return:             0x00-success
                    0x01-read data fail
                    0x02-Low voltage protection
                    0x04-Locked rotor protection
                    0x08-Angle error
                    0x40-CS error
                    0x80-Non support command received    
Others:             -
*************************************************/
uint8_t BM22O2321_A::setPosTime(uint8_t EID,int16_t Position, uint16_t Moving_Time, bool syncaction)
{
    uint8_t buf[10];
	_eid = EID;
    buf[0] = LE_SERVO_SETPOSTIME;
    if(syncaction) 
    {
        buf[0] += CMD_DELAY;
    }
    buf[1] = Position;
    buf[2] = Position >> 8;
    buf[3] = Moving_Time;
    buf[4] = Moving_Time >> 8;
    sendData(buf, 5);
    delay(10);    
   if(readBytes(buf,4)==SUCCESS)
    { 
       if(buf[2]==0x00)
      {
       return SUCCESS;
      }
      else
      {
         return buf[2]; 
      }
    }
    else
    {
      if(EID==0)
      {
        return SUCCESS;
      }
      else
      {
       return FAIL;        
      } 
    }
}
/*************************************************
Description:        Sending system commands
Parameters:         cmd : command
                    radio : Radio control
Return:             -
Others:             -
*************************************************/
void BM22O2321_A::writeCommand(uint8_t cmd, bool radio)
{
    uint16_t mid;
    uint8_t Tlen_eid, instr, check_sum;
    uint16_t senddata[5];

    if(radio) 
    {
        mid = 0x100;
        Tlen_eid = 2<<4;
    }
    else 
    {
        mid = 0x100 + _mid;
        Tlen_eid = (2<<4) + (_eid & 0x0f);
    }
    instr = cmd;
    check_sum = ~(mid + Tlen_eid + instr);
    senddata[0]= mid;
    senddata[1]= Tlen_eid;
    senddata[2]= instr;
    senddata[3]= (uint8_t)check_sum;
    writeBytes(senddata,5);
}
/*************************************************
Description:        writeBytes
Parameters:         writedata[] : Data to be written.
                    len : Data length.
Return:             -
Others:             -
*************************************************/
void BM22O2321_A::writeBytes(uint16_t writedata[], uint8_t len)
{
     while(_serial->available() > 0)
    {
       _serial->read(); 
    }
    _serial->SetRxStatus(DISABLE);
    _serial->SetTxStatus(ENABLE);
    for(uint8_t i =0;i<len; i++)
    {
      _serial->write(writedata[i]); 
    }
    _serial->flush();
    _serial->SetTxStatus(DISABLE);
    _serial->SetRxStatus(ENABLE);
}
/*************************************************
Description:        Configuration of Servo effect
Parameters:         buf[] : point of Servo effect command 
                    len : Length of Servo effect command
Return:             -
Others:             -
*************************************************/
void BM22O2321_A::sendData(uint8_t buf[], uint8_t len)
{
    uint16_t mid;
    uint8_t i,y=0, Tlen_eid, TLen1, check_sum;
    uint16_t senddata[255];

    mid = 0x100 + _mid;
    if(len > 14)
    {
        Tlen_eid = (_eid & 0x0f);
        TLen1 = len + 1;
    }
    else
    {
        Tlen_eid = ((len + 1)<<4) + (_eid & 0x0f);
        TLen1 = 0;
    }
    check_sum = mid + Tlen_eid + TLen1;
    senddata[y] = mid;
    y++;
    senddata[y] = Tlen_eid;
    y++;
    if(TLen1) 
    {
        _serial->write(TLen1);
        y++;
    }
    for(i = 0; i < len; i++)
    {
      senddata[y]=buf[i];
      y++;
      check_sum += buf[i];
    }
    senddata[y] = (uint8_t)~(check_sum);
    writeBytes(senddata,y+1);
}

/**********************************************************
Description:  get response
Parameters:   rBuf[] : Variables for storing buf data
              rlen:The length of the data where the data is stored
              waitTime:Timeout
Return:       Read status  0:Success 1:Fail 7:Timeout error      
Others:      
**********************************************************/
 uint8_t BM22O2321_A::readBytes(uint8_t rBuf[],uint8_t rlen,uint8_t waitTime)
 {
    uint8_t cnt = 0;
    uint8_t i ,delayCnt =0;
    uint8_t check = 0;
    for (cnt = 0; cnt < rlen; cnt++)
    {
      while (_serial->available() == 0)
      {
        delay(1);
        delayCnt++;
        if (delayCnt > waitTime)
        {
          return Timeout; // Timeout error
        }
      }
     rBuf[cnt] = _serial->read(); 
    }
    
    for(i = 0; i < cnt - 1; i++)
    {
        check += rBuf[i];
    }
    if((check) == (uint8_t)(~rBuf[cnt-1]))
    {
        if(((rBuf[0] & 0xff) == _mid)&&((rBuf[1] & 0x0f) == _eid))
        {                        
        return SUCCESS;
        }
           
        else  
        {
          return FAIL;
        }                   
    }
    else
    { 
       return FAIL; 
    }
 }
/*************************************************
Description:        Initialize a member variable
Parameters:         *theSerial : point of Serial(default : Serial1)
Return:             -
Others:             -
*************************************************/
BM22O2221_A::BM22O2221_A(HardwareSerial *theSerial) 
{
    _serial = theSerial;
    _mid = 0x25;
    _eid = 0;
}

/*************************************************
Description:        Enable the Serial port of the Module
Parameters:         baud : Baud rate(default : 115200)
Return:             -
Others:             -
*************************************************/
void BM22O2221_A::begin(uint32_t baud)
{
   _serial->begin(baud, SERIAL_9N1);
}

/*************************************************
Description:       Set the Servo into standby mode
Parameters:         -
Return:             -
Others:             -
*************************************************/
void BM22O2221_A::standby()
{
    writeCommand(CMD_STANDBY, 1);
    delay(10);
}
/*************************************************
Description:        Reset module
Parameters:         EID:moudle eid.
                    radio : Radio control, TRUE or FALSE(defaul)
Return:             Communication status  0:Success 1:Fail   
Others:             -
*************************************************/
uint8_t BM22O2221_A::reset(uint8_t EID, bool radio)
{
   uint8_t buf[10];
    _eid = EID;
    writeCommand(CMD_RESET, radio);
    delay(10);
    if(radio) 
    {
        return SUCCESS;
    }
    else
    {
      if(readBytes(buf,4)==SUCCESS)
      {          
        if(buf[2]==0)
        {
          return SUCCESS;
        }
        else 
        {
          return FAIL;
        }        
      }
       else 
       {
        return FAIL;
       }
    }
}

/*************************************************
Description:        Sync action module, using with delay command
Parameters:         -
Return:             -
Others:             -
*************************************************/
void BM22O2221_A::syncaction()
{
    writeCommand(CMD_SYNCACTION, 1);
    delay(10);
}

/**********************************************************
Description:       Get the version information 
Parameters:        EID:moudle eid.
Return:            version information  
Others:            -
**********************************************************/
uint16_t BM22O2221_A::getFWVer(uint8_t EID)
{
    uint8_t buf[10];
    uint16_t version;
    _eid = EID;
    writeCommand(CMD_GETFWVER, 0);
    delay(10);
    if(readBytes(buf,6)==SUCCESS)
    {
       if(buf[2]==0x00)
      {     
       version = (buf[4] << 8) | buf[3];
       return version;
      }
      else
      {
         return 0; 
      }
    }
    else
    {
      return 0;  
    }
}
/*************************************************
Description:        Set up module _eid
Parameters:         EID : module eid
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM22O2221_A::setEID(uint8_t EID)
{
    uint8_t buf[10], eidTemp;
    eidTemp = _eid;
    _eid = EID;
    writeCommand(CMD_SETEID + (EID & 0x0f), 1);
    delay(10);
    if(readBytes(buf,4)==SUCCESS)
    {  
       if(buf[2]==0x00)
      {
       return SUCCESS;
      }
      else
      {
         return FAIL; 
      }
    }
    else
     {
         return FAIL; 
     }
    _eid = eidTemp;
}
/*************************************************
Description:        Servo Position Effect
Parameters:         EID : module _eid
                    Position : 0~240 
                    Moving_Time : 0~5000
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             0x00-success
                    0x01-read data fail
                    0x02-Low voltage protection
                    0x04-Locked rotor protection
                    0x08-Angle error
                    0x40-CS error
                    0x80-Non support command received    
Others:             -
*************************************************/
uint8_t BM22O2221_A::setPosTime(uint8_t EID,int16_t Position, uint16_t Moving_Time, bool syncaction)
{
    uint8_t buf[10];
	  _eid = EID;
    buf[0] = LE_SERVO_SETPOSTIME;
    if(syncaction) 
    {
        buf[0] += CMD_DELAY;
    }
    buf[1] = Position;
    buf[2] = Position >> 8;
    buf[3] = Moving_Time;
    buf[4] = Moving_Time >> 8;
    sendData(buf, 5);
    delay(10);
    if(readBytes(buf,4)==SUCCESS)
    { 
       if(buf[2]==0x00)
      {
       return SUCCESS;
      }
      else
      {
         return buf[2]; 
      }
    }
    else
    {
      if(EID==0)
      {
        return SUCCESS;
      }
      else
      {
       return FAIL;        
      } 
    }
}
/*************************************************
Description:        Sending system commands
Parameters:         cmd : command
                    radio : Radio control
Return:             -
Others:             -
*************************************************/
void BM22O2221_A::writeCommand(uint8_t cmd, bool radio)
{
    uint16_t mid;
    uint8_t Tlen_eid, instr, check_sum;
    uint16_t senddata[5];

    if(radio) 
    {
        mid = 0x100;
        Tlen_eid = 2<<4;
    }
    else 
    {
        mid = 0x100 + _mid;
        Tlen_eid = (2<<4) + (_eid & 0x0f);
    }
    instr = cmd;
    check_sum = ~(mid + Tlen_eid + instr);

    senddata[0]= mid;
    senddata[1]= Tlen_eid;
    senddata[2]= instr;
    senddata[3]= (uint8_t)check_sum;
    writeBytes(senddata,5);
}

/*************************************************
Description:        writeBytes
Parameters:         writedata[] : Data to be written.
                    len : Data length.
Return:             -
Others:             -
*************************************************/
void BM22O2221_A::writeBytes(uint16_t writedata[], uint8_t len)
{
     while(_serial->available() > 0)
    {
       _serial->read(); 
    }
    _serial->SetRxStatus(DISABLE);
    _serial->SetTxStatus(ENABLE);
    for(uint8_t i =0;i<len; i++)
    {
      _serial->write(writedata[i]); 
    }
    _serial->flush();
    _serial->SetTxStatus(DISABLE);
    _serial->SetRxStatus(ENABLE);
}
/*************************************************
Description:        Configuration of Servo effect
Parameters:         buf[]: point of Servo effect command 
                    len : Length of Servo effect comman
Return:             -
Others:             -
*************************************************/
void BM22O2221_A::sendData(uint8_t buf[], uint8_t len)
{
    uint16_t mid;
    uint8_t i,y=0, Tlen_eid, TLen1, check_sum;
    uint16_t senddata[255];

    mid = 0x100 + _mid;
    if(len > 14)
    {
        Tlen_eid = (_eid & 0x0f);
        TLen1 = len + 1;
    }
    else
    {
        Tlen_eid = ((len + 1)<<4) + (_eid & 0x0f);
        TLen1 = 0;
    }
    check_sum = mid + Tlen_eid + TLen1;

    senddata[y] = mid;
    y++;
    senddata[y] = Tlen_eid;
    y++;
    
    if(TLen1) 
    {
        senddata[y] = TLen1;
         y++;
    }
    for(i = 0; i < len; i++)
    {
      senddata[y]=buf[i];
      y++;
      check_sum += buf[i];
    }

    senddata[y] = (uint8_t)~(check_sum);
    writeBytes(senddata,y+1);
}

/**********************************************************
Description: get response
Parameters:       rBuf[] : Variables for storing buf data
                  rlen:The length of the data where the data is stored
                  waitTime:Timeout
Return:      Read status  0:Success 1:Fail 7:Timeout error    
Others:      
**********************************************************/
 uint8_t BM22O2221_A::readBytes(uint8_t rBuf[],uint8_t rlen,uint8_t waitTime)
 {
    uint8_t cnt = 0;
    uint8_t i ,delayCnt =0;
    uint8_t check = 0;
    for (cnt = 0; cnt < rlen; cnt++)
    {
      while (_serial->available() == 0)
      {
        delay(1);
        delayCnt++;
        if (delayCnt > waitTime)
        {
          return Timeout; // Timeout error
        }
      }
     rBuf[cnt] = _serial->read(); 
    }
    
    for(i = 0; i < cnt - 1; i++)
    {
        check += rBuf[i];
    }
    if((check) == (uint8_t)(~rBuf[cnt-1]))
    {
        if(((rBuf[0] & 0xff) == _mid)&&((rBuf[1] & 0x0f) == _eid))
        {                        
        return SUCCESS;
        }
           
        else  
        {
          return FAIL;
        }                   
    }
    else
    { 
       return FAIL; 
    }

 }
/*************************************************
Description:        Initialize a member variable
Parameters:         *theSerial : point of Serial(default : Serial1)
Return:             -
Others:             -
*************************************************/
BM22O2121_A::BM22O2121_A(HardwareSerial *theSerial) 
{
    _serial = theSerial;
    _mid = 0x26;
    _eid = 0;
}

/*************************************************
Description:        Enable the Serial port of the Module
Parameters:         baud : Baud rate(default : 115200)
Return:             -
Others:             -
*************************************************/
void BM22O2121_A::begin(uint32_t baud)
{
   _serial->begin(baud, SERIAL_9N1);
}

/*************************************************
Description:       Set the Servo into standby mode
Parameters:         -
Return:             -
Others:             -
*************************************************/
void BM22O2121_A::standby()
{
    writeCommand(CMD_STANDBY, 1);
    delay(10);
}

/*************************************************
Description:        Reset module
Parameters:         radio : Radio control, TRUE or FALSE(defaul)
                    EID : module _eid
Return:             Communication status  0:Success 1:Fail   
Others:             -
*************************************************/
uint8_t BM22O2121_A::reset(uint8_t EID, bool radio)
{
    uint8_t buf[10];
    _eid = EID;
    writeCommand(CMD_RESET, radio);
    delay(10);
    if(radio) 
    {
        return SUCCESS;
    }
    else
    {
      if(readBytes(buf,4)==SUCCESS)
      {          
        if(buf[2]==0)
        {
          return SUCCESS;
        }
        else 
        {
          return FAIL;
        }        
      }
       else 
       {
        return FAIL;
       }
    }
}

/*************************************************
Description:        Sync action module, using with delay command
Parameters:         -
Return:             -
Others:             -
*************************************************/
void BM22O2121_A::syncaction()
{
    uint8_t buf[10];
//  _eid = EID;
    writeCommand(CMD_SYNCACTION, 1);
    readBytes(buf,4);
}

/**********************************************************
Description:       Get the version information 
Parameters:        EID : module _eid
Return:            Communication status  0:Success 1:Fail   
Others:            -
**********************************************************/
uint16_t BM22O2121_A::getFWVer(uint8_t EID)
{
     uint8_t buf[10];
    uint16_t version;
    _eid = EID;
    writeCommand(CMD_GETFWVER, 0);
    delay(10);
    if(readBytes(buf,6)==SUCCESS)
    {
       if(buf[2]==0x00)
      {     
       version = (buf[4] << 8) | buf[3];
       return version;
      }
      else
      {
         return 0; 
      }
    }
    else
    {
      return 0;  
    }
}
/*************************************************
Description:        Set up module _eid
Parameters:          EID : module _eid
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM22O2121_A::setEID(uint8_t EID)
{
    uint8_t buf[10], eidTemp;
    eidTemp = _eid;
    _eid = EID;
    writeCommand(CMD_SETEID + (EID & 0x0f), 1);
    delay(10);
    if(readBytes(buf,4)==SUCCESS)
    {  
       if(buf[2]==0x00)
      {
       return SUCCESS;
      }
      else
      {
         return FAIL; 
      }
    }
    else
     {
         return FAIL; 
     }
    _eid = eidTemp;
}
/*************************************************
Description:        Servo Position Effect
Parameters:         Position : 0~240 
                    Moving_Time : 0~5000
                    syncaction : delayed operation control, TRUE or FALSE(default)
                    EID : module _eid
Return:             0x00-success
                    0x01-read data fail
                    0x02-Low voltage protection
                    0x04-Locked rotor protection
                    0x08-Angle error
                    0x40-CS error
                    0x80-Non support command received    
Others:             -
*************************************************/
uint8_t BM22O2121_A::setPosTime( uint8_t EID,int16_t Position, uint16_t Moving_Time, bool syncaction)
{
    uint8_t buf[10];
	_eid = EID;
    buf[0] = LE_SERVO_SETPOSTIME;
    if(syncaction) 
    {
        buf[0] += CMD_DELAY;
    }
    buf[1] = Position;
    buf[2] = Position >> 8;
    buf[3] = Moving_Time;
    buf[4] = Moving_Time >> 8;
    sendData(buf, 5);
    delay(10);
    if(readBytes(buf,4)==SUCCESS)
    { 
       if(buf[2]==0x00)
      {
       return SUCCESS;
      }
      else
      {
         return buf[2]; 
      }
    }
    else
    {
      if(EID==0)
      {
        return SUCCESS;
      }
      else
      {
       return FAIL;        
      } 
    }
}
/*************************************************
Description:        Sending system commands
Parameters:         cmd : command
                    radio : Radio control
Return:             -
Others:             -
*************************************************/
void BM22O2121_A::writeCommand(uint8_t cmd, bool radio)
{
    uint16_t mid;
    uint8_t Tlen_eid, instr, check_sum;
    uint16_t senddata[5];
    _serial->SetRxStatus(DISABLE);
    _serial->SetTxStatus(ENABLE);
    while(_serial->available() > 0)
    {
       _serial->read(); 
    }

    if(radio) 
    {
        mid = 0x100;
        Tlen_eid = 2<<4;
    }
    else 
    {
        mid = 0x100 + _mid;
        Tlen_eid = (2<<4) + (_eid & 0x0f);
    }
    instr = cmd;
    check_sum = ~(mid + Tlen_eid + instr);
    _serial->write(mid);
    _serial->write(Tlen_eid);
    _serial->write(instr);
    _serial->write(check_sum);

    senddata[0]= mid;
    senddata[1]= Tlen_eid;
    senddata[2]= instr;
    senddata[3]= (uint8_t)check_sum;
    writeBytes(senddata,5);
}

/*************************************************
Description:        writeBytes
Parameters:         writedata : Data to be written.
                    len : Data length.
Return:             -
Others:             -
*************************************************/
void BM22O2121_A::writeBytes(uint16_t writedata[], uint8_t len)
{
     while(_serial->available() > 0)
    {
       _serial->read(); 
    }
    _serial->SetRxStatus(DISABLE);
    _serial->SetTxStatus(ENABLE);
    for(uint8_t i =0;i<len; i++)
    {
      _serial->write(writedata[i]); 
    }
    _serial->flush();
    _serial->SetTxStatus(DISABLE);
    _serial->SetRxStatus(ENABLE);
}

/*************************************************
Description:        Configuration of Servo effect
Parameters:         *buf : point of Servo effect command 
                    len : Length of Servo effect command
Return:             -
Others:             -
*************************************************/
void BM22O2121_A::sendData(uint8_t buf[], uint8_t len)
{
     uint16_t mid;
    uint8_t i,y=0, Tlen_eid, TLen1, check_sum;
    uint16_t senddata[255];
  
    mid = 0x100 + _mid;
    if(len > 14)
    {
        Tlen_eid = (_eid & 0x0f);
        TLen1 = len + 1;
    }
    else
    {
        Tlen_eid = ((len + 1)<<4) + (_eid & 0x0f);
        TLen1 = 0;
    }
    check_sum = mid + Tlen_eid + TLen1;
    senddata[y] = mid;
    y++;
    senddata[y] = Tlen_eid;
    y++;
    if(TLen1) 
    {
        senddata[y] = TLen1;
         y++;
    }
    for(i = 0; i < len; i++)
    {
      senddata[y]=buf[i];
      y++;
      check_sum += buf[i];
    }
    senddata[y] = (uint8_t)~(check_sum);
    writeBytes(senddata,y+1);
}


/**********************************************************
Description: get response
Parameters:       rBuf : Variables for storing buf data
                  rlen:The length of the data where the data is stored
                  waitTime:Timeout
Return:      Read status  0:Success 1:Fail 7:Timeout error    
Others:      
**********************************************************/
 uint8_t BM22O2121_A::readBytes(uint8_t rBuf[],uint8_t rlen,uint8_t waitTime)
 {
    uint8_t cnt = 0;
    uint8_t i ,delayCnt =0;
    uint8_t check = 0;
    for (cnt = 0; cnt < rlen; cnt++)
    {
      while (_serial->available() == 0)
      {
        delay(1);
        delayCnt++;
        if (delayCnt > waitTime)
        {
          return Timeout; // Timeout error
        }
      }
     rBuf[cnt] = _serial->read(); 
    }
    
    for(i = 0; i < cnt - 1; i++)
    {
        check += rBuf[i];
    }
    if((check) == (uint8_t)(~rBuf[cnt-1]))
    {
        if(((rBuf[0] & 0xff) == _mid)&&((rBuf[1] & 0x0f) == _eid))
        {                        
        return SUCCESS;
        }
           
        else  
        {
          return FAIL;
        }                   
    }
    else
    { 
       return FAIL; 
    }

 }
