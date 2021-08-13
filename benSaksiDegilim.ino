#include "DHT.h"
#include <SoftwareSerial.h>
SoftwareSerial saksiBaglanti(7,6);

#define nemSensoru A0
#define yagmurSensoru A1
#define dhtPin 2
#define motorPin 3

#define dhtTip DHT11
DHT dht(dhtPin, dhtTip);

char veri = '9'; //bluetoothdan gelen verileri saklar

void setup(){
  pinMode(motorPin, OUTPUT); //motor pinini çıkış olarak ayarlar
  Serial.begin(9600); //bilgisayar ile olan haberleşme başlar
  saksiBaglanti.begin(9600); //bluetooth ile olan haberleşme başlar
  dht.begin();

  Serial.println("Degerler okunuyor..");
  saksiBaglanti.println("Degerler okunuyor..");
}

void loop(){
  //VERİLER OKUNUYOR
  int toprakNem = analogRead(nemSensoru);
  int yagmur = analogRead(yagmurSensoru);
  float ortamNem = dht.readHumidity();
  float ortamSicaklik = dht.readTemperature();

  //VERİLERİN ANLAMDIRILMASI
  toprakNem = (1024 - toprakNem) / 10; //0 ile 1023 arasında gelen veri % olarak kaydedilir

  //HC-05'den VERİ ALMA
  if(saksiBaglanti.available()){
    veri = saksiBaglanti.read();
  }

  //ALINAN VERİYE GÖRE GÖSTERİLECEK ÇIKTILAR
  switch(veri){
    case '0': //sadece toprağın nem durumu gönderilir
      saksiBaglanti.print("Toprak Nemi: %");
      saksiBaglanti.println(toprakNem);
      break;
    case '1': //sadece yağmur durumu gönderilir
      if(yagmur < 650){
        saksiBaglanti.println("Yagmur Durumu: Yagis Var!");
      }
      else{
        saksiBaglanti.println("Yagmur Durumu: Yagis Yok!");
      }
      break;
    case '2': //sadece ortamın nem ve sıcaklık değerleri gönderilir
      saksiBaglanti.print("Ortam Sicakligi: ");
      saksiBaglanti.print(ortamSicaklik);
      saksiBaglanti.println("C");
      saksiBaglanti.print("Ortam Nemi: ");
      saksiBaglanti.println(ortamNem);
      break;
    default: // tüm bilgiler gönderilir
      saksiBaglanti.print("Toprak Nemi: %");
      saksiBaglanti.println(toprakNem);
      saksiBaglanti.print("Ortam Sicakligi: ");
      saksiBaglanti.print(ortamSicaklik);
      saksiBaglanti.println("C");
      saksiBaglanti.print("Ortam Nemi: ");
      saksiBaglanti.println(ortamNem);
      if(yagmur < 650){
        saksiBaglanti.println("Yagmur Durumu: Yagis Var!");
      }
      else{
        saksiBaglanti.println("Yagmur Durumu: Yagis Yok!");
      }
      break;
  }

  //VERİLERE GÖRE SAKSININ KENDİ KENDİNE ALACAĞI ÖNLEMLER ve UYARILAR
  if(yagmur < 650){
    saksiBaglanti.println("!! YAGMUR VAR!! DIKKAT EDIN!!");
  }

  if(toprakNem < 40){
    digitalWrite(motorPin, HIGH);
    saksiBaglanti.println("!! CICEK SULANIYOR !!");
    delay(3000);
    digitalWrite(motorPin, LOW);
    delay(1000);
  }
  else{
    digitalWrite(motorPin, LOW);
  }

  if(ortamSicaklik > 40){
    saksiBaglanti.println("!!ORTAM SICAKLIGI YUKSEK!! DIKKAT EDIN!!");
  }
  else if(ortamSicaklik < 10){
    saksiBaglanti.println("!!ORTAM SICAKLIGI DUSUK!! DIKKAT EDIN!!");
  }

  saksiBaglanti.println("****************");
  delay(2000);
}
