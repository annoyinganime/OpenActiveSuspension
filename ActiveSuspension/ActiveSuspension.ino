#include <Wire.h>
#include <Servo.h>

#define SCALE 10 // servo "degrees" per newton
#define SERVO_MIN 70
#define SERVO_MAX 110
#define SERVO_MID 90
#define SERVO_DIFF 12
#define X_CAL 0.0
#define Y_CAL -0.4
#define Z_CAL 1.2
#define PITCH -acz
#define ROLL acy

int d[4]; //for storing angles in "degrees"
Servo fl, fr, rl, rr; //front-left, front-right, rear-left, rear-right
void setup() {
  Wire.begin();
  w(0x6b, 0x00); //disable mpu6050 sleep
  w(0x1c, 0b00001000); //maximum accelerometer resolution, also change value in gc if changing this
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
 
  /*for (int i = 0; i<4; i++){   // for calibration purposes only
    d[i]=SERVO_MAX;            // Use SERVO_MAX, SERVO_MIN or SERVO_MID to see where maximal, minimal and central points are in reality. Comment out wa(d) in loop() 
    }
  wa(d);
    /**/
  
}
double acx, acy, acz; 

void loop() {
  // 3b- x, 3d- y, 3f- z       rr, fr, rl, fl
  acx = gc(0x3b)-X_CAL; 
  acy = gc(0x3d)-Y_CAL; 
  acz = gc(0x3f)-Z_CAL;
  
  Serial.print(acx);
  Serial.print("   ");
  Serial.print(acy);
  Serial.print("   ");
  Serial.print(acz);
  Serial.println();/**/
  //   "Real looking" suspension - car acts like it has big inertia
  /*
  d[0] = SERVO_MID + (PITCH-ROLL)*SCALE;  //rr
  d[1] = SERVO_MID + (-PITCH-ROLL)*SCALE; //fr
  d[2] = SERVO_MID + (PITCH+ROLL)*SCALE;  //rl
  d[3] = SERVO_MID + (-PITCH+ROLL)*SCALE; //fl/**/

  //   Active suspension - car tries to counteract its inertia and compensate its angle
  d[0] = SERVO_MID + (-PITCH+ROLL)*SCALE; //rr
  d[1] = SERVO_MID + (PITCH+ROLL)*SCALE;  //fr
  d[2] = SERVO_MID + (-PITCH-ROLL)*SCALE; //rl
  d[3] = SERVO_MID + (PITCH-ROLL)*SCALE;  //fl
  wa(d);
  delay(10);
  
}


////////////////////////////////functions area////////////////////////////////////

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

void wa(int d[]){ // writes angles to servos, applies calibration and limits max points
  //FIRST - apply diagonal calibration
  d[0] = d[0] - SERVO_DIFF;
  d[1] = d[1] + SERVO_DIFF;
  d[2] = d[2] + SERVO_DIFF;
  d[3] = d[3] - SERVO_DIFF;
  //SECOND - remove overshoots
  for (int i = 0; i<4; i++){
    if (d[i] < SERVO_MIN){
      d[i]=SERVO_MIN;
      }
    else if (d[i] > SERVO_MAX){
      d[i]=SERVO_MAX;
      }
    }
  /*for (int i = 0; i<4; i++){
    Serial.print(" ");
    Serial.print(i);
    Serial.print(":  ");
    Serial.print(d[i]);
    }
  Serial.println();/**/
  //THIRD - invert two of four servos
  d[0] = 180 - d[0];
  d[1] = d[1];
  d[2] = d[2];
  d[3] = 180 - d[3];
  //AND ONLY THEN write to servos
  rr.write(d[0]);
  fr.write(d[1]);
  rl.write(d[2]);
  fl.write(d[3]);
  
  }













  
