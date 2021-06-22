// This code was used to determine the states of a door and 
// two windows at P2
// Robodyn ESP8266 NodeMCU was used

#include <OOCSI.h>
#include <ArduinoJson.h>
#include "Adafruit_Sensor.h"
#include <DHT.h>
#include <DHT_U.h>

/*********************************************************************
   Example of the ESP connecting to WiFi and accessing items in an
   Entity dataset on the Data Foundry.
   This works only with an existing account, project and dataset on
   the Data Foundry (https://data.id.tue.nl)
 *********************************************************************/

#include "DFDataset.h"

//Timer values
const long uploadINT = 1000;

//Door(s)
const int switchDOOR = 2;
bool DOORopen = false;

//Window(s)
const int switchWIND1 = 5;
bool WINDopen1 = false;

const int switchWIND2 = 4;
bool WINDopen2 = false;


//Humidity / Temperature
#define DHTPIN 13
#define DHTTYPE DHT22   // DHT 22 (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

// ------------ by OOCSI
// SSID of your Wifi network, the library currently does not support WPA2 Enterprise networks
const char* ssid = "YOUR SSID HERE";
// Password of your Wifi network.
const char* password = "YOUR PASSWORD HERE";

// put the adress of Data Foundry here
const char* datafoundry = "data.id.tue.nl";

// create connection to dataset with server address, dataset id, and the access token
DFDataset iot(datafoundry, "YOUR TOKEN-ID HERE", "YOUR TOKEN HERE");

// put your setup code here, to run once:
void setup() {
  Serial.begin(115200);
  pinMode(switchDOOR, INPUT);
  pinMode(switchWIND1, INPUT);
  pinMode(switchWIND2, INPUT);
  
  // establish Wifi connection
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print('.');
  }

  Serial.println("Connected to the WiFi network");

  dht.begin();
   
  // output OOCSI activity on the built-in LED
  pinMode(LED_BUILTIN, OUTPUT);
  iot.setActivityLEDPin(LED_BUILTIN);
  // you can also already set the device and activity here,
  // if they don't change throughout the program execution
  //  iot.device("mydevice");
  //  iot.activity("idle");
}

void loop() {
  int DOORstat = digitalRead(switchDOOR);
  int WINDstat1 = digitalRead(switchWIND1);
  int WINDstat2 = digitalRead(switchWIND2);

  //DOOR
    if (DOORstat == HIGH){
        DOORopen = false;
      } else if (DOORstat == LOW ){
        DOORopen = true;
      }

  //WINDOWS
    if (WINDstat1 == HIGH){
        WINDopen1 = false;
      } else if (WINDstat1 == LOW ){
        WINDopen1 = true;
      }

    if (WINDstat2 == HIGH){
        WINDopen2 = false;
      } else if (WINDstat2 == LOW ){
        WINDopen2 = true;
      }

//------------------------------------------------------------------------
// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float fahrenheit_temperature = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature) || isnan(fahrenheit_temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float heat_index_fahrenheit = dht.computeHeatIndex(fahrenheit_temperature, humidity);
  // Compute heat index in Celsius (isFahreheit = false)
  float heat_index_celsius = dht.computeHeatIndex(temperature, humidity, false);

  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperature);
  Serial.print(F("°C "));
  Serial.print(fahrenheit_temperature);
  Serial.print(F("°F  Heat index: "));
  Serial.print(heat_index_celsius);
  Serial.print(F("°C "));
  Serial.print(heat_index_fahrenheit);
  Serial.println(F("°F"));
//--------------------------------------------------------------------------
  
  // specify device, can be empty
  iot.device("YOUR DEVICE ID HERE"); 

  // specify activity, can be empty
  iot.activity("idle");

  // fill in some data for the item (very similar to OOCSI)
  iot.addFloat("HumidityKzero", humidity);
  iot.addFloat("TemperatureKzero", temperature);
  iot.addFloat("HICKzero", heat_index_celsius);
  
  if(DOORopen == true){
    iot.addInt("DoorStatKzero", 0);
    Serial.println("DoorOpen");
  } else {
    iot.addInt("DoorStatKzero", 1);
    Serial.println("DoorClosed");
  }
  if(WINDopen1 == true){
    iot.addInt("Window1StatKzero", 0);
    Serial.println("Window1Open");
  } else {
    iot.addInt("Window1StatKzero", 1);
    Serial.println("Window1Closed");
  }
  if(WINDopen2 == true){
    iot.addInt("Window2StatKzero", 0);
    Serial.println("Window2Open");
  } else {
    iot.addInt("Window2StatKzero", 1);
    Serial.println("Window2Closed");
  }

  
  iot.addLong("time", (long) millis());
  
  // log the item data to the dataset
  iot.logItem();

  delay(uploadINT);
}
