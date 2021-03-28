#ifdef ESP32
#include <esp_now.h>
#include <WiFi.h>
#else
#include <espnow.h>
#include <ESP8266WiFi.h>
typedef enum {ESP_NOW_SEND_SUCCESS = 0, ESP_NOW_SEND_FAIL};
typedef enum {ESP_OK = 0} esp_err_t;
#endif

unsigned long now = 0;
unsigned long interval = 0;
// Ganti dengan MAC penerima
uint8_t broadcastAddress[] = {0xCC, 0x50, 0xE3, 0x0B, 0x99, 0x54};

// ID harus unik
const int id = 1;

// struktur untuk pengiriman data harus sama dengan struktur pada penerima
typedef struct struct_message {
  int id;
  int x;
  int y;
  int lati[4];
  int longi[4];
} struct_message;

struct_message myData;

// callback saat data dikirim
#ifdef ESP32
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
#else
void OnDataSent(uint8_t *mac_addr, uint8_t status)
#endif
{
  now = millis();
  Serial.print("Status pengiriman:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sukses" : "Gagal");
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Gagal inisialisasi ESP-NOW");
    return;
  }
#ifdef ESP32
#else
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

#endif
  // registrasi ke penerima dan cek status paket
  esp_now_register_send_cb(OnDataSent);

  // Registrasi peer
#ifdef ESP32
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Menambahkan diri peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Gagal menambahkan diri ke peer");
    return;
  }
#else
  if (esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0) != ESP_OK) {
    Serial.println("Gagal menambahkan diri ke peer");
    return;
  }
#endif

  //daftrakan callback
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Initializing ESP-NOW OK");
}



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
  interval = millis() - now;
  char macStr[18];

  Serial.println();
  Serial.print("Paket diterima dari: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);

  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("ID: %u (%u byte)\n", myData.id, len);
  Serial.printf("Delay: %lu \n", interval);

  Serial.printf("Panjang data: %d \n", myData.x);
  Serial.printf("Dummy: %d \n", myData.y);

  float a;
  //byte array to float
  for (byte i = 0; i < 4; i++) {
    ((uint8_t*)&a)[i] = myData.lati[i];
  }
  Serial.printf("Latitude: %f bytes\n", a);

  for (byte i = 0; i < 4; i++) {
    ((uint8_t*)&a)[i] = myData.longi[i];
  }
  Serial.printf("Longitude: %f bytes\n", a);

  Serial.println();
}

byte counter = 0;
void loop() {
  counter++;
  // Nilai untuk dikirim
  myData.id = counter;
  myData.x = 0;
  myData.y = WiFi.RSSI();//random(0, 50);
  for (byte i = 0; i < 4; i++) {
    myData.lati[i] = 0;
    myData.longi[i] = 0;
  }

#ifdef ESP32
  esp_err_t result;
#else
  int result;
#endif

  Serial.print("Permintaan Pengiriman ");
  result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    //Serial.print("x (dummy):"); Serial.println(myData.x, DEC);
    //Serial.print("y:"); Serial.println(myData.y, DEC);
    Serial.println("berhasil");
  }
  else {
    Serial.println("gagal");
  }

  delay(5000);
}
