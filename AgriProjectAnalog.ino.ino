/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"

//  DHT sensor type you're using!
#define DHTTYPE DHT11  

#define VOLTAGE_MAX 1.0
#define VOLTAGE_MAXCOUNTS 1023.0
// Replace with your network details
const char* ssid = "Honorsom";
const char* password = "sunny007";
unsigned long myChannelNumberInput = 228461;
unsigned long myChannelNumberOutput= 227051;
const char * myWriteAPIKey = "JILCDTW6ZP60HZVN";
// DHT Sensor
const int DHTPin = 5;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

WiFiClient  client;
// only runs once on boot
void setup() {
    pinMode(16,OUTPUT);//MOTOR
    digitalWrite(16, LOW);
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);

  dht.begin();
  
  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Starting the web server
  ThingSpeak.begin(client);
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
}

// runs over and over again
void loop() {
  int mot = ThingSpeak.readIntField(myChannelNumberInput,2);
  
  if(mot==1)
  {
      digitalWrite(16, HIGH);
      Serial.println("Motor ON");
  }
  else
  { 
      digitalWrite(16, LOW);
      Serial.println("Motor OFF");
  }
   int sensorValue = analogRead(A0);
   Serial.println(sensorValue);
  // Convert the analog reading 
  // On Uno,Mega,YunArduino:  0 - 1023 maps to 0 - 5 volts
  // On ESP8266:  0 - 1023 maps to 0 - 1 volts
  // On MKR1000,Due: 0 - 4095 maps to 0 - 3.3 volts
   //int randNumber = random(0,9);
  // Serial.println(randNumber*0.01);
  //float voltage = (sensorValue * (VOLTAGE_MAX / VOLTAGE_MAXCOUNTS))+5+(randNumber*0.01);
  float voltage =(1-(sensorValue * (VOLTAGE_MAX / VOLTAGE_MAXCOUNTS)))*100;

  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  Serial.println(voltage);
  ThingSpeak.setField(3,voltage);
  Serial.println("Channel Updated");
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    strcpy(celsiusTemp,"Failed");
    strcpy(fahrenheitTemp, "Failed");
    strcpy(humidityTemp, "Failed");         
  }
  else{
       // Computes temperature values in Celsius + Fahrenheit and Humidity
              //float hic = dht.computeHeatIndex(t, h, false);       
             // dtostrf(hic, 6, 2, celsiusTemp);             
             // float hif = dht.computeHeatIndex(f, h);
             // dtostrf(hif, 6, 2, fahrenheitTemp);         
              dtostrf(h, 6, 2, humidityTemp);
              // You can delete the following Serial.print's, it's just for debugging purposes
              Serial.print("Humidity: ");
              Serial.print(h);
              Serial.print(" %\t Temperature: ");
              Serial.print(t);
              Serial.print(" *C ");
              Serial.print(f);
              Serial.print(" *F");
              //Serial.print(" *F\t Heat index: ");
              //Serial.print(hic);
              //Serial.print(" *C ");
              //Serial.print(hif);
             // Serial.print(" *F");
               Serial.println("");
             
              ThingSpeak.setField(1,h);
                  Serial.println("Updated humudity");
              ThingSpeak.setField(2,t);
             Serial.println("Updated temperature");

            }   
    ThingSpeak.writeFields(myChannelNumberOutput, myWriteAPIKey);
    delay(15000);  
  }   
          
      
       
