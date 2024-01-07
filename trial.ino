#include <ESP8266WiFi.h>                 
#include <FirebaseArduino.h>      
#include <DHT.h>              
 
#define FIREBASE_HOST "sgp108-24de7-default-rtdb.firebaseio.com"      
#define FIREBASE_AUTH "YysKombeQnn473V2aggQVFj1S8uUahl9RZEMb3ms"            
#define WIFI_SSID "SA"                                  
#define WIFI_PASSWORD "aashakashachi"            
 
#define DHTPIN D4                                            // Digital pin connected to DHT11
#define DHTTYPE DHT11                                        
DHT dht(DHTPIN, DHTTYPE);                                                    

int TRIGGER = D3;
int ECHO = D2;
int data;

void setup() 
{
  pinMode(TRIGGER, OUTPUT); 
  pinMode(ECHO, INPUT);
  Serial.begin(115200);
  dht.begin();                                                   
               
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                  
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
 
  Serial.println();
  Serial.print("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());                              
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                 
 
}
 
void loop() 
{
//TEMPERATURE-HUMIDITY-------------------------------------------------------------------------------------------------------------------------------  
  float h = dht.readHumidity();                                 
  float t = dht.readTemperature();                              
  
  if (isnan(h) || isnan(t))                                     
  {                                   
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  } 
  Serial.print("Humidity: ");  
  Serial.print(h);
  float fireHumid = h;
  //String fireHumid = String(h) + String("%");                   
  
  Serial.print("%  Temperature: ");  
  Serial.print(t);  
  Serial.println("°C ");
  float fireTemp = t;
  //String fireTemp = String(t) + String("°C");                  
  delay(1000);
  
//SOIL-MOISTURE---------------------------------------------------------------------------------------------------------------------------------

  Serial.print("MOISTURE LEVEL : ");
  value= analogRead(sense_Pin);
  value= value/10;
  Serial.println(value);
  int fireSoil = value;
  delay(1000);
  
//Water-level---------------------------------------------------------------------------------------------------------------------------------

  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIGGER, LOW);
  long duration = pulseIn(ECHO, HIGH);
  data = (duration/2) / 29.09;
  Serial.print("Water Level:");
  Serial.println(data);
  int fireWater = data;
 // String fireWater = String(data)+ String("cm");
  delay(1000);
  
//--------------------------------------------------------------------------------------------------------------------------------- 
  //setup path
  Firebase.setFloat("/Data/Humidity", fireHumid);            
  Firebase.setFloat("/Data/Temperature", fireTemp);         
  Firebase.setInt("/Data/Soil-Moisture", fireSoil);
  Firebase.setInt("/Data/Light-Intensity", fireLight);
  Firebase.setInt("/Data/Water-Level", fireWater);
    if (Firebase.failed()) 
    {
 
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error()); 
      return;
  }
}
