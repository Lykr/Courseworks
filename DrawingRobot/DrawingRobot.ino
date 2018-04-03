//Define StepMotor
int rM0 = 2;
int rM1 = 3;
int rM2 = 4;
int rM3 = 5;
int lM0 = 6;
int lM1 = 7;
int lM2 = 8;
int lM3 = 9;

//Define Servo
#define SERVO 10

float wheellength = 21;
float penDistance = 5;
float distance;
float Speed = 2;

void(* resetFunc) (void) = 0;

void setup() {
  //rM0~3, lM0~3;
  for (int i = 2; i < 10; i++)
  {
    pinMode(i, OUTPUT);
  }
  pinMode(SERVO, OUTPUT);

  pen(0);
  Serial.begin(9600);
  Serial.println("Please choose the shape you want to draw: \n1. Circle\n2. Triangle\n3. Square\n4. Rectangle\n5. Polygon\n6. Clover\n7. Magic");
}

void loop() {
  switch ((int)getFloat()) {
    case 1:
      drawCircle();
      break;
    case 2:
      drawTriangle();
      break;
    case 3:
      drawSquare();
      break;
    case 4:
      drawRectangle();
      break;
    case 5:
      drawPolygon();
      break;
    case 6:
      drawClover();
      break;
    case 7:
      drawMagic();
      break;
    default:
      break;
  }
}


void drawCircle() {
  float r;
  Serial.println("Please enter the length of radius(greater than 8.24197cm).");
  r = getFloat();
  //drawTest(r + 15.0);
  pen(1);
  turnForward(1, 360 , r); //Need to be changed
  pen(0);
  resetFunc();

}

void drawTriangle() {
  float sideLength;
  Serial.println("Please enter the length of side.");
  sideLength = getFloat();
  //drawTest(sideLength / 1.74 + 10.0);
  //pen down
  for (int i = 0; i < 3; i++) {
    pen(1);
    moveForward(sideLength);
    pen(0);
    moveForward(penDistance + 0.3);//Need to be changed
    turn(1, 120);
    moveBackward(penDistance - 0.5);//Need to be changed
  }
  resetFunc();
}


void drawSquare() {
  float sideLength;
  Serial.println("Please enter the length of side.");
  sideLength = getFloat();
  //drawTest(sideLength / 2.83 + 10.0);
  for (int i = 0; i < 4; i++) {
    pen(1);
    moveForward(sideLength);
    pen(0);
    moveForward(penDistance + 0.2);
    turn(1, 90);
    moveBackward(penDistance - 0.3);
  }
  resetFunc();
}

void drawRectangle() {
  float Length;
  float Wide;
  Serial.println("Please enter the length of side.");
  Length = getFloat();
  Serial.println("Please enter the wide of side.");
  Wide = getFloat();
  //drawTest(sideLength / 2.83 + 10.0);
  for (int i = 0; i < 4; i++) {
    pen(1);
    if (i % 2 == 0) {
      moveForward(Length);
    } else {
      moveForward(Wide);
    }
    pen(0);
    moveForward(penDistance + 0.2);
    turn(1, 90);
    moveBackward(penDistance - 0.3);
    pen(1);
  }
  resetFunc();
}

void drawPolygon() {
  float sideLength;
  int eachAngle, sideNum;
  Serial.println("Please enter the num of sides.");
  sideNum = getFloat();
  eachAngle = (int)(360 / sideNum);
  Serial.println("Please enter the length of side.");
  sideLength = getFloat();
  for (int i = 0; i < sideNum; i++) {
    pen(1);
    moveForward(sideLength);
    pen(0);
    moveForward(penDistance);
    turn(1, eachAngle);
    moveBackward(penDistance - 0.1);
  }
  resetFunc();
}

void drawClover() {
  pen(1);
  for (int i = 0; i < 4; i++) {
    moveForward(12.8);
    turnForward((i + 1) % 2, 180, 8.37);
    pen(0);
    moveForward(penDistance);
    turn((i + 1) % 2, 76);
    moveBackward(penDistance);
    pen(1);
    moveForward(12.8);
    if (i == 1) {
      pen(0);
      moveForward(penDistance);
      turn(1, 90);
      moveBackward(penDistance);
      pen(1);
    }
  }
  resetFunc();
}

