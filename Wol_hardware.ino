#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
int ReadPin = D0;
const char* ssid = ""; //ssid here
const char* password = ""; //pw here
ESP8266WebServer server(80);

const char pageMain[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>aa2 IoT Network</title>
    <style>
        body, html{
            background-color: black;
            color: white;
            display: flex;
            align-items: center;
            justify-content: center;
            flex-direction: column;
            width: 100%;
            height: 100%;
            overflow: hidden;
        }
        #code{
            color: white;
            border: 1px solid white;
            background-color: black;
            outline: none;
        }
        #send{
            color: white;
            border: 1px solid white;
            background-color: black;
        }
    </style>
</head>
<body>
    <h1>aa2 IoT Network</h1>
    <div>
        <div>Active Code is Here</div>

        <form action="/signal_on" method="post">
            <label for = "name"></label>
            <input id = "code" type="password" id="name" name="name">
            <input id = "send" type="submit" value="전송">
        </form>
    </div>
</body>
</html>
)=====";
 
void htmlmain() {
	String html = pageMain;
	server.send(200, "text/html", html);
}


void signal_on() {
  if (server.hasArg("name")) {
    if (server.arg("name") == "somepasswd here"){server.send(200, "text/plain", "signal injected!"); 
      wol();
    	Serial.println("wifi");
    }
    else {server.send(200, "text/plain", "password is incorrect!");}
  } 
  else {server.send(418);}
}
 
void setup() {
  Serial.begin(115200);
  analogWrite(D1, 50); //120때 적당했음 더 낮춰본다 지금은
  pinMode(ReadPin, INPUT);
  
  WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.print("Connecting to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
 
	server.on("/", htmlmain);
	server.on("/signal_on", signal_on);
	server.begin();
	Serial.println("Server started");
}

void loop() {
  server.handleClient();
  int Result = analogRead(ReadPin);
  if (Result >= 1000){
    Serial.println("local");
    wol();
  }
}

void wol() {
  Serial.println("wol!");
  analogWrite(D6, 100);
  pinMode(D5, OUTPUT);
  delay(1000);
  analogWrite(D6, 0);
  pinMode(D5, INPUT);
}