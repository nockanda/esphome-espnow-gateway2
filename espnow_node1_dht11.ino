//10번보드의 코드

#include <ESP8266WiFi.h>
#include <espnow.h>
#include "DHT.h"

#define DHTPIN D3
#define DHTTYPE DHT11
 DHT dht(DHTPIN, DHTTYPE);
 
#define myid 1
#define my_channel 9

byte board[] = {0x40,0x91,0x51,0x4E,0x65,0xE2};

unsigned long t = 0;

struct mysensor{
  byte type = 1;
  int id = myid;
  float data1;
  float data2;
};

struct payload{
  int id;
  char msg;
};

mysensor sensor;
payload mypayload;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  dht.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_STA); //제일 처음 할일! 와이파이모드를 스테이션 모드로!
  WiFi.begin("youtuber","nockanda",my_channel,NULL,false); //채널9로 바꾼다
  WiFi.disconnect(); //그러나 WiFi로써의 기능은 사용하지 않겠다!
  
  //ESPNOW의 기능을 활성화한다!
  if (esp_now_init() != 0) {
    //ESP NOW 시작 실패!
    Serial.println("ESPNOW 실패!");
    return;
  }else{
     //ESP NOW 시작 성공!
     Serial.println("ESPNOW 성공!");
  }

  //지금 이보드의 역할은 주고 받는것을 다하겠다!
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  //나의 슬레이브를 등록해야겠다!
  esp_now_add_peer(board, ESP_NOW_ROLE_COMBO, 1, NULL, 0);//뒤에 2개는 ID, PW

  //송신완료 콜백함수 등록
  esp_now_register_send_cb(OnDataSent);

  //수신완료 콜백 함수 등록
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis() - t > 2000){
    t = millis();
    sensor.data1 = dht.readHumidity();
    sensor.data2 = dht.readTemperature();
    esp_now_send(board, (uint8_t *) &sensor, sizeof(sensor));
  }
}

//송신콜백함수 원형:
void OnDataSent(uint8_t *mac, uint8_t status) {
   //status가 0일때 송신 완료!
   if(status == 0){
      Serial.println("성공적으로 송신했음!");
   }else{
      Serial.println("송신 실패!!");
   }
}


//수신콜백함수 원형:
void OnDataRecv(uint8_t * mac, uint8_t * data, uint8_t len) {
  memcpy(&mypayload,data,sizeof(mypayload));
  if(mypayload.id != myid){
    Serial.println("이 메시지는 나에게 온게 아닙니다!");
    return;
  }
  Serial.print(mypayload.id);  
  Serial.print(", ");
  Serial.println(mypayload.msg);
  if(mypayload.msg == '0'){
    //켜기
    digitalWrite(LED_BUILTIN,LOW);
  }else if(mypayload.msg == '1'){
    //끄기
    digitalWrite(LED_BUILTIN,HIGH);
  }
}
