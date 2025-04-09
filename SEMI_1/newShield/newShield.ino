// 라인트레이서 센서
#define LEFT 9
#define RIGHT 8

// 모터드라이버 PWR 
#define speedR 10
#define speedL 3

// 모터드라이버 모터 방향
#define motorRIN1 7
#define motorRIN2 6
#define motorLIN1 4
#define motorLIN2 5

// 인식률을 위한 딜레이
#define delay_front 50
#define delay_end 50

#define normal_speed 100

void set_motorR_speed(int n);
void set_motorL_speed(int n);
void turnR();
void turnL();
void goStr();
void goBack();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(motorRIN1, OUTPUT);
  pinMode(motorRIN2, OUTPUT);
  pinMode(motorLIN1, OUTPUT);
  pinMode(motorLIN2, OUTPUT);
  
  pinMode(speedR, OUTPUT);
  pinMode(speedL, OUTPUT);

  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);
}

bool goLeft, goRight;

void loop() {
  // put your main code here, to run repeatedly:
  set_motorR_speed(normal_speed);
  set_motorL_speed(normal_speed);

  goLeft = digitalRead(LEFT);
  goRight =digitalRead(RIGHT);

  if(goLeft==HIGH){
    Serial.println("left");
    delay(1000);
    turnL();
  }else if(goRight==HIGH){
    Serial.println("right");
    delay(1000);
    turnR();
  }

  if(goLeft == LOW && goRight == LOW){
    goStr();
  }
}

void set_motorR_speed(int n){
  analogWrite(speedR, n);
}

void set_motorL_speed(int n){
  analogWrite(speedL, n);
}

void turnR(){
  digitalWrite(motorLIN1, HIGH);
  digitalWrite(motorLIN2, LOW);
  digitalWrite(motorRIN1, LOW);
  digitalWrite(motorRIN2, HIGH);
}

void turnL(){
  digitalWrite(motorLIN1,LOW);
  digitalWrite(motorLIN2, HIGH);
  digitalWrite(motorRIN1, HIGH);
  digitalWrite(motorRIN2, LOW);
}

void goStr(){
  digitalWrite(motorRIN1, HIGH);
  digitalWrite(motorRIN2, LOW);
  digitalWrite(motorLIN1, HIGH);
  digitalWrite(motorLIN2, LOW);
}

void stop(int time){
  set_motorR_speed(0);
  set_motorL_speed(0);
  delay(time);
  set_motorR_speed(normal_speed);
  set_motorL_speed(normal_speed);
}

void goBack(){
  digitalWrite(motorRIN1, LOW);
  digitalWrite(motorRIN2, HIGH);
  digitalWrite(motorLIN1, LOW);
  digitalWrite(motorLIN2, HIGH);
}
