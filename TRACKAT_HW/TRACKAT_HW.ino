/**
 * TRACKAT
 */
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

int const RX_SIM        = 8;
int const TX_SIM        = 7;

int const RX_GPS        = 3;
int const TX_GPS        = 4;

int const SERIAL_BAUD   = 9600;
int const SIM_BAUD      = 9600;
int const GPS_BAUD      = 9600;


SoftwareSerial sim(RX_SIM, TX_SIM);
SoftwareSerial GPSserial(RX_GPS, TX_GPS);

String const SERVER_NBR = "+14175573752";
String const DEVICE_ID  = "34en094nrng";

int const DELAY_MS      = 5*60*1000;
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
  }
  
  
  

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
  String data = "{\"id\":\""+DEVICE_ID+"\",\n\r\"lat\":"+String(latitude,6)+",\n\r\"long\":"+String(longitude,6)+",\n\r\"battery\":"+String(battery,2)+"}";
  sim.println(data);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  Serial.println("Transmitted");
}
