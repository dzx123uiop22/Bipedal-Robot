#include <Servo.h>
#include "constants.h"

Servo ankleL;
Servo ankleR;
Servo kneeL;
Servo kneeR;
Servo hipL;
Servo hipR;

void move(int hipChange, int kneeChange, int ankleChange, char leg)
{
  if (leg == 'l')
  {
    hipL.write(hipLOffset - hipChange);
    kneeL.write(kneeLOffset - kneeChange);
    ankleL.write(2 * ankleLOffset - ankleChange);
  }
  else if (leg == 'r')
  {
    hipR.write(hipROffset + hipChange);
    kneeR.write(kneeROffset + kneeChange);
    ankleR.write(ankleChange);
  }
}

void pos(float x, float y, char leg)
{
  float hipRad1 = acos(x / y);
  int hipDeg1 = hipRad1 * (180 / PI);
  float z = sqrt((sq(x)) + (sq(y)));
  float hipRad2 = acos((sq(k1) + sq(z) - sq(k2)) / (2 * k1 * z));
  int hipDeg2 = hipRad2 * (180 / PI);
  int hipDeg = hipDeg1 + hipDeg2;
  float kneeRad = acos((sq(k1) + sq(k2) - sq(z)) / (2 * k1 * k2));
  int kneeDeg = kneeRad * (180 / PI);
  int ankleDeg = hipDeg + kneeDeg - 90;

  move(hipDeg, kneeDeg, ankleDeg, leg);
}

void takeStep(int stepLength, int stepSpeed)
{
  for (int i = stepLength; i >= -stepLength; i -= 0.5)
  {
    pos(i, stepHeight - stepClearance, 'r');
    pos(-i, stepHeight - stepClearance, 'l');
    delay(stepSpeed);
  }
  for (float i = stepLength; i >= -stepLength; i -= 0.5)
  {
    pos(-i, stepHeight - stepClearance, 'r');
    pos(i, stepHeight, 'l');
    delay(stepSpeed);
  }
}

void initialize()
{
  for (float i = 10.7; i >= stepHeight; i -= 0.1)
  {
    pos(0, i, 'l');
    pos(0, i, 'r');
  }
}
void setup()
{
  Serial.begin(9600);
  ankleL.attach(4);
  ankleR.attach(5);
  kneeL.attach(6);
  kneeR.attach(7);
  hipL.attach(8);
  hipR.attach(9);

  ankleL.write(ankleLOffset);
  ankleR.write(ankleROffset);
  kneeL.write(kneeLOffset);
  kneeR.write(kneeROffset);
  hipL.write(hipLOffset);
  hipR.write(hipROffset);

  initialize();

  delay(5000);
}

void loop()
{
  takeStep(2, 0);
}
