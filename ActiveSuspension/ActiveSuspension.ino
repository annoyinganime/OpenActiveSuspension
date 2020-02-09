#include <Wire.h>
#include <Servo.h>

#define SCALE 10 // servo "degrees" per newton
#define SERVO_MIN 45
#define SERVO_MAX 135
#define SERVO_MID 96

int d[4]; //for storing angles in "degrees"
Servo fl, fr, rl, rr; //front-left, front-right, rear-left, rear-right
void setup() {
  Wire.begin();
  w(0x6b, 0x00); //disable mpu6050 sleep
  w(0x1c, 0b00001000); //also change gc if changing this
  w(0x1a, 0b00000110); //filter setup
  
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  rr.attach(2);
  fr.attach(3);
  rl.attach(4);
  fl.attach(5);
 
  rr.write(SERVO_MID);
  delay(200);
  fr.write(SERVO_MID);
  delay(200);
  fl.write(SERVO_MID);
  delay(200);
  rl.write(SERVO_MID);
  delay(500);
  for (int i = 0; i<4; i++){
    d[i]=SERVO_MID;
    }
}
double acx, acy, acz; 

void loop() {
  // 3b- x, 3d- y, 3f- z (probably unneeded)      rr, fr, rl, fl
  acx = gc(0x3b); //right-left, right-positive
  acy = gc(0x3d)+0.3; //front-rear, front-positive
  acz = gc(0x3f)-0.2;
  Serial.print(acx);
  Serial.print("   ");
  Serial.print(acy);
  Serial.print("   ");
  Serial.print(acz);
  Serial.println();/**/
  d[0]=SERVO_MID+acy*SCALE+acz*SCALE;
  d[1]=SERVO_MID-acy*SCALE+acz*SCALE;
  d[2]=SERVO_MID+acy*SCALE-acz*SCALE;
  d[3]=SERVO_MID-acy*SCALE-acz*SCALE;
  wa(d);
  delay(10);
  
}


////////////////////////////////functions area//////////////////////////////

void w(uint8_t address, uint8_t value){ //write byte to address
  Wire.beginTransmission(0x68);
  Wire.write(address);
  Wire.write(value);
  Wire.endTransmission();
  }

int16_t r2(uint8_t address){ //read two bytes 
  Wire.beginTransmission(0x68);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 2);
  return (Wire.read() << 8) | Wire.read();
  }

int8_t r(uint8_t address){ //read one byte
  Wire.beginTransmission(0x68);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 1);
  return Wire.read();
  }

double gc(uint8_t address) { //read and convert to newtons
  return r2(address)/819.2;
  }

void wa(int d[]){ // write angles to servos
  for (int i = 0; i<4; i++){
    if (d[i] < SERVO_MIN){
      d[i]=SERVO_MIN;
      }
    else if (d[i] > SERVO_MAX){
      d[i]=SERVO_MAX;
      }
    }
  rr.write(d[0]);
  fr.write(d[1]);
  rl.write(d[2]);
  fl.write(d[3]);  
  }













  
