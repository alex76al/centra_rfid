/*
* Read a card using a mfrc522 reader on your SPI interface
 * Pin layout should be as follows (on Arduino 2560):
 * MOSI: Pin 51 / ICSP-4    MISO: Pin 50 / ICSP-1    SCK: Pin 52 / ISCP-3
 * SS: Pin 45 sda             RST: Pin 9
 */

#include <SPI.h>
#include <RFID.h>
#include <LiquidCrystal.h>
#include <Ethernet.h>
#include <EthernetUdp.h> 

#define SS_PIN 45
#define RST_PIN 41


RFID rfid(SS_PIN, RST_PIN); 
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177), server(192, 168, 1, 100);
unsigned int listeningPort = 1701;
EthernetUDP Udp;


// Setup variables:
int serNum0;  int serNum1;  int serNum2;
int serNum3;  int serNum4;  char tCar[2];


void setup()
{ 
  Serial.begin(9600);
  SPI.begin(); 
  rfid.init();
  lcd.begin(16, 2);
  Ethernet.begin(mac,ip);
  Udp.begin(listeningPort);
  delay(150);
}


void loop()
{
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      serNum0 = rfid.serNum[0];
      serNum1 = rfid.serNum[1];
      serNum2 = rfid.serNum[2];
      serNum3 = rfid.serNum[3];
      serNum4 = rfid.serNum[4];

      lcd.setCursor(0,0);
      lcd.print("Cardnumber:");
      lcd.setCursor(0,1);
      Serial.println("Cardnumber:");
      
      for (int i=0; i<5; i++) {
        Serial.print(rfid.serNum[i],DEC);
        Serial.print(", ");
        lcd.print(rfid.serNum[i]);
        lcd.print(" ");
        }
    }
    delay(150);
    Udp.beginPacket(server, listeningPort);
    for (int i=0; i<5; i++) {
    itoa(rfid.serNum[i], tCar, 10);
    Udp.write(tCar);
    Udp.write(" ");}
    Udp.endPacket();

  }
  rfid.halt();
}


