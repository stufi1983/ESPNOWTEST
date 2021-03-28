#include <AntaresESP8266HTTP.h>
#include <ArduinoJson.h>

#define ACCESSKEY "your-access-key"       // Ganti dengan access key akun Antares anda
#define WIFISSID "ELCONS"         // Ganti dengan SSID WiFi anda
#define PASSWORD "elconsware"     // Ganti dengan password WiFi anda

#define projectName "KONSEN"   // Ganti dengan application name Antares yang telah dibuat
#define deviceName "KONSEN-DEV"     // Ganti dengan device Antares yang telah dibuat

AntaresESP8266HTTP antares(ACCESSKEY);    // Buat objek antares
void setup() {
  Serial.begin(115200);     // Buka komunikasi serial dengan baudrate 115200
  antares.setDebug(true);   // Nyalakan debug. Set menjadi "false" jika tidak ingin pesan-pesan tampil di serial monitor
  antares.wifiConnection(WIFISSID,PASSWORD);  // Mencoba untuk menyambungkan ke WiFi
}

void loop() {
  // Isi variabel dengan nilai acak, dengan tipe data yang berbeda
  int x = random(25,30) ;
  int y = random(75,90);
  float lati = float(random(20, 30))/3.33;
  float longi = float(random(0, 20))/6.99;
  String lat = "-6.8718189";
  String lon = "107.5872477";

  // Memasukkan nilai-nilai variabel ke penampungan data sementara
  antares.add("x", x);
  antares.add("x", y);
  antares.add("Lati", lati);
  antares.add("Longi", longi);
  antares.add("latitude", lat);
  antares.add("longitude", lon);

  // Kirim dari penampungan data ke Antares
  antares.send(projectName, deviceName);
  delay(3000);
}