void drawMagic() {
  pen(1);
  turnForward(1, 361 , 8.24197); //Need to be changed
  pen(0);
  moveForward(penDistance);
  turn(1, 97);
  moveBackward(penDistance);
  for (int i = 0; i < 3; i++) {
    pen(1);
    moveForward(14.5);
    pen(0);
    moveForward(penDistance + 0.3);//Need to be changed
    if (i == 2) {
      turn(1, 90);
      break;
    } else {
      turn(1, 117);
    }
    moveBackward(penDistance - 0.5);//Need to be changed
  }

  moveForward(8.36);
  turn(1, 90);
  moveBackward(penDistance);
  for (int i = 0; i < 3; i++) {
    pen(1);
    moveForward(14.48);
    pen(0);
    moveForward(penDistance);//Need to be changed
    turn(1, 119);
    if (i == 2) break;
    moveBackward(penDistance);//Need to be changed
  }
  resetFunc();
}

/*void drawTest(float drawSide) {
  for (int i = 0; i < 360; i++) {
    turn(1, 1);
    delay(300);//Need to be changed
    if (getDistance() < drawSide) {
      Serial.println("There is not enough room for drawing, please find a better place.");
      delay(5000);
      turn(0, i + 1);
      resetFunc(); //Back to menu
    } else {
      continue;
    }
  }
  }
*/
// 0-left, 1-right
void turn(int direction, int angle) {
  int num = 22.88 * angle;
  int stepNum = 0;
  while (num--) {
    switch (stepNum) {
      case 0:
        digitalWrite(lM3, 0);
        digitalWrite(rM0, 0);
        digitalWrite(lM2, 0);
        digitalWrite(rM1, 0);
        digitalWrite(lM1, 0);
        digitalWrite(rM2, 0);
        digitalWrite(lM0, 1);
        digitalWrite(rM3, 1);
        break;
      case 1:
        digitalWrite(lM3, 0);
        digitalWrite(rM0, 0);
        digitalWrite(lM2, 0);
        digitalWrite(rM1, 0);
        digitalWrite(lM1, 1);
        digitalWrite(rM2, 1);
        digitalWrite(lM0, 1);
        digitalWrite(rM3, 1);
        break;
      case 2:
        digitalWrite(lM3, 0);
        digitalWrite(rM0, 0);
        digitalWrite(lM2, 0);
        digitalWrite(rM1, 0);
        digitalWrite(lM1, 1);
        digitalWrite(rM2, 1);
        digitalWrite(lM0, 0);
        digitalWrite(rM3, 0);
        break;
      case 3:
        digitalWrite(lM3, 0);
        digitalWrite(rM0, 0);
        digitalWrite(lM2, 1);
        digitalWrite(rM1, 1);
        digitalWrite(lM1, 1);
        digitalWrite(rM2, 1);
        digitalWrite(lM0, 0);
        digitalWrite(rM3, 0);
        break;
      case 4:
        digitalWrite(lM3, 0);
        digitalWrite(rM0, 0);
        digitalWrite(lM2, 1);
        digitalWrite(rM1, 1);
        digitalWrite(lM1, 0);
        digitalWrite(rM2, 0);
        digitalWrite(lM0, 0);
        digitalWrite(rM3, 0);
        break;
      case 5:
        digitalWrite(lM3, 1);
        digitalWrite(rM0, 1);
        digitalWrite(lM2, 1);
        digitalWrite(rM1, 1);
        digitalWrite(lM1, 0);
        digitalWrite(rM2, 0);
        digitalWrite(lM0, 0);
        digitalWrite(rM3, 0);
        break;
      case 6:
        digitalWrite(lM3, 1);
        digitalWrite(rM0, 1);
        digitalWrite(lM2, 0);
        digitalWrite(rM1, 0);
        digitalWrite(lM1, 0);
        digitalWrite(rM2, 0);
        digitalWrite(lM0, 0);
        digitalWrite(rM3, 0);
        break;
      case 7:
        digitalWrite(lM3, 1);
        digitalWrite(rM0, 1);
        digitalWrite(lM2, 0);
        digitalWrite(rM1, 0);
        digitalWrite(lM1, 0);
        digitalWrite(rM2, 0);
        digitalWrite(lM0, 1);
        digitalWrite(rM3, 1);
        break;
      default:
        digitalWrite(lM3, 0);
        digitalWrite(rM0, 0);
        digitalWrite(lM2, 0);
        digitalWrite(rM1, 0);
        digitalWrite(lM1, 0);
        digitalWrite(rM2, 0);
        digitalWrite(lM0, 0);
        digitalWrite(rM3, 0);
        break;
    }
    if (direction) {
      stepNum++;
    } else {
      stepNum--;
    }
    if (stepNum > 7) {
      stepNum = 0;
    }
    if (stepNum < 0) {
      stepNum = 7;
    }
    delay(Speed);
  }
  delay(500);
}

