#ifdef ESP32
#include <esp_now.h>
#include <WiFi.h>
#else
#include <espnow.h>
#include <ESP8266WiFi.h>
typedef enum {ESP_NOW_SEND_SUCCESS = 0, ESP_NOW_SEND_FAIL};
typedef enum {ESP_OK = 0} esp_err_t;
#endif


// Ganti dengan MAC penerima
uint8_t broadcastAddress[] = {0x50, 0x02, 0x91, 0xFE, 0x4C, 0x01};

// ID harus unik
const int id = 1;

// struktur untuk pengiriman data harus sama dengan struktur pada penerima
typedef struct struct_message {
  int id;
  int x;
  int y;
} struct_message;

struct_message myData;

// callback saat data dikirim
#ifdef ESP32
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
#else
void OnDataSent(uint8_t *mac_addr, uint8_t status)
#endif
{
  Serial.print("Status pengiriman:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sukses" : "Gagal");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Gagal inisialisasi ESP-NOW");
    return;
  }
#ifdef ESP32
#else
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

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
  if (esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0) != ESP_OK) {
    Serial.println("Gagal menambahkan diri ke peer");
    return;
  }
#endif
}

void loop() {
  // Nilai untuk dikirim
  myData.id = 1;
  myData.x = random(0, 50);
  myData.y = random(0, 50);

  Serial.print("x:"); Serial.println(myData.x, DEC);
  Serial.print("y:"); Serial.println(myData.y, DEC);
  // Kirim pesan

#ifdef ESP32
  esp_err_t result;
#else
  int result;
#endif
  result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Permintaan pengiriman berhasil");
  }
  else {
    Serial.println("Gagal mengirim data");
  }
  delay(10000);
}
