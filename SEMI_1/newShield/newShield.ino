// 라인트레이서 센서
#define LEFT 9
#define RIGHT 8

// 모터드라이버 PWR 
#define motorR_F 6   //A-1A (PWM)
#define motorR_B 7 //A-1B
#define motorL_F 5    //B-1A (PWM)
#define motorL_B 4  //B-1B

// 인식률을 위한 딜레이
#define delay_front 50
#define delay_end 50

int default_speedR = 150;
int default_speedL = 140;

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

}

bool goLeft, goRight;

void loop() {
  goLeft = digitalRead(RIGHT);
  goRight =digitalRead(LEFT);

  if(goLeft==HIGH && goRight ==LOW){

    turnL();
  }
  
  else if(goRight==HIGH && goLeft ==LOW){
    turnR();
  }
  else{
    goStr();
  }
  delay(30);
  stop(80);

}

void turnR(){
  analogWrite(motorL_F, default_speedL);
  analogWrite(motorR_F,0);
  analogWrite(motorL_B, 0);
  analogWrite(motorR_B,0);
}

void turnL(){
  analogWrite(motorL_F, 0);
  analogWrite(motorR_F,default_speedR);
  analogWrite(motorL_B,0);
  analogWrite(motorR_B,0);
}

void goStr(){
  analogWrite(motorL_F, default_speedL);
  analogWrite(motorL_B, 0);
  analogWrite(motorR_F,default_speedR);
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
  analogWrite(motorR_B,default_speedR);

}