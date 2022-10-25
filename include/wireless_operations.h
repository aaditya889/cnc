#include <ESP8266WiFi.h>


void connect_AP(const char *ssid, const char *password)
{
    // Set your Static IP address
  IPAddress local_IP(192, 168, 1, 69);
  // Set your Gateway IP address
  IPAddress gateway(192, 168, 1, 1);

  IPAddress subnet(255, 255, 255, 0);
  IPAddress primaryDNS(8, 8, 8, 8); // optional
  IPAddress secondaryDNS(1, 1, 1, 1); // optional


  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

//  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
//  delay(1000);
//  WiFi.setSleepMode(WIFI_NONE_SLEEP);
//  WiFi.mode(WIFI_STA);        //Only Station No AP, This line hides the viewing of ESP as wifi hotspot
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  Serial.print("Connecting...");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}