void turnForward(int direction, int angle, float radius) {
  int numf = (int)(angle * (sqrt(radius * radius - 23.04) + 6.7) * 3.404265); //numf = angle / 360 * 2 * (sqrt(r*r-23.04) + 6.7) * PI * 195.05
  float v = (sqrt(radius * radius - 23.04) + 6.7) / (sqrt(radius * radius - 23.04) - 6.7);
  int lStepNum = 0;
  int rStepNum = 0;
  unsigned long startTime = millis();
  unsigned long nowTime;
  int lTimes = 0;
  int rTimes = 0;
  while (1) {
    nowTime = millis();
    if (nowTime - startTime > Speed * lTimes) {
      moveLM(lStepNum--);
      lTimes++;
    }
    if (nowTime - startTime > Speed * v * rTimes) {
      moveRM(rStepNum--);
      rTimes++;
    }
    if (lStepNum < 0) {
      lStepNum = 7;
    }
    if (rStepNum < 0) {
      rStepNum = 7;
    }
    if (numf == lTimes) break;
  }
}

void moveLM(int lStepNum) {
  switch (lStepNum) {
    case 0:
      digitalWrite(lM0, 0);
      digitalWrite(lM1, 0);
      digitalWrite(lM2, 0);
      digitalWrite(lM3, 1);
      break;
    case 1:
      digitalWrite(lM0, 0);
      digitalWrite(lM1, 0);
      digitalWrite(lM2, 1);
      digitalWrite(lM3, 1);
      break;
    case 2:
      digitalWrite(lM0, 0);
      digitalWrite(lM1, 0);
      digitalWrite(lM2, 1);
      digitalWrite(lM3, 0);
      break;
    case 3:
      digitalWrite(lM0, 0);
      digitalWrite(lM1, 1);
      digitalWrite(lM2, 1);
      digitalWrite(lM3, 0);
      break;
    case 4:
      digitalWrite(lM0, 0);
      digitalWrite(lM1, 1);
      digitalWrite(lM2, 0);
      digitalWrite(lM3, 0);
      break;
    case 5:
      digitalWrite(lM0, 1);
      digitalWrite(lM1, 1);
      digitalWrite(lM2, 0);
      digitalWrite(lM3, 0);
      break;
    case 6:
      digitalWrite(lM0, 1);
      digitalWrite(lM1, 0);
      digitalWrite(lM2, 0);
      digitalWrite(lM3, 0);
      break;
    case 7:
      digitalWrite(lM0, 1);
      digitalWrite(lM1, 0);
      digitalWrite(lM2, 0);
      digitalWrite(lM3, 1);
      break;
    default:
      digitalWrite(lM0, 0);
      digitalWrite(lM1, 0);
      digitalWrite(lM2, 0);
      digitalWrite(lM3, 0);
      break;
  }
}

void moveRM(int rStepNum) {
  switch (rStepNum) {
    case 0:
      digitalWrite(rM0, 0);
      digitalWrite(rM1, 0);
      digitalWrite(rM2, 0);
      digitalWrite(rM3, 1);
      break;
    case 1:
      digitalWrite(rM0, 0);
      digitalWrite(rM1, 0);
      digitalWrite(rM2, 1);
      digitalWrite(rM3, 1);
      break;
    case 2:
      digitalWrite(rM0, 0);
      digitalWrite(rM1, 0);
      digitalWrite(rM2, 1);
      digitalWrite(rM3, 0);
      break;
    case 3:
      digitalWrite(rM0, 0);
      digitalWrite(rM1, 1);
      digitalWrite(rM2, 1);
      digitalWrite(rM3, 0);
      break;
    case 4:
      digitalWrite(rM0, 0);
      digitalWrite(rM1, 1);
      digitalWrite(rM2, 0);
      digitalWrite(rM3, 0);
      break;
    case 5:
      digitalWrite(rM0, 1);
      digitalWrite(rM1, 1);
      digitalWrite(rM2, 0);
      digitalWrite(rM3, 0);
      break;
    case 6:
      digitalWrite(rM0, 1);
      digitalWrite(rM1, 0);
      digitalWrite(rM2, 0);
      digitalWrite(rM3, 0);
      break;
    case 7:
      digitalWrite(rM0, 1);
      digitalWrite(rM1, 0);
      digitalWrite(rM2, 0);
      digitalWrite(rM3, 1);
      break;
    default:
      digitalWrite(rM0, 0);
      digitalWrite(rM1, 0);
      digitalWrite(rM2, 0);
      digitalWrite(rM3, 0);
      break;
  }
}

void moveForward(float distance) {
  moveStraight(0, distance);
}

void moveBackward(float distance) {
  moveStraight(1, distance);
}

