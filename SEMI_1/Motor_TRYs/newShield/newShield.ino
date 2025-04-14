// 라인트레이서 센서
#define LEFT 9
#define RIGHT 8

// 모터드라이버 PWR 
#define motorR_F 6   //A-1A (PWM)
#define motorR_B 7 //A-1B
#define motorL_F 5    //B-1A (PWM)
#define motorL_B 4  //B-1B

#define ECHO 10
#define TRIG 11

<<<<<<< HEAD:SEMI_1/newShield/newShield.ino
<<<<<<< HEAD
int default_speed = 150;
=======
#define normal_speed 100
>>>>>>> main
=======
int default_speedR = 150;
int default_speedL = 140;
>>>>>>> main:SEMI_1/Motor_TRYs/newShield/newShield.ino

long duration;
double distance;

void set_motorR_speed(int n);
void set_motorL_speed(int n);
void turnR();
void turnL();
void goStr();
void goBack();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(motorR_F, OUTPUT);
  pinMode(motorL_F, OUTPUT);
  
  pinMode(motorR_B, OUTPUT);
  pinMode(motorL_B, OUTPUT);

  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

}

bool goLeft, goRight;

void loop() {
  goLeft = digitalRead(RIGHT);
  goRight =digitalRead(LEFT);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH);
  if(duration == 0){
    return;
  }

  distance = duration /58;

  if(distance <=6){
    stop(100);
    return;
  }

  if(goLeft==HIGH && goRight ==LOW){

<<<<<<< HEAD:SEMI_1/newShield/newShield.ino
  goStr();
/*
  if(goLeft == LOW && goRight == LOW){
    goStr();
  }
  if(goLeft==HIGH){
    turnL();
  }
  if(goRight==HIGH){
    turnR();
  }

  */
}

void turnR(){
  analogWrite(motorL_F, default_speed);
  analogWrite(motorR_F,0);
  analogWrite(motorL_B, 0);
  analogWrite(motorR_B,default_speed);
=======
    turnL();
  }
  
  else if(goRight==HIGH && goLeft ==LOW){
    turnR();
  }
  else{
    goStr();
  }
  delay(30);
  stop(70);

}

void turnR(){
  analogWrite(motorL_F, default_speedL);
  analogWrite(motorR_F,0);
  analogWrite(motorL_B, 0);
  analogWrite(motorR_B,0);
>>>>>>> main:SEMI_1/Motor_TRYs/newShield/newShield.ino
}

void turnL(){
  analogWrite(motorL_F, 0);
<<<<<<< HEAD:SEMI_1/newShield/newShield.ino
  analogWrite(motorR_F,default_speed);
  analogWrite(motorL_B, default_speed);
=======
  analogWrite(motorR_F,default_speedR);
  analogWrite(motorL_B,0);
>>>>>>> main:SEMI_1/Motor_TRYs/newShield/newShield.ino
  analogWrite(motorR_B,0);
}

void goStr(){
<<<<<<< HEAD:SEMI_1/newShield/newShield.ino
  analogWrite(motorL_F, default_speed);
  analogWrite(motorR_F,default_speed);
  analogWrite(motorL_B, 0);
=======
  analogWrite(motorL_F, default_speedL);
  analogWrite(motorL_B, 0);
  analogWrite(motorR_F,default_speedR);
>>>>>>> main:SEMI_1/Motor_TRYs/newShield/newShield.ino
  analogWrite(motorR_B,0);
}

void stop(int time){
  analogWrite(motorL_F,0);
  analogWrite(motorR_F,0);
  analogWrite(motorL_B,0);
  analogWrite(motorR_B,0);
  delay(time);
}

void goBack(){
  analogWrite(motorL_F, 0);
  analogWrite(motorL_B, default_speedL);
  analogWrite(motorR_F,0);
<<<<<<< HEAD:SEMI_1/newShield/newShield.ino
  analogWrite(motorR_B,default_speed);
}
=======
  analogWrite(motorR_B,default_speedR);

}
>>>>>>> main:SEMI_1/Motor_TRYs/newShield/newShield.ino
