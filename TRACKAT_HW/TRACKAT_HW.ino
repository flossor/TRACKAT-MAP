/**
 * TRACKAT
 */
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

int const RX_SIM        = 10;
int const TX_SIM        = 9;

int const RX_GPS        = 3;
int const TX_GPS        = 4;

int const SERIAL_BAUD   = 9600;
int const SIM_BAUD      = 9600;
int const GPS_BAUD      = 9600;


SoftwareSerial sim(RX_SIM, TX_SIM);
SoftwareSerial GPSserial(RX_GPS, TX_GPS);

TinyGPSPlus gps;

String const SERVER_NBR = "+14175573752";
int const DEVICE_ID  = 0;

int const DELAY_MS      = 10*1000;
int currentTime         = 0;

double longitude        = 0;
double latitude         = 0;
double battery          = 0;

void setup() {

  delay(2000);
  Serial.begin(9600);
  sim.begin(9600);
  GPSserial.begin(GPS_BAUD);
  delay(1000);
  currentTime = millis();
  

}

void loop() {

  getPosition();

  if (millis() - currentTime > DELAY_MS){
    transmit();
    currentTime = millis();
  }
  Serial.print((millis() - currentTime)/1000);
  Serial.println(" Seconds");
  
  
  

}

void getPosition(){
  while (GPSserial.available() > 0){
    if (gps.encode(GPSserial.read())){
      if (gps.location.isUpdated()){
//        Serial.print("Latitude: ");
//        Serial.println(gps.location.lat(),6);
//        
//        Serial.print("Longitude: ");
//        Serial.println(gps.location.lng(),6);
        longitude = gps.location.lng();
        latitude = gps.location.lat();
      }
    }
  }
  
}


void transmit(){
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + SERVER_NBR + "\"\r"); //Mobile phone number to send message
  delay(200);
  String data = "{\"id\":\""+String(DEVICE_ID)+"\",\n\r\"lat\":"+String(latitude,6)+",\n\r\"long\":"+String(longitude,6)+",\n\r\"battery\":"+String(battery,2)+"}";
  sim.println(data);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  Serial.println("Transmitted");
  Serial.println(data);
}
