#ifdef ESP32
  #include <esp_now.h>
  #include <WiFi.h>
#else
  #include <espnow.h>
  #include <ESP8266WiFi.h>
  #define ESP_OK 0
#endif

 // struktur untuk pengiriman data harus sama dengan struktur pada penerima
typedef struct struct_message {
  int id;
  int x;
  int y;
}struct_message;
 
struct_message myData;
 
// Membuat struktur untuk tiap board
struct_message board1;
 
struct_message boardsStruct[3] = {board1};
 
// callback saat data diterima
#ifdef ESP32
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) 
#else
void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) 
#endif
{
  char macStr[18];
  Serial.print("Paket diterima dari: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].x = myData.x;
  boardsStruct[myData.id-1].y = myData.y;
  Serial.printf("x value: %d \n", boardsStruct[myData.id-1].x);
  Serial.printf("y value: %d \n", boardsStruct[myData.id-1].y);
  Serial.println();
}

void setup(){
  Serial.begin(74880);
  Serial.println();
  Serial.print("ESP Board MAC Addressx:  ");
  Serial.println(WiFi.macAddress());

   WiFi.mode(WIFI_STA);
 
  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
   
  //daftrakan callback
  esp_now_register_recv_cb(OnDataRecv);
    Serial.println("Initializing ESP-NOW OK");
}
  
void loop() {
  // Mengakses data dari setiap board
  //Serial.println(boardsStruct[0].x,DEC); 
  //delay(1000);  
}
