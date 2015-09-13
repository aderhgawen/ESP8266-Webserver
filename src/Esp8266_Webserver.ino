/* 
 * ESP8266 Webserver
 * Starts the ESP8266 as an access point and provides a web interface to configure WiFi credentials and control GPIO pins
 * Go to http://192.168.4.1 in a web browser connected to this access point to see it
 * 
 * Created by: Ashish Derhgawen
 * E-mail: ashish.derhgawen@gmail.com
 * Blog: http://ashishrd.blogspot.com
 * Last modified: 13 September, 2015
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

// Access point credentials
const char *ap_ssid = "ESPap";
const char *ap_password = "thereisnospoon";

// ESP8266 GPIO pins
#define GPIO2 2

ESP8266WebServer server(80);

void setup() 
{
  pinMode(GPIO2, OUTPUT);
  
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  
  /* You can add the password parameter if you want the AP to be password protected */
  WiFi.softAP(ap_ssid); // Wifi.softAP(ssid, password)

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  /* Set page handler functions */
  server.on("/",   rootPageHandler);
  server.on("/wlan_config", wlanPageHandler);
  server.on("/gpio", gpioPageHandler);
  server.onNotFound(handleNotFound);
  
  server.begin();
  
  Serial.println("HTTP server started");
}

void loop() 
{
  server.handleClient();
}

/* Root page for the webserver */
void rootPageHandler() 
{
  String response_message = "<html><head><title>ESP8266 Webserver</title></head>";
  response_message += "<body style=\"background-color:PaleGoldenRod\"><h1><center>ESP8266 Webserver</center></h1>";
  
  if (WiFi.status() == WL_CONNECTED)
  {
    response_message += "<center>WLAN Status: Connected</center><br>";
  }
  else
  {
    response_message += "<center>WLAN Status: Disconnected</center><br>";
  }
  
  response_message += "<ul><li><a href=\"/wlan_config\">Configure WLAN settings</a></li>";
  response_message += "<li><a href=\"/gpio\">Control GPIO pins</h4></li></ul>";
  response_message += "</body></html>";
  
  server.send(200, "text/html", response_message);
}

/* WLAN page allows users to set the WiFi credentials */
void wlanPageHandler()
{
  // Check if there are any GET parameters
  if (server.hasArg("ssid"))
  {    
    if (server.hasArg("password"))
    {
      WiFi.begin(server.arg("ssid").c_str(), server.arg("password").c_str());
    }
    else
    {
      WiFi.begin(server.arg("ssid").c_str());
    }
    
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
      
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(1000);
  }
  
  String response_message = "";
  response_message += "<html>";
  response_message += "<head><title>ESP8266 Webserver</title></head>";
  response_message += "<body style=\"background-color:PaleGoldenRod\"><h1><center>WLAN Settings</center></h1>";
  
  if (WiFi.status() == WL_CONNECTED)
  {
    response_message += "Status: Connected<br>";
  }
  else
  {
    response_message += "Status: Disconnected<br>";
  }
  
  response_message += "<p>To connect to a WiFi network, please select a network...</p>";

  // Get number of visible access points
  int ap_count = WiFi.scanNetworks();
  
  if (ap_count == 0)
  {
    response_message += "No access points found.<br>";
  }
  else
  {
    response_message += "<form method=\"get\">";

    // Show access points
    for (uint8_t ap_idx = 0; ap_idx < ap_count; ap_idx++)
    {
      response_message += "<input type=\"radio\" name=\"ssid\" value=\"" + String(WiFi.SSID(ap_idx)) + "\">";
      response_message += String(WiFi.SSID(ap_idx)) + " (RSSI: " + WiFi.RSSI(ap_idx) +")";
      (WiFi.encryptionType(ap_idx) == ENC_TYPE_NONE) ? response_message += " " : response_message += "*";
      response_message += "<br><br>";
    }
    
    response_message += "WiFi password (if required):<br>";
    response_message += "<input type=\"text\" name=\"password\"><br>";
    response_message += "<input type=\"submit\" value=\"Connect\">";
    response_message += "</form>";
  }

  response_message += "</body></html>";
  
  server.send(200, "text/html", response_message);
}

/* GPIO page allows you to control the GPIO pins */
void gpioPageHandler()
{
  // Check if there are any GET parameters
  if (server.hasArg("gpio2"))
  { 
    if (server.arg("gpio2") == "1")
    {
      digitalWrite(GPIO2, HIGH);
    }
    else
    {
      digitalWrite(GPIO2, LOW);
    }
  }

  String response_message = "<html><head><title>ESP8266 Webserver</title></head>";
  response_message += "<body style=\"background-color:PaleGoldenRod\"><h1><center>Control GPIO pins</center></h1>";
  response_message += "<form method=\"get\">";

  response_message += "GPIO2:<br>";

  if (digitalRead(GPIO2) == LOW)
  {
    response_message += "<input type=\"radio\" name=\"gpio2\" value=\"1\" onclick=\"submit();\">On<br>";
    response_message += "<input type=\"radio\" name=\"gpio2\" value=\"0\" onclick=\"submit();\" checked>Off<br>";
  }
  else
  {
    response_message += "<input type=\"radio\" name=\"gpio2\" value=\"1\" onclick=\"submit();\" checked>On<br>";
    response_message += "<input type=\"radio\" name=\"gpio2\" value=\"0\" onclick=\"submit();\">Off<br>";
  }

  response_message += "</form></body></html>";

  
  server.send(200, "text/html", response_message);
}

/* Called if requested page is not found */
void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  server.send(404, "text/plain", message);
}
