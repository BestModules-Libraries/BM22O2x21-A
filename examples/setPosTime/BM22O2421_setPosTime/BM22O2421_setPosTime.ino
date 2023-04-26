/*****************************************************************
File:         SetRotate.ino
Description:  Control the Servo to rotate 0 degrees->120 degrees->240 degreeswith time
Note: 
******************************************************************/
#include <bm22O2x21-a.h>


BM22O2421_A BM22O2421_A;              //define object, use HW Serial1 on BMduino
#define    EID      1
uint8_t ServoStatus;
// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  BM22O2421_A.begin();
}
// the loop function runs over and over again forever
void loop() {
   ServoStatus=BM22O2421_A.setPosTime(EID,0, 0);//Set Standard Digital Servo Position and Time
    StatusPrint();
    delay(5000);
    ServoStatus=BM22O2421_A.setPosTime(EID,120, 0);
    StatusPrint();
    delay(5000);
    ServoStatus=BM22O2421_A.setPosTime(EID,240, 0);
    StatusPrint();
    delay(5000);
}
void StatusPrint(void)
{
  if(ServoStatus == SUCCESS)
    {
      Serial.println("Setting success");
    }
    else if(ServoStatus == 0x02)
    {
      Serial.println("Low Voltage");
    }
    else if(ServoStatus == 0x04)
    {
      Serial.println("Stall Error");
    }
    else if(ServoStatus == 0x08)
    {
      Serial.println("Angle Error");
    }
    else if(ServoStatus == 0x40)
    {
      Serial.println("CheckSum Error");
    }
    else if(ServoStatus == 0x80)
    {
      Serial.println("Command Notsupport");
    }
    else
    {
      Serial.println("Setting fail");
    }
}
