// LIBRARIES
#include <Arduino.h>
#include "Wire.h"
#include "SPI.h"
#include "math.h"
#include "Adafruit_Sensor.h"    // Adafruit sensor library
#include "Adafruit_AM2320.h"    // AM2320 library
#include "Adafruit_BMP3XX.h"    // BMP3XX library
#include <ESP8266WiFi.h>        // Include the Wi-Fi library

/* webserver libraries*/
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include <Arduino_JSON.h>

// DEFINES
/* DEFINE PINOUT FOR SCK, MISO, MOSI, CS */
#define BMP_SCK 14
#define BMP_MISO 12
#define BMP_MOSI 13
#define BMP_CS 15

/* DEFINE SEALEVELPRESSURE FOR YOUR LOCATION */
#define SEALEVELPRESSURE_HPA (1021) // adjust for your location

/* REPLACE WITH YOUR NETWORK CREDENTIALS */
const char* ssid = "wifi";
const char* password = "password";

/* INITIALIZE OBJECTS */
// INITIALIZE AN AM2320 OBJECT
Adafruit_AM2320 am2320 = Adafruit_AM2320();
/* INITIALIZE AN BMP388 OBJECT */
Adafruit_BMP3XX bmp;
/* CREATE ASYNCWEBSERVER OBJECT ON PORT 80 */
AsyncWebServer server(80);
/* CREATE AN EVENT SOURE ON /events */
AsyncEventSource events("/events");
/* JSON VARIABLE TO HOLD SENSOR READINGS */
JSONVar readings;

/* GLOBAL VARIABLES */
/* TIMER/MILLIS VARIABLES */
unsigned long lastTimeUpdatedWebserver = 0;
unsigned long timerDelayWebserverUpdate = 3000;   // delay for webserver update
unsigned long lastTimeUpdatedArray = 0;
unsigned long timerDelayUpdatingArray = 3600000;   // 60min delay for data collection 60000 = 1 minute, 
/* SET PINOUT FOR ADC */
const int analogInPin = A0;                   // ESP8266 Analog Pin ADC0 = A0

/* DATA ARRAYS TO HOLD LAST arrayLen sensor readings */
const int arrayLen = 12;
float am2320temperatures [arrayLen] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0 , 0};
float am2320humidities [arrayLen] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0 , 0};
float bmp388temperatures [arrayLen] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0 , 0};
float bmp388pressures [arrayLen] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0 , 0};

/* FUNCTIONS */
/* SENSOR FUNCTIONS */
/* Initialize BMP388 */
void initBMP388(){
  //if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
  //if (! bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode  
  if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  // Set up oversampling, filter initialization, and data rate
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);   // set oversampling for temperature
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);      // set oversampling for pressure 
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);         // set IIR filte coefficient
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);                  // set output datarate
}
// Read and return AM2320 data
// Read and return AM2320 temperature
float get_am2320_temp(){
  return am2320.readTemperature();
}
// Read and return AM2320 humidity
float get_am2320_humid(){
  return am2320.readHumidity();
}
// Read and return BMP388 data
// Read and return BMP388 temp
float get_bmp388_temp(){
  if (!bmp.performReading()) {
  Serial.println("Failed to perform BMP388 temperature reading :(");
  return -1;
  }
  return bmp.temperature;
}
// Read and return BMP388 pressure
float get_bmp388_pressure(){
  if (!bmp.performReading()) {
  Serial.println("Failed to perform BMP388 pressure reading :(");
  return -1;
  }
  return bmp.pressure / 100.0;
}
// Read and return BMP388 altitude
float get_bmp388_altitude(){
  // check that bmp reading can be performed
  if (!bmp.performReading()) {
  Serial.println("Failed to perform BMP388 altitude reading :(");
  return -1;
  }
  return bmp.readAltitude(SEALEVELPRESSURE_HPA);
}

/* Function for collecting data over time*/
void collect_data(){
  // Shift all values one position in all data arrays
  for(int i=0; i<arrayLen-1 ; i++){
    am2320temperatures[i] = am2320temperatures[i+1];  // shift am2320temp
    am2320humidities[i] = am2320humidities[i+1];      // shift am2320humid
    bmp388temperatures[i] = bmp388temperatures[i+1];  // shift bmp388temp
    bmp388pressures[i] = bmp388pressures[i+1];        // shift bmp388press
  }
  // Insert new values
  am2320temperatures[arrayLen - 1] = get_am2320_temp();    // new value in am2320 temp
  am2320humidities[arrayLen - 1] = get_am2320_humid();      // new value am2320 humidity
  bmp388temperatures[arrayLen - 1] = get_bmp388_temp();  // new value bmp388 temp
  bmp388pressures[arrayLen - 1] = get_bmp388_pressure();     // new value bmp388 pressure
} 

