# OpenActiveSuspension
My simple attempt to make an active suspension
DEMO: https://www.youtube.com/watch?v=fvEdsod2xxM


This was written to work with arduino nano (any atmega328 or 168 will be fine) and mpu6050 gyro. 
This is my attempt to make something like this: https://www.youtube.com/watch?v=MSoF7S1yCOU

Setup guide:
1. Connect mpu6050 to arduino, connect servos to pins: rear right to D2, front right to D3, rear left to D4 and front left to D5. Secure gyro in its place in the car. Also secure servos so that they can controll the suspension angle.
2. Comment out "wa(d);" in loop().
3. Flash arduino with your sketch. This will set all your servos centered. While servos are powered, place servo arms as close to their desired position as possible. At this point all your four wheels should be touching the ground. If one of them is slightly lifted, you should adjust SERVO_MID value - add or substract 2-3 degrees and see where your lifted wheel moved. Proceed when all wheels are touching the gound equally. 
4. Now open Serial monitor. Here, you will be able to find which gyro axes you should use - the one you don't need is the one wich shows highest/lowest value (higher than 7 or lower than -7), for example - Z. Write other two values in the calibration zone: in our example they will be X_CAL and Y_CAL. 
5. Tilt your car about 30 degrees nose down. Look which of useful values is changing, and if it is increasing or decreasing. For example, let it be acx, and it is decreasing. Then write -acx in #define PITCH. Then tilt your car same amount, but to the right. In our example, if the value is increasing, write acy in #define ROLL, and if it is decreasing - write -acy.
6. Now uncoment "wa(d)" from step 2, flash your sketch again an now everything will be ready. You can now play with SCALE to cahnge how much the car is tilting.

I only tested this sketch on 1:24 scale car with 9g servos on a suspension, but it shoud work fine on a larger scale models.
