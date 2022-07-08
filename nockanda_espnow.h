#include <ESP8266WiFi.h>
#include <espnow.h>

byte board[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

typedef void (*type1_cb)(byte type, float data1, float data2);
typedef void (*type2_cb)(byte type, int data1, int data2);
typedef void (*type3_cb)(byte type, int data1, int data2, int data3);

type1_cb sensor1_cb;
type2_cb sensor2_cb;
type3_cb sensor3_cb;

struct mysensor1{
  byte type;
  int id;
  float data1;
  float data2;
};
struct mysensor2{
  byte type;
  int id;
  int data1;
  int data2;
};
struct mysensor3{
  byte type;
  int id;
  int data1;
  int data2;
  int data3;
};

struct payload{
  int id;
  char msg;
};

mysensor1 sensor1;
mysensor2 sensor2;
mysensor3 sensor3;
payload mypayload;

void OnDataRecv(uint8_t * mac, uint8_t * data, uint8_t len) {
  if(data[0] == 1){
     memcpy(&sensor1,data,sizeof(sensor1));
     ESP_LOGD("nockanda", "DHT-11 타입: %d, 값1: %f, 값2: %f\n",sensor1.type, sensor1.data1,sensor1.data2);
     sensor1_cb(sensor1.type,sensor1.data1,sensor1.data2);
  }else if(data[0] == 2){
     memcpy(&sensor2,data,sizeof(sensor2));
     ESP_LOGD("nockanda", "MH-Z19 타입: %d, 값1: %d, 값2: %d\n",sensor2.type, sensor2.data1,sensor2.data2);
     sensor2_cb(sensor2.type,sensor2.data1,sensor2.data2);
  }else if(data[0] == 3){
     memcpy(&sensor3,data,sizeof(sensor3));
     ESP_LOGD("nockanda", "PMS7003 타입: %d, 값1: %d, 값2: %d, 값3: %d\n",sensor3.type, sensor3.data1,sensor3.data2,sensor3.data3,sensor3.data3);
     sensor3_cb(sensor3.type,sensor3.data1,sensor3.data2,sensor3.data3);
  }
}

//송신콜백함수 원형:
void OnDataSent(uint8_t *mac, uint8_t status) {
   //status가 0일때 송신 완료!
   #ifdef ESP_LOGD
   if(status == 0){
      ESP_LOGD("nockanda", "성공\n");
   }else{
      ESP_LOGD("nockanda", "실패\n");
   }
        
   #endif
   
}

class {
public:
    void begin() {
        WiFi.mode(WIFI_STA);  
        if (esp_now_init() == 0) {
            esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
            //나의 슬레이브를 등록해야겠다!
             esp_now_add_peer(board, ESP_NOW_ROLE_COMBO, 1, NULL, 0);//뒤에 2개는 ID, PW
             //송신완료 콜백함수 등록
             esp_now_register_send_cb(OnDataSent);
            esp_now_register_recv_cb(OnDataRecv);
        }
    }
    void set_sensor1(type1_cb callback){
       sensor1_cb = callback;
    }
    void set_sensor2(type2_cb callback){
       sensor2_cb = callback;
    }
    void set_sensor3(type3_cb callback){
       sensor3_cb = callback;
    }
    void send_data(int id, char c){
       mypayload.id = id;
       mypayload.msg = c;    
       esp_now_send(board, (uint8_t *) &mypayload, sizeof(mypayload));
    }
} NOCKANDA_ESPNOW;
