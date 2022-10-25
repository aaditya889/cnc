#include <ArduinoJson.h>
#include "webpage.h"

const char* input_parameter1 = "output";
const char* input_parameter2 = "state";

String get_debug_switch_value(int debug_switch_idx){
  if (DEBUG_SWITCHES[debug_switch_idx]) return "checked";
  else return "";
}


// Replaces placeholder with button section in your web page
String processor(const String& var){
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    buttons += "<h4>Enable Verbose Logging</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"0\" " + get_debug_switch_value(0) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Is Inches?</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"1\" " + get_debug_switch_value(1) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Debug Switch 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + get_debug_switch_value(2) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Debug Switch 3</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"3\" " + get_debug_switch_value(3) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Debug Switch 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + get_debug_switch_value(4) + "><span class=\"slider\"></span></label>";

    buttons += "<h4>Buttons</h4><button class=\"btn btn-danger\" type=\"button\" onclick=\"triggerButton(this)\" id=\"10\" >Healthcheck</button>";
    return buttons;
  }
  return String();
}


void push_gcode_line(AsyncWebServerRequest *request)
{
  AsyncWebServerResponse *response;
  char message[100];
  String gcode_line;

  // //List all parameters (Compatibility)
  // int args = request->args();
  // for(int i=0;i<args;i++){
  //   Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
  // }

  Serial.printf("Free heap space: %d\n", ESP.getFreeHeap());
  gcode_line = request->getParam("gcode_line", true)->value();

  // sprintf(message, "Got GCODE line: %s", gcode_line);
  if (add_gcode_line_to_queue(gcode_line.c_str())) sprintf(message, "GCode line added");
  else sprintf(message, "Unable to add the GCode line");

  // sprintf(message, "yoyo!");

  response = request->beginResponse(200, "text/plain",  message);
  response->addHeader("Access-Control-Allow-Origin", "*");
  // response->addHeader("Connection", "close");
  request->send(response);
  // request->client()->stop();
  // while (request->client()->connected()) 
  // {
  //   Serial.printf("Waiting for the connection to close...\n");
  //   delay(1);
  // }
  // request->client()->close();
  // delete request;
}


void cnc_healthcheck(AsyncWebServerRequest *request)
{
  AsyncWebServerResponse *response;
  char message[300];
  Serial.printf("Healthcheck Passed!\n");
  sprintf(message, "Healthcheck Passed!\nIP used by the CNC Machine:: %s\n", WiFi.localIP().toString().c_str());
  
  response = request->beginResponse(200, "text/plain",  message);
  response->addHeader("Access-Control-Allow-Origin", "*");
  request->send(response);
}


void update_debug_switch(AsyncWebServerRequest *request)
{
  AsyncWebServerResponse *response;
  String inputMessage1;
  String inputMessage2;
  short switch_id = 0;
  boolean switch_state = false;
  char message[300];

  if (request->hasParam("switch_id") && request->hasParam("state")) 
  {
    switch_id = request->getParam("switch_id")->value().toInt();
    switch_state = (boolean) request->getParam("state")->value().toInt();
    // digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
  }
  Serial.printf("Updating debug switch:: id: %d, state: %d\n", switch_id, switch_state);
  DEBUG_SWITCHES[switch_id] = (boolean) switch_state;
  sprintf(message, "updated!");

  response = request->beginResponse(200, "text/plain",  message);
  response->addHeader("Access-Control-Allow-Origin", "*");
  request->send(response);
}


// void api_not_found()
// {
//   SERVER.sendHeader("Access-Control-Allow-Origin", "*");
//   SERVER.send(404, "text/plain", "Not found");
// }


void setup_server()
{
  // Route for root / web page
  SERVER.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // API routes 
  SERVER.on("/push_gcode_line", HTTP_POST, [](AsyncWebServerRequest *request){ push_gcode_line(request); });
  SERVER.on("/cnc_healthcheck", HTTP_GET, [](AsyncWebServerRequest *request){ cnc_healthcheck(request); });
  SERVER.on("/update_debug_switch", HTTP_GET, [](AsyncWebServerRequest *request){ update_debug_switch(request); });
  // SERVER.onNotFound(api_not_found);
}


void send_udp(char *message)
{
  // udp send takes around 700 - 750 microseconds
  udp_client.beginPacket(REMOTE_IP, REMOTE_PORT);
  udp_client.write(message, strlen(message));
  udp_client.endPacket();
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

