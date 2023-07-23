/**
 * TRACKAT
 * 
 * Maxim Bacar
 * COEN 390
 */
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

int const RX_SIM        = 9;
int const TX_SIM        = 10;

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

unsigned long const TIME_DELAY      = 60000;
long lastTime         = 0;

double longitude        = -73.555457;
double latitude         = 45.56616;
double battery          = 0;

void setup() {

  delay(2000);
  Serial.begin(SERIAL_BAUD);
  sim.begin(SIM_BAUD);
  GPSserial.begin(GPS_BAUD);
  delay(1000);
  lastTime = millis();
  

}

void loop() {

  
  long timePassed = (millis() - lastTime);
 
  Serial.print(timePassed/1000);
  Serial.print("sec Since Last Transmission... | Current Location[");
  Serial.print(latitude);
  Serial.print(", ");
  Serial.print(longitude);
  Serial.println("]");

  getPosition();
  
  if (timePassed > TIME_DELAY){
    lastTime = millis();
    transmit();
    
  }
  
}

void getPosition(){
  while (GPSserial.available() > 0){
    if (gps.encode(GPSserial.read())){
      if (gps.location.isUpdated()){    
        longitude = gps.location.lng();
        latitude = gps.location.lat();

      }
    }
  }
  
}

/**
 * Transmit data via GSM to the server
 */
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
