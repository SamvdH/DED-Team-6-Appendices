// Basis used for this sketch is: "Example testing sketch for various DHT humidity/temperature sensors written by ladyada, modified by Eva van der Born".
// Code, Comments and Guide edited and updated by Sam van der Horst using NeoPixelBus and ESP servo library examples.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Library: https://github.com/adafruit/Adafruit_Sensor
// - OOCSI for ESP Library: https://github.com/iddi/oocsi-esp
// - ESP servo library: https://github.com/RoboticsBrno/ServoESP32
// - NeoPixelBus library: https://github.com/Makuna/NeoPixelBus

  #include "OOCSI.h"
  #include "DHT.h"
  #include "Adafruit_Sensor.h"
  #include "Servo.h"
  #include "NeoPixelBus.h"

// Digital pin connected to the DHT sensor
  #define DHTPIN 13


// Uncomment whatever type you're using, I use a DHT11
  #define DHTTYPE DHT11   // DHT 11
  
//#define DHTTYPE DHT22   // DHT 22 (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

//DHT connection instructions:  
  // Connect pin 1 (on the left) of the sensor to +5V
    // NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1 to 3.3V instead of 5V!
  // Connect pin 2 of the sensor to whatever your DHTPIN is (we use pin 13 on the Wemos Lolin esp32)
  // Connect pin 4 (on the right) of the sensor to GROUND
  // Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor 
    //NOTE:Not always necessary, check your data through the OOCSI dashboard.

// Initialize DHT sensor.
  // Note that older versions of this library took an optional third parameter to
  // tweak the timings for faster processors.  This parameter is no longer needed
  // as the current DHT reading algorithm adjusts itself to work on faster procs.

  DHT dht(DHTPIN, DHTTYPE);

// FOR OOCSI CONNECTION:
// We set up an OOCSI connection so we can monitor and upload the values of the DHT sensor.
// use this if you want the OOCSI-ESP library to manage the connection to the Wifi

// SSID of your Wifi network, the library currently does not support WPA2 Enterprise networks
  const char* ssid = "YOUR SSID HERE";
// Password of your Wifi network.
  const char* password = "YOUR PASSWORD HERE";

// name for connecting with OOCSI (unique handle)
  const char* OOCSIName = "YOUR_OWN_OOCSI_HANDLE";
// put the adress of your OOCSI server here, can be URL or IP address string
  const char* hostserver = "OOCSI.SERVER.NAME"; //oocsi.id.tue.nl

// OOCSI reference for the entire sketch
// To connect to an OOCSI network, that is, a running OOCSI server, you first need to create an OOCSI reference:
  OOCSI oocsi = OOCSI();

// Define Servo pin and name:
  static const int servoPin = 12;

  Servo servo1;

// Define led ring
  const uint16_t PixelCount = 8; // this example assumes 8 pixels, making it smaller will cause a failure
  const uint8_t PixelPin = 14;  // make sure to set this to the correct pin, ignored for Esp8266

  #define colorSaturation 128

  NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

  RgbColor white(colorSaturation); //basic color to start with.

