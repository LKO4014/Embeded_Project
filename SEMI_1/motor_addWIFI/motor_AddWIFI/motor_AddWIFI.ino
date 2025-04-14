#include <WiFiEspAT.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>


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

#define routeDelay 2000
/////wifi
// 📡 ESP8266 AT용 소프트웨어 시리얼
SoftwareSerial espSerial(2, 3); // RX, TX (ESP8266 연결 핀)

// 📶 Wi-Fi 정보
const char* ssid = "KOSTA_1Class_2.4G";
const char* password = "kosta1001";

// 🧠 라즈베리파이의 MQTT 브로커 IP
const char* mqttServer = "192.168.0.6";
const int mqttPort = 1883;
const char* mqttTopic = "test/arduino_main";

//wifi
// 🌐 클라이언트 객체
WiFiClient espClient;
PubSubClient client(espClient);

void turnPoint(String a);

// 📨 MQTT 메시지 수신 콜백
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("[📥 MQTT 수신] 토픽: ");
  Serial.print(topic);
  Serial.print(" | 메시지: ");
  

  String message = "";
  
  for (unsigned int i = 0; i < length; i++) {
    char c = (char)payload[i];
    Serial.print(c);
    message += c;
  }
  
  Serial.println();

  turnPoint(message);
}

// 🔁 MQTT 브로커 연결 함수
void connectMQTT() {
  while (!client.connected()) {
    Serial.print("MQTT 연결 시도...");

    String clientId = "arduinoClient-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("성공!");
      client.subscribe(mqttTopic);  // 토픽 구독
    } else {
      Serial.print("실패, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}
//////

int default_speedR = 150;
int default_speedL = 140;
int cnt=0;

long duration;
double distance;

void set_motorR_speed(int n);
void set_motorL_speed(int n);
void turnR();
void turnL();
void goStr();
void goBack();

void turnPoint(String a);
void routeR();
void routeL();
void routeS();

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

  espSerial.begin(9600);     // ESP8266 시리얼
  WiFi.init(&espSerial);     // WiFiEspAT 초기화

  // Wi-Fi 연결
  Serial.println("Wi-Fi 연결 시도 중...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\n✅ Wi-Fi 연결 완료!");
  Serial.print("📡 IP 주소: ");
  Serial.println(WiFi.localIP());

  // MQTT 브로커 설정 및 연결
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  connectMQTT();

  Serial.println("💬 MQTT 메시지 발행 준비 완료!");

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
  

  if(cnt%20==0){
    if (!client.connected()) {
      connectMQTT(); 
    }  
    client.loop();
  }
  
  //delay(1000);
  cnt++;
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

void turnPoint(String a)
{
  bool sensor;
  if(digitalRead(LEFT)==HIGH && digitalRead(RIGHT)==HIGH){
    sensor == true;
  }else{
    sensor = false;
  }
  if(sensor == true){
    if(a=="0"){
      routeL();
    }else if(a=="1"){
      routeS();
    }
    else if(a=="2"){
      routeR();
    }
  } 
}

void routeR()
{
  turnR();
  delay(routeDelay);
}

void routeL()
{
  turnL();
  delay(routeDelay);
}

void routeS()
{
  goStr();
  delay(routeDelay);
}