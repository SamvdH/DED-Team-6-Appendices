// This code was re-used for three data collection modules:
//     - two located at P1 for which RoboDyn ESP8266 NodeMCU were used
//     - one located at P2 for which Demos D1 Mini ESP32 was used
// Strings for SSID, password, Token-ID, Token, DHT column headers and 
// door specification column headers were renamed accordingly for each 
// data collection device
// 
// Columnheaders in this document demonstrate headers specified for the
// data colleciton device in a living room next the door to the kitchen
// (i.e. HumidityKtest, KitchenDoor)
//
// Other data collection devices changed accordingly:
// (e.g. HumidityKtest --> HumidityEzero) 
// (     KitchenDoor --> EntranceDoor   )
//
// Indicated with: String was renamed accordingly for each data collection device

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
//Data for testgroup
#include "DFDataset.h"

//Timer values
const long uploadINT = 1000;

//DOORstat(s)
const int switchDOOR = 4;
bool DOORopen = false;

//Humidity / Temperature
#define DHTPIN 13
#define DHTTYPE DHT22   // DHT 22 (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

// -------------- by OOCSI
// SSID of your Wifi network, the library currently does not support WPA2 Enterprise networks
const char* ssid = "YOUR SSID HERE";                                          //String was renamed accordingly for each data collection device
// Password of your Wifi network.
const char* password = "YOUR PASSWORD HERE";                                  //String was renamed accordingly for each data collection device

// put the adress of Data Foundry here
const char* datafoundry = "data.id.tue.nl";

// create connection to dataset with server address, dataset id, and the access token
DFDataset iot(datafoundry, "YOUR TOKEN-ID HERE", "YOUR TOKEN HERE");          //Strings were renamed accordingly for each data collection device

// put your setup code here, to run once:
void setup() {
  Serial.begin(115200);
  pinMode(switchDOOR, INPUT);
  
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

  //DOOR
    if (DOORstat == HIGH){
        DOORopen = false;
      } else if (DOORstat == LOW ){
        DOORopen = true;
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
  iot.device("YOUR DEVICE ID HERE");                  //String was renamed accordingly for each data collection device

  // specify activity, can be empty
  iot.activity("idle");

  // fill in some data for the item (very similar to OOCSI)
  iot.addFloat("HumidityKtest", humidity);            //String was renamed accordingly for each data collection device
  iot.addFloat("TemperatureKtest", temperature);      //String was renamed accordingly for each data collection device
  iot.addFloat("HICKtest", heat_index_celsius);       //String was renamed accordingly for each data collection device

  
  if(DOORopen == true){
    iot.addInt("KitchenDoortest", 0);                 //String was renamed accordingly for each data collection device
    Serial.println("DoorOpen");
  } else {
    iot.addInt("KitchenDoortest", 1);                 //String was renamed accordingly for each data collection device
    Serial.println("DoorClosed");
  }

  iot.addLong("time", (long) millis());
  
  // log the item data to the dataset
  iot.logItem();

  delay(uploadINT);
}