void moveStraight(int direction, float distance) {
  int num = getSteps(distance);
  int stepNum = 0;
  while (num--) {
    switch (stepNum) {
      case 0:
        digitalWrite(lM0, 0);
        digitalWrite(rM0, 0);
        digitalWrite(lM1, 0);
        digitalWrite(rM1, 0);
        digitalWrite(lM2, 0);
        digitalWrite(rM2, 0);
        digitalWrite(lM3, 1);
        digitalWrite(rM3, 1);
        break;
      case 1:
        digitalWrite(lM0, 0);
        digitalWrite(rM0, 0);
        digitalWrite(lM1, 0);
        digitalWrite(rM1, 0);
        digitalWrite(lM2, 1);
        digitalWrite(rM2, 1);
        digitalWrite(lM3, 1);
        digitalWrite(rM3, 1);
        break;
      case 2:
        digitalWrite(lM0, 0);
        digitalWrite(rM0, 0);
        digitalWrite(lM1, 0);
        digitalWrite(rM1, 0);
        digitalWrite(lM2, 1);
        digitalWrite(rM2, 1);
        digitalWrite(lM3, 0);
        digitalWrite(rM3, 0);
        break;
      case 3:
        digitalWrite(lM0, 0);
        digitalWrite(rM0, 0);
        digitalWrite(lM1, 1);
        digitalWrite(rM1, 1);
        digitalWrite(lM2, 1);
        digitalWrite(rM2, 1);
        digitalWrite(lM3, 0);
        digitalWrite(rM3, 0);
        break;
      case 4:
        digitalWrite(lM0, 0);
        digitalWrite(rM0, 0);
        digitalWrite(lM1, 1);
        digitalWrite(rM1, 1);
        digitalWrite(lM2, 0);
        digitalWrite(rM2, 0);
        digitalWrite(lM3, 0);
        digitalWrite(rM3, 0);
        break;
      case 5:
        digitalWrite(lM0, 1);
        digitalWrite(rM0, 1);
        digitalWrite(lM1, 1);
        digitalWrite(rM1, 1);
        digitalWrite(lM2, 0);
        digitalWrite(rM2, 0);
        digitalWrite(lM3, 0);
        digitalWrite(rM3, 0);
        break;
      case 6:
        digitalWrite(lM0, 1);
        digitalWrite(rM0, 1);
        digitalWrite(lM1, 0);
        digitalWrite(rM1, 0);
        digitalWrite(lM2, 0);
        digitalWrite(rM2, 0);
        digitalWrite(lM3, 0);
        digitalWrite(rM3, 0);
        break;
      case 7:
        digitalWrite(lM0, 1);
        digitalWrite(rM0, 1);
        digitalWrite(lM1, 0);
        digitalWrite(rM1, 0);
        digitalWrite(lM2, 0);
        digitalWrite(rM2, 0);
        digitalWrite(lM3, 1);
        digitalWrite(rM3, 1);
        break;
      default:
        digitalWrite(lM0, 0);
        digitalWrite(rM0, 0);
        digitalWrite(lM1, 0);
        digitalWrite(rM1, 0);
        digitalWrite(lM2, 0);
        digitalWrite(rM2, 0);
        digitalWrite(lM3, 0);
        digitalWrite(rM3, 0);
        break;
    }
    if (direction) {
      stepNum++;
    } else {
      stepNum--;
    }
    if (stepNum > 7) {
      stepNum = 0;
    }
    if (stepNum < 0) {
      stepNum = 7;
    }
    delay(Speed);
  }
  delay(500);
}

//0-up, 1-down
void pen(int ud) {
  int i;
  switch (ud) {
    case 0:
      for (i = 0; i < 50; i++) {
        servopulse(40);
      }
      break;
    case 1:
      for (i = 0; i < 50; i++) {
        servopulse(10);
      }
      break;
    default:
      break;
  }
  delay(800);//Need to be changed
}

int getSteps(float distance) {
  int steps;
  steps = (int)(195.05 * distance); // 4096 / wheellength * distance
  return steps;
}

void servopulse(int angle) {
  int pulsewidth = (angle * 11) + 500;
  digitalWrite(SERVO, HIGH);
  delayMicroseconds(pulsewidth);
  digitalWrite(SERVO, LOW);
  delayMicroseconds(20000 - pulsewidth);
}

float getFloat() {
  String inString = "";
  while (1) {
    while (Serial.available() > 0)
    {
      int inChar = Serial.read();
      if (inChar != '\n') {
        inString += (char)inChar;
      } else {
        goto end;
      }
    }
  }
end:
  return inString.toFloat();
}
