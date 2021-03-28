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
  int lati[4];
  int longi[4];
} struct_message;

struct_message myData;

// Membuat struktur untuk tiap board
struct_message board1;

struct_message boardsStruct[3] = {board1};


uint8_t rec_len = 0;
bool datareceived = false;
float new_lati = 12.34567890;
float new_longi = 9.876543210;
// struktur untuk pengiriman data harus sama dengan struktur pada penerima


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
  //boardsStruct[myData.id - 1].x = myData.x;
  //boardsStruct[myData.id - 1].y = myData.y;
  Serial.printf("x value: %d \n", myData.x);
  Serial.printf("y value: %d \n", myData.y);
  Serial.println();
  rec_len = len;
  datareceived = true;
}

//Receiver address
uint8_t broadcastAddress[] = {0xCC, 0x50, 0xE3, 0x0B, 0x99, 0xD7};

// Callback when data is sent
void OnDataSent(uint8_t *broadcastAddress, uint8_t sendStatus) {
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  }
  else {
    Serial.println("Delivery fail");
  }
}

void setup() {
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

#ifdef ESP32
#else
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
#endif

  //daftrakan send callback
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

  //daftrakan recv callback
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Initializing ESP-NOW OK");
}
void loop() {
  if (datareceived) {
    datareceived = false;
    //myData.id = myData.id;
    myData.x = rec_len;
    myData.y = WiFi.RSSI();

    byte dataArray[4] = {
      ((uint8_t*)&new_lati)[0],
      ((uint8_t*)&new_lati)[1],
      ((uint8_t*)&new_lati)[2],
      ((uint8_t*)&new_lati)[3]
    };

    for (byte i = 0; i < 4; i++) {
      myData.lati[i] = dataArray[i];
    }

    byte dataArrayLongi[4] = {
      ((uint8_t*)&new_longi)[0],
      ((uint8_t*)&new_longi)[1],
      ((uint8_t*)&new_longi)[2],
      ((uint8_t*)&new_longi)[3]
    };

    for (byte i = 0; i < 4; i++) {
      myData.longi[i] = dataArrayLongi[i];
    }


    //long value = (unsigned long)(buf[4] << 24) | (buf[3] << 16) | (buf[2] << 8) | buf[1];
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  }
}