//operating CODE:
  void setup() {
   servo1.attach(servoPin);
  
   Serial.begin(115200);
 
   Serial.println(F("DHTxx test!"));
   dht.begin();
   
  // output OOCSI activity on the built-in LED
   pinMode(LED_BUILTIN, OUTPUT);
    oocsi.setActivityLEDPin(LED_BUILTIN);
   oocsi.connect(OOCSIName, hostserver, ssid, password);

    while (!Serial); // wait for serial attach

    // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();
    
  }


  void loop() {
  // Wait 4 sec. between measurements. (make sure it is at least 2 seconds, the DHT11 is a slow sensor)
    delay(4000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor).
    float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
    float temperature = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
    float fahrenheit_temperature = dht.readTemperature(true);

  //Base the intensity of the light on the value of the DHT sensor
  //Map these values so when humidity is getting lower, sensor will be an increasingly brighter hue of Blue
    int BlueHumid = map(humidity, 0, 95, 255, 0);
    
  //Map these values so when humidity is getting higher, sensor will be an increasingly brighter hue of Red  
    int RedHumid = map(humidity, 0, 95, 0, 255);

  //Define these as colors for the LED
    RgbColor red(RedHumid, 5, 5);
    RgbColor blue(5, 5, BlueHumid);
    
  // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature) || isnan(fahrenheit_temperature)) {
     Serial.println(F("Failed to read from DHT sensor!"));
     return;
   }

  //The code that operates the Servo and LED for certain humidty values   
     // Setting of Servo and LED for Humidity is lower than 30 
     if (humidity < 30) {
       servo1.write(75);
       delay(20);

        strip.SetPixelColor(0, blue);
        strip.SetPixelColor(1, blue);
        strip.SetPixelColor(2, blue);
        strip.SetPixelColor(3, blue);
        strip.SetPixelColor(4, blue);
        strip.SetPixelColor(5, blue);
        strip.SetPixelColor(6, blue);
        strip.SetPixelColor(7, blue);
        strip.Show();
        delay(20);
      }

      // Setting of Servo and LED for Humidity is lower than 35 and higher or equal to 30 
      if (humidity < 35 && humidity >= 30) {
       servo1.write(70);
       delay(20);
       
        strip.SetPixelColor(0, blue);
        strip.SetPixelColor(1, blue);
        strip.SetPixelColor(2, blue);
        strip.SetPixelColor(3, blue);
        strip.SetPixelColor(4, blue);
        strip.SetPixelColor(5, blue);
        strip.SetPixelColor(6, blue);
        strip.SetPixelColor(7, blue);
        strip.Show();
        delay(20);
      }

      // Setting of Servo and LED for Humidity is lower than 40 and higher or equal to 35 
      if (humidity < 40 && humidity >= 35) {
       servo1.write(65);
       delay(20);
       
        strip.SetPixelColor(0, blue);
        strip.SetPixelColor(1, blue);
        strip.SetPixelColor(2, blue);
        strip.SetPixelColor(3, blue);
        strip.SetPixelColor(4, blue);
        strip.SetPixelColor(5, blue);
        strip.SetPixelColor(6, blue);
        strip.SetPixelColor(7, blue);
        strip.Show();
        delay(20);
      }
      
      // Setting of Servo and LED for Humidity is between 40 and 60
      if (humidity >= 40 && humidity < 60) {
       servo1.write(55);
       delay(20);

        strip.SetPixelColor(0, white);
        strip.SetPixelColor(1, white);
        strip.SetPixelColor(2, white);
        strip.SetPixelColor(3, white);
        strip.SetPixelColor(4, white);
        strip.SetPixelColor(5, white);
        strip.SetPixelColor(6, white);
        strip.SetPixelColor(7, white);
        strip.Show();
        delay(20); 
      }

      // Setting of Servo and LED for Humidity is lower than 65 and higher or equal to 60 
      if (humidity >= 60 && humidity < 65) {
       servo1.write(65);
       delay(20);
       
        strip.SetPixelColor(0, red);
        strip.SetPixelColor(1, red);
        strip.SetPixelColor(2, red);
        strip.SetPixelColor(3, red);
        strip.SetPixelColor(4, red);
        strip.SetPixelColor(5, red);
        strip.SetPixelColor(6, red);
        strip.SetPixelColor(7, red);
        strip.Show();
        delay(20);
     }

     // Setting of Servo and LED for Humidity is lower than 70 and higher or equal to 65 
     if (humidity >= 65 && humidity < 70) {
       servo1.write(70);
       delay(20);
       
        strip.SetPixelColor(0, red);
        strip.SetPixelColor(1, red);
        strip.SetPixelColor(2, red);
        strip.SetPixelColor(3, red);
        strip.SetPixelColor(4, red);
        strip.SetPixelColor(5, red);
        strip.SetPixelColor(6, red);
        strip.SetPixelColor(7, red);
        strip.Show();
        delay(20);
     }

    // Setting of Servo and LED for Humidity is lower than 75 and higher or equal to 70 
    if (humidity >= 70 && humidity < 75) {
       servo1.write(75);
       delay(20);
       
        strip.SetPixelColor(0, red);
        strip.SetPixelColor(1, red);
        strip.SetPixelColor(2, red);
        strip.SetPixelColor(3, red);
        strip.SetPixelColor(4, red);
        strip.SetPixelColor(5, red);
        strip.SetPixelColor(6, red);
        strip.SetPixelColor(7, red);
        strip.Show();
        delay(20);
     }
     
     // Setting of Servo and LED for Humidity is higher or equal to 75 
     if (humidity >= 75) {
       servo1.write(85);
       delay(20);
       
        strip.SetPixelColor(0, red);
        strip.SetPixelColor(1, red);
        strip.SetPixelColor(2, red);
        strip.SetPixelColor(3, red);
        strip.SetPixelColor(4, red);
        strip.SetPixelColor(5, red);
        strip.SetPixelColor(6, red);
        strip.SetPixelColor(7, red);
        strip.Show();
        delay(20);
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

  // make OOCSI message ready. Insert DF channel here
    oocsi.newMessage("DATAFOUNDRY-CHANNEL-NAME");

  // add data (primitive data types int, float, long, string)
  // the labels such as "count" or "timestamp" are completely free to choose

  // replace DEVICE with your reference ID from a device
    oocsi.addString("device_id", "DEVICE");
  
    oocsi.addFloat("Humidity", humidity);
    oocsi.addFloat("Temperature", temperature);
    oocsi.addFloat("HIC", heat_index_celsius);
    oocsi.addLong("time", (long) millis());

  // this command will send the message; don't forget to call this after creating a message
    oocsi.sendMessage();
  
  // prints out the raw message (how it is sent to the OOCSI server)
  // oocsi.printSendMessage();
  // needs to be checked in order for OOCSI to process incoming data.
    oocsi.check();
}
