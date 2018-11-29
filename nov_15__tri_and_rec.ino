#include<SoftwareSerial.h>

// pin assignments for arduino to roomba 7 mini-din port

int rxPin = 10;        // from arduino(10) to roomba pin(4) white wire
int txPin = 11;       // to arduino(11) from roomba pin(3) red wire
int ddPin = 5;        // device detect used to wake up roomba

char inByte = 0;       // sensro data read from serial buffer
int i = 0;            // byte counter for reading sensor data
SoftwareSerial mySerial(rxPin, txPin);  // new port with 10rx and 11tx
                                  // this means pin 4 on roomba connects to pin 10 on arduino
                                  // pin 3 on roomba connects to pin 11 on arduino                      // from arduino(5) to roomba pin(5) yellow wire
void setup() {
  Serial.begin(9600);    // start the serial port for the serial monitor
  Serial.println("hello dave");      // print a message to the monitor

  pinMode (ddPin, OUTPUT);            // set ddPin(5) to output, that's from arduino to roomba
  
  mySerial.begin(19200);       // start software serial for communicating with roomba
                                // use software serial because roomba's tx doesn't always
                                // carry enough voltage to meet arduino's requirements
  delay(10);              // set a short delay to allow the command to propegate 
 
 // check comminication between Arduino and roomba 
  if (mySerial.available())   //First order of business: listen to Roomba
    Serial.println(mySerial.read());   //writes to USB input from soft serial if connected to lapt  

// wake up roomba with a series of pulses to the dd pin on the roomba  
  digitalWrite(ddPin, HIGH);
  delay(100);
  digitalWrite(ddPin, LOW);
  delay(500);
  digitalWrite(ddPin, HIGH);
  delay(2000);
  Serial.println("i'm awake.");

// put roomba into passive mode

  mySerial.write(128);       // put roomba into passive mode
                              // clean light on
  delay(10);                // wait for mode change to propegate
  
  mySerial.write(131);      // put roomba into safe mode                           
  delay(10);
   
  int r;
  int l;
  
//rectangle 2m x 1.5m

  r = 300;
  l = 300;
  
  driveWheels(r,l);
  delay(6666);
  stopDrive();
  delay(10);

//  r = 300;
//  l = 10;
//  
//  driveWheels(r,l);
//  delay(1300);
//  stopDrive();
//  delay(10);
//
//  r = 300;
//  l = 300;
//  
//  driveWheels(r,l);
//  delay(5000);
//  stopDrive();
//  delay(10);
//
//  r = 300;
//  l = 10;
//  
//  driveWheels(r,l);
//  delay(1300);
//  stopDrive();
//  delay(10);
//
//  r = 300;
//  l = 300;
//  
//  driveWheels(r,l);
//  delay(6666);
//  stopDrive();
//  delay(10);
//  
//  r = 300;
//  l = 10;
//  
//  driveWheels(r,l);
//  delay(1300);
//  stopDrive();
//  delay(10);
//
//  r = 300;
//  l = 300;
//  
//  driveWheels(r,l);
//  delay(5000);
//  stopDrive();
//  delay(10);
//  
//// triangle
//
//  r = 300;
//  l = 300;
//  
//  driveWheels(r,l);
//  delay(2000);
//  stopDrive();
//  delay(10);
//   
//  r = 300;
//  l = 10;
//  
//  driveWheels(r,l);
//  delay(1833);
//  stopDrive();
//  delay(10);
//
//  r = 300;
//  l = 300;
//  
//  driveWheels(r,l);
//  delay(2000);
//  stopDrive();
//  delay(10);
//
//  r = 300;
//  l = 10;
//  
//  driveWheels(r,l);
//  delay(1833);
//  stopDrive();
//  delay(10);
//
//  r = 300;
//  l = 300;
//  
//  driveWheels(r,l);
//  delay(2000);
//  stopDrive();
//  delay(10);
//
//  //circle
//    int v;
//    int radi;
//    
//    v = 300;
//    radi = 750;
//    vroomvroom(v, radi);
//    delay(15700);
//  
}
void loop() {
 // put your main code here, to run repeatedly:
   Serial.write(142); // requests the OI to send a packet of
    // sensor data bytes
    Serial.write(9); // request cliff sensor value specifically
    delay(250); // poll sensor 4 times a second
    if (Serial.available() > 0) {
        inByte = Serial.read();
        if(inByte == '8')
        {
            goForward();
        }
        if(inByte == '5')
            stopDrive();
    }
    Serial.println(inByte);
} // end loop
 /* goForward sends the opcode 137 drive 
  *  the 4 bytes are
  *  velocity high byte, low byte
  *  radius high byte, low byte
  *  this op code doesn't drive the wheel motors directly
  */
 void goForward(void)
 {
  //Serial.println("in goForward");
    mySerial.write(byte(137));
    mySerial.write(byte(0));
    mySerial.write(byte(100));
    mySerial.write(byte(0x80));
    mySerial.write(byte (0));    // go forward
 } // end goForward

/***********************************************
 * direct drive wheel motors
 * 4 bytes are
 * right velocity high byte
 * right velocity low byte
 * left velocity high byte
 * left velocitn low byte
 * 
 * examples of both bitwise shift right and highByte, lowByte
 * to parse the argument into 2 bytes
 * 
 * constrain is used to ensure that the parameter values are within 
 * range for driving roomba's wheels
 */
void driveWheels(int right, int left)
{
  constrain(right, -500, 500);
  constrain(left, -500, 500);

  Serial.println("in driveWheels");
  
  mySerial.write(145);
//  mySerial.write(right >> 8);
//  mySerial.write(right);
//  mySerial.write(left >> 8);
//  mySerial.write(left);

  mySerial.write(highByte(right));
  mySerial.write(lowByte(right));
  mySerial.write(highByte(left));
  mySerial.write(lowByte(left));
 } // end drive wheels

 void vroomvroom(int velocity, int radius)
{
  constrain(velocity, -500, 500);
  constrain(radius, -2000, 2000);

  Serial.println("in drive");
  
  mySerial.write(137);
  mySerial.write(highByte(velocity));
  mySerial.write(lowByte(velocity));
  mySerial.write(highByte(radius));
  mySerial.write(lowByte(radius));
 } // end drive wheels
/*******************************************
 * stop roomba, send drive opcode (137) 
 * with all zero arguments
 ********************************************/
void stopDrive(void){
  mySerial.write(byte(137));
  mySerial.write(byte(0));
  mySerial.write(byte(0));
  mySerial.write(byte(0));
  mySerial.write(byte(0));  
} // end stopdrive()

/*******************************************
 * wake up roomba by pulsing dd pin 5 high low high
 ********************************************/
 void wakeUp (void)
{
  //setWarningLED(ON);
  //Serial.println("wake up, wake up.");
  digitalWrite(ddPin, HIGH);
  delay(100);
  digitalWrite(ddPin, LOW);
  delay(500);
  digitalWrite(ddPin, HIGH);
  delay(2000);
}