// debug print
void print_collected_data(){
  Serial.println("data");
  for(int i = 0; i < 12; i++){
    Serial.print(" am2320temp: ");
    Serial.print(am2320temperatures[i]);
    Serial.print(" am2320humid: ");
    Serial.print(am2320humidities[i]);
    Serial.print(" bmp388temp: ");
    Serial.print(bmp388temperatures[i]);
    Serial.print(" bmp388pressure: ");
    Serial.println(bmp388pressures[i]);
  }
}
/* WebServer Handling Function */
// Get Sensor Readings and return JSON object
String getSensorReadings(){
  // am2320 temp
  readings["AM2320temp0"] = am2320temperatures[0];
  readings["AM2320temp1"] = am2320temperatures[1];
  readings["AM2320temp2"] = am2320temperatures[2];
  readings["AM2320temp3"] = am2320temperatures[3];
  readings["AM2320temp4"] = am2320temperatures[4];
  readings["AM2320temp5"] = am2320temperatures[5];
  readings["AM2320temp6"] = am2320temperatures[6];
  readings["AM2320temp7"] = am2320temperatures[7];
  readings["AM2320temp8"] = am2320temperatures[8];
  readings["AM2320temp9"] = am2320temperatures[9];
  readings["AM2320temp10"] = am2320temperatures[10];
  readings["AM2320temp11"] = am2320temperatures[11];
  // bmp388 temp
  readings["BMP388temp0"] = bmp388temperatures[0];
  readings["BMP388temp1"] = bmp388temperatures[1];
  readings["BMP388temp2"] = bmp388temperatures[2];
  readings["BMP388temp3"] = bmp388temperatures[3];
  readings["BMP388temp4"] = bmp388temperatures[4];
  readings["BMP388temp5"] = bmp388temperatures[5];
  readings["BMP388temp6"] = bmp388temperatures[6];
  readings["BMP388temp7"] = bmp388temperatures[7];
  readings["BMP388temp8"] = bmp388temperatures[8];
  readings["BMP388temp9"] = bmp388temperatures[9];
  readings["BMP388temp10"] = bmp388temperatures[10];
  readings["BMP388temp11"] = bmp388temperatures[11];
  // am2320 hum
  readings["AM2320hum0"] = am2320humidities[0];
  readings["AM2320hum1"] = am2320humidities[1];
  readings["AM2320hum2"] = am2320humidities[2];
  readings["AM2320hum3"] = am2320humidities[3];
  readings["AM2320hum4"] = am2320humidities[4];
  readings["AM2320hum5"] = am2320humidities[5];
  readings["AM2320hum6"] = am2320humidities[6];
  readings["AM2320hum7"] = am2320humidities[7];
  readings["AM2320hum8"] = am2320humidities[8];
  readings["AM2320hum9"] = am2320humidities[9];
  readings["AM2320hum10"] = am2320humidities[10];
  readings["AM2320hum11"] = am2320humidities[11]; 
  // bmp388 press
  readings["BMP388press0"] = bmp388pressures[0];
  readings["BMP388press1"] = bmp388pressures[1];
  readings["BMP388press2"] = bmp388pressures[2];
  readings["BMP388press3"] = bmp388pressures[3];
  readings["BMP388press4"] = bmp388pressures[4];
  readings["BMP388press5"] = bmp388pressures[5];
  readings["BMP388press6"] = bmp388pressures[6];
  readings["BMP388press7"] = bmp388pressures[7];
  readings["BMP388press8"] = bmp388pressures[8];
  readings["BMP388press9"] = bmp388pressures[9];
  readings["BMP388press10"] = bmp388pressures[10];
  readings["BMP388press11"] = bmp388pressures[11];
  // timedelay between sensor readings
  readings["timedelay"] = timerDelayUpdatingArray;
  
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

// Initialize LittleFS
void initLittleFS() {
  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  else{
    Serial.println("LittleFS mounted successfully");
  }
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  // SET PINS
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(A0, INPUT);
  // Init functions
  initWiFi();           // initialize WiFi
  initLittleFS();       // Initialize LittleFS
  // initialize sensors
  am2320.begin(); // Initialize am2320
  initBMP388();   // Initialize BMP388
  collect_data(); // collect some data on startup to have something interesting to publish on webpage
  
  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.serveStatic("/", LittleFS, "/");
  
  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    lastTimeUpdatedWebserver = lastTimeUpdatedWebserver + timerDelayWebserverUpdate;
    request->send(200, "text/plain", getSensorReadings().c_str());
  });

  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  // Start server
  server.begin();
}

void loop() {
  /* GRAB SENSOR READINGS EVERY timerDelayUpdatingArray miliseconds and store in data arrays*/
  if ((millis() - lastTimeUpdatedArray) > timerDelayUpdatingArray) {
    lastTimeUpdatedArray = millis();
    digitalWrite(LED_BUILTIN, LOW);
    collect_data();
    // let the LED shine bright for some time to get feedback from station during sampling
    delay(500);
  }
  digitalWrite(LED_BUILTIN, HIGH);
}
