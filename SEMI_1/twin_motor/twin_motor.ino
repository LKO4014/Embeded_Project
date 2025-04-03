#include <SoftwareSerial.h>
#define TXD 2
#define RXD 3
#define MOT_R 7
#define MOT_L 8

SoftwareSerial bt(TXD, RXD);

enum dir{left=0, front, right};
String str;
int self_mode;
int angle;
int accel;

void set_info(){
  if(str.length()!=0){
    str.trim();
    Serial.print("str : ");
    Serial.println(str);
    self_mode = str[0]-'0';
    int i=2;
    String tmp;
    while(i<str.length()-2){
      tmp+=str[i];
      i++;
    }
    angle = tmp.toInt();
    accel = str[str.length()-1]-'0';
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  bt.begin(9600);
  pinMode(MOT_R, OUTPUT);
  pinMode(MOT_L, OUTPUT);
  str = "0 0 0";
  self_mode=1; angle = 0; accel = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(bt.available()){
    str = bt.readStringUntil('/');
    set_info();

    /*
    Serial.print("self_mode : ");
    Serial.print(self_mode);
    Serial.print(" angle : ");
    Serial.print(angle);
    Serial.print(" accel : ");
    Serial.println(accel);
    */
  }
}
