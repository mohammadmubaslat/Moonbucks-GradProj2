  #include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "Moonbucks Machine";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);


uint8_t Button1pin = 13;
bool Button1status = LOW;

uint8_t Button2pin = 3;
bool Button2status = LOW;

uint8_t Button3pin = 5;
bool Button3status = LOW;

uint8_t Button4pin = 6;
bool Button4status = LOW;

char c = '0';

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void handle_OnConnect() {
  Button1status = LOW;
  Button2status = LOW;
  Button3status = LOW;
  Button4status = LOW;
  //Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status, Button4status)); 
}

void setup() {
  Serial2.begin(115200);
  pinMode(Button1pin, OUTPUT);
  pinMode(Button2pin, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  
  server.on("/button1on", handle_button1on);
  server.on("/button1off", handle_button1off);
  server.on("/button2on", handle_button2on);
  server.on("/button2off", handle_button2off);
  server.on("/button3on", handle_button3on);
  server.on("/button3off", handle_button3off);
  server.on("/button4on", handle_button4on);
  server.on("/button4off", handle_button4off);
  
  server.onNotFound(handle_NotFound);
  server.begin();
  //Serial.println("HTTP server started");
}


String SendHTML(uint8_t button1stat, uint8_t button2stat, uint8_t button3stat, uint8_t button4stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #2980b9;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #2980b9;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #2980b9;}\n";
  ptr +=".button-off:active {background-color: #2980b9;}\n";
  ptr +="p {font-size: 30px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Moonbucks Machine</h1>\n";
  ptr +="<h3></h3>\n";
  
  if(button1stat)
  {ptr +="<p>Normal: ON</p><a class=\"button button-off\" href=\"/button1off\">OFF</a>\n";}
  else
  {ptr +="<p>Normal: OFF</p><a class=\"button button-on\" href=\"/button1on\">ON</a>\n";}
 
  if(button2stat)
  {ptr +="<p>No Sugar: ON</p><a class=\"button button-off\" href=\"/button2off\">OFF</a>\n";}
  else
  {ptr +="<p>No Sugar: OFF</p><a class=\"button button-on\" href=\"/button2on\">ON</a>\n";}

  if(button3stat)
  {ptr +="<p>Sugar Plus: ON</p><a class=\"button button-off\" href=\"/button3off\">OFF</a>\n";}
  else
  {ptr +="<p>Sugar Plus: OFF</p><a class=\"button button-on\" href=\"/button3on\">ON</a>\n";}
 
  if(button4stat)
  {ptr +="<p>Dark: ON</p><a class=\"button button-off\" href=\"/button4off\">OFF</a>\n";}
  else
  {ptr +="<p>Dark: OFF</p><a class=\"button button-on\" href=\"/button4on\">ON</a>\n";}
  
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}


void handle_button1on() {
  Button1status = HIGH;
  c = '1';
  server.send(200, "text/html", SendHTML(true, Button2status, Button3status, Button4status)); 
}

void handle_button1off() {
  Button1status = LOW;
  c = '0';
  server.send(200, "text/html", SendHTML(false, Button2status, Button3status, Button4status)); 
}

void handle_button2on() {
  Button2status = HIGH;
  c = '2';
  server.send(200, "text/html", SendHTML(Button1status,true , Button3status, Button4status)); 
}

void handle_button2off() {
  Button2status = LOW;
  c = '0';
  server.send(200, "text/html", SendHTML(Button1status, false, Button3status, Button4status)); 
}
void handle_button3on() {
  Button3status = HIGH;
  c = '3';
  server.send(200, "text/html", SendHTML(Button1status, Button2status, true, Button4status)); 
}

void handle_button3off() {
  Button3status = LOW;
  c = '0';
  server.send(200, "text/html", SendHTML(Button1status, Button2status, false, Button4status)); 
}

void handle_button4on() {
  Button4status = HIGH;
  c = '4';
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status, true)); 
}

void handle_button4off() {
  Button4status = LOW;
  c = '0';
  server.send(200, "text/html", SendHTML(Button1status, Button2status, Button3status, false)); 
}

void loop() {
  server.handleClient();


 if (c == '0'){
    Serial2.write(c);
  Serial2.println();
  delay(1500);
 } else if ( c == '1'){
  
  Serial2.write(c);
  Serial2.println();
  delay(1500);
  Button1status = LOW;
  Button2status = LOW;
  Button3status = LOW;
  Button4status = LOW;
  handle_button1off();
 Serial2.write(c);
  Serial2.println();
  delay(1500);
 } else if ( c == '2'){
   Serial2.write(c);
  Serial2.println();
  delay(1500);
  Button1status = LOW;
  Button2status = LOW;
  Button3status = LOW;
  Button4status = LOW;
  handle_button2off();
  Serial2.write(c);
  Serial2.println();
  delay(1500);
 } else if ( c == '3'){
    Serial2.write(c);
  Serial2.println();
  delay(1500);
  Button1status = LOW;
  Button2status = LOW;
  Button3status = LOW;
  Button4status = LOW;
  handle_button3off();
  Serial2.write(c);
  Serial2.println();
  delay(1500);
 } else if ( c == '4'){
   Serial2.write(c);
  Serial2.println();
  delay(1500);
  Button1status = LOW;
  Button2status = LOW;
  Button3status = LOW;
  Button4status = LOW;
  handle_button4off();
  Serial2.write(c);
  Serial2.println();
  delay(1500);
 }

  
}
