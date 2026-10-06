#include <Arduino.h> //Libraries needed for this lab
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <WiFi.h>
#include <WebServer.h>

Adafruit_BMP280 bmp; //Global variables needed for this lab
float temperature;
const char* ssid = "iot1lab7";
const char* password = "Dawson2026";
WebServer server(80);
const int ledPin = 23;

//Pasted pogram from HTML.txt
const char webpage[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>

<head>

<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<meta http-equiv="refresh" content="5">

<title>ESP32 Environmental Monitor</title>

<style>

body{
    font-family:Arial,sans-serif;
    background:linear-gradient(
        135deg,
        #1E3C72,
        #2A5298
    );
    color:white;
    text-align:center;
    margin:0;
    padding:20px;
}

.container{
    max-width:800px;
    margin:auto;
}

.card{
    background:rgba(
        255,
        255,
        255,
        0.15
    );

    padding:25px;
    border-radius:15px;

    box-shadow:
    0px 4px 12px rgba(
        0,
        0,
        0,
        0.3
    );
}

.value{
    font-size:42px;
    font-weight:bold;
    color:#FFD54F;
}

button{

    width:180px;
    height:60px;

    margin:10px;

    font-size:20px;

    border:none;

    border-radius:10px;

    cursor:pointer;
}

.on{
    background:#4CAF50;
    color:white;
}

.off{
    background:#F44336;
    color:white;
}

.info{
    margin-top:20px;
}

</style>

</head>

<body>

<div class="container">

<h1>🌎 ESP32 Environmental Monitor</h1>

<div class="card">

<h2>Temperature</h2>

<p class="value">
TEMP_PLACEHOLDER
</p>

</div>

<br>

<button class="on"
onclick="location.href='/on'">
LED ON
</button>

<button class="off"
onclick="location.href='/off'">
LED OFF
</button>

<div class="info">

<p>
SSID: ESP32_Lab7
</p>

<p>
IP Address: 192.168.4.1
</p>

</div>

</div>

</body>

</html>

)rawliteral";


void handleRoot() //Function to display the temp readings in the webpage
{
 temperature = bmp.readTemperature();
 
 String page = webpage;
 page.replace("TEMP_PLACEHOLDER",String(temperature,1) + "%C");
 server.send( 200, "text/html",page);

}

void handleLEDOn() //Function to turn the LED on with the webpage
{ 
  Serial.println("LED ON Route Accessed");
	digitalWrite(ledPin,HIGH);
	server.sendHeader("Location", "/" );
	server.send(303);
}

void handleLEDOff() //Function to turn off the LED off with the webpage
{
  Serial.println("LED OFF Route Accessed");
	digitalWrite( ledPin,LOW);
	server.sendHeader("Location", "/" );
	server.send(303);
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Program Started");
  Wire.begin(); //Initialize I2C communication with the BMO280 sensor

  if(bmp.begin(0x76)) //If statement to determine the if the BMP280 is found
  {
    Serial.println("BMP280 Found");
  }
  else
  {
    Serial.println("BMP280 Not Found");
  }
 
  pinMode(ledPin,OUTPUT); //Setting the led as an output

  //WiFi code
  WiFi.softAP(ssid,password);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  
  //Routes code
  server.on( "/", handleRoot );
  server.on("/on", handleLEDOn);
  server.on("/off", handleLEDOff);

  //Start the web server
  server.begin();
  Serial.println("Web Server Started");

  
 


}

void loop() {
  // put your main code here, to run repeatedly:
  
  server.handleClient(); //Handle client reuests
}

