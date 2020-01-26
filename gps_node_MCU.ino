#include <TinyGPS++.h> 
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
TinyGPSPlus gps;  
SoftwareSerial ss(4, 5); 
const char* ssid = "rezvee";
const char* password = "12345678"; 
float latitude , longitude;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str;
int pm;

#include <FirebaseArduino.h>
#define FIREBASE_HOST "driverinfo-13e5c.firebaseio.com"
#define FIREBASE_AUTH "9pyCfq15GROGiN8Ltr12or8dyg6PI5j6sOsyifDx"

#include <Wire.h>


void setup()
{
  Serial.begin(9600);
  ss.begin(9600);
  Wire.begin();  

  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("."); //print "...."
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void firebasereconnect()
{
  Serial.println("Trying to reconnect");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
    



void loop()
{
  Wire.requestFrom(8, 7); 
  if (Firebase.failed())
  {
    Serial.print("setting number failed:");
    Serial.println(Firebase.error());
    firebasereconnect();
    return;
  }

  Serial.println("Connect to Firebase");

  //RFID start
  
  String rfidNo="";
  int rr=0;   

while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
  if(rr==1){
    Serial.println(rfidNo);
  Firebase.setString("/vehicle/vehicle1/driverId/", rfidNo);
  
  }

  Serial.println("End of RFID");
  

  //RFID end

  
  
  while (ss.available() > 0) 
    if (gps.encode(ss.read())) 
    {
      if (gps.location.isValid()) 
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6); 
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
        String re= lat_str+"+"+lng_str;
        Serial.println(re);
        Firebase.setString("/vehicle/vehicle1/location/", re);
        Firebase.setString("/vehicle/vehicle2/location/", re);
      }
      
    }



 
  delay(100);
}
