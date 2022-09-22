#include <ArduinoJson.h>
#include "webpage.h"


const char* input_parameter1 = "output";
const char* input_parameter2 = "state";


String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}


// Replaces placeholder with button section in your web page
String processor(const String& var){
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    buttons += "<h4>Output - GPIO 32</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"32\" " + outputState(32) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 25</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"25\" " + outputState(25) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 27</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"27\" " + outputState(27) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 13</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"13\" " + outputState(13) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}


void testing(AsyncWebServerRequest *request)
{
  AsyncWebServerResponse *response;
  String inputMessage1;
  String inputMessage2;
  // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  if (request->hasParam(input_parameter1) && request->hasParam(input_parameter2)) {
      Serial.println("HAVE PARAMS!");
      inputMessage1 = request->getParam(input_parameter1)->value();
      inputMessage2 = request->getParam(input_parameter2)->value();
      // digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
  }
  else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
  }
  if (request->hasParam("body", true))
  {
      Serial.println("HAVE BODY!");
      inputMessage1 = request->getParam("body", true)->value();
  }

  Serial.print("GPIO: ");
  Serial.print(inputMessage1);
  Serial.print(" - Set to: ");
  Serial.println(inputMessage2);
  response = request->beginResponse(200, "text/plain",  "OK");
  request->send(response);
}


void push_gcode_line(AsyncWebServerRequest *request)
{
  AsyncWebServerResponse *response;
  char message[200];
  char gcode_line[100];
  const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 300;
  DynamicJsonDocument payload(bufferSize);
  deserializeJson(payload, request->getParam("body", true)->value());

  if (payload["gcode_line"] != nullptr) strcpy(gcode_line, payload["gcode_line"]);

  // sprintf(message, "Got GCODE line: %s", gcode_line);
  add_gcode_line_to_queue(gcode_line);

  response = request->beginResponse(200, "text/plain",  message);
  response->addHeader("Access-Control-Allow-Origin", "*");
  request->send(response);
}


void cnc_healthcheck(AsyncWebServerRequest *request)
{
  AsyncWebServerResponse *response;
  char message[300];

  sprintf(message, "Healthcheck Passed!\nIP used by the CNC Machine:: %s\n", WiFi.localIP().toString().c_str());
  
  response = request->beginResponse(200, "text/plain",  message);
  response->addHeader("Access-Control-Allow-Origin", "*");
  request->send(response);
}


// void api_not_found()
// {
//   SERVER.sendHeader("Access-Control-Allow-Origin", "*");
//   SERVER.send(404, "text/plain", "Not found");
// }


void send_udp(char *message)
{
  // udp send takes around 700 - 750 microseconds
  udp_client.beginPacket(REMOTE_IP, REMOTE_PORT);
  udp_client.write(message, strlen(message));
  udp_client.endPacket();
}


void setup_server()
{
  // Route for root / web page
  SERVER.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // API routes 
  SERVER.on("/testing", HTTP_GET, [](AsyncWebServerRequest *request){ testing(request); });
  SERVER.on("/push_gcode_line", HTTP_POST, [](AsyncWebServerRequest *request){ push_gcode_line(request); });
  SERVER.on("/cnc_healthcheck", HTTP_GET, [](AsyncWebServerRequest *request){ cnc_healthcheck(request); });
  // SERVER.onNotFound(api_not_found);
}


void init_server()
{
  char udp_message[100];
  setup_server();
  AsyncElegantOTA.begin(&SERVER);    // Start ElegantOTA
  SERVER.begin();
  Serial.print(WiFi.localIP());
  Serial.println("HTTP server started");

  sprintf(udp_message, "Server initiated, listening on %s:%d...\n", WiFi.localIP().toString().c_str(), SERVER_PORT);
  send_udp(udp_message);
}

