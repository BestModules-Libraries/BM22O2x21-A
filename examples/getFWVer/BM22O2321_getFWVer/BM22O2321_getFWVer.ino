/*****************************************************************
File:        getFWVer.ino
Description:  write data to the module set ID. And get the FW version.
Note: 
******************************************************************/

#include <bm22O2x21-a.h>

BM22O2321_A BM22O2321_A;//define object, use HW Serial1 on BMduino

// the setup function runs once when you press reset or power the board
void setup() {

  Serial.begin(115200);
   BM22O2321_A.begin();  

 if( BM22O2321_A.setEID(1)== SUCCESS)
 {
  Serial.println("setting EID SUCCESS");
  }
  else
  {
    Serial.println("setting EID FAIL"); 
  }
 delay(100);

 }

// the loop function runs over and over again forever
void loop() {
    Serial.print("BM22O2321_A FW version:");
    Serial.println(BM22O2321_A.getFWVer(1),HEX);//Read Standard Digital Servo Version
    delay(5000);
}
