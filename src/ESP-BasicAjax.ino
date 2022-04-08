#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Function.h"

// **************************  Configuration ***********************
// WiFiMode_t MODE = WIFI_STA;
WiFiMode_t MODE = WIFI_AP;

// **********************************************************************

// Set SSID name if in Station mode
#ifndef STASSID
#define STASSID "ROAM1"
#define STAPSK  ""
#endif

#define DIV1_STYLE Style='background-color:black;color:yellow;width:20vw'

// Function prototypes
void handleRoot();
void handleNotFound();
char* buildPage(float f);
void sendAsyncData(void);
void update(void);

// Pin definitions
const int led = LED_BUILTIN;

// Constants
const char* DEVICE_NAME = "TMon-1";
const char* ssid = STASSID;
const char* password = STAPSK;
const int PAGE_BUFFER_SIZE = 2000;
char pageBuffer[PAGE_BUFFER_SIZE];
unsigned long _timestamp = 0;

ESP8266WebServer server(80);

char* buildPage(float temp)
{
    const char *page =
        "<head>\
            <style>\
                body{background-color:black; color:yellow;font-family:sans-serif;font-size:12}\
                .status{margin-left:30vw;padding-left:1vw;background-color:darkslateblue; color:white;width:30vw;font-size:24}\
                .control{margin-left:30vw;padding-left:1vw;vertical-align:center;background-color:darkslateblue; color:white;width:30vw;height:10vh;font-size:24}\
                .buttonStyle{height:75px;width:100px;font-size:24}\
            </style>\
            <script>\
                setInterval(function()\
                {\
                    getData();\
                }, 5000); \
                function getData() {\
                  var xhttp = new XMLHttpRequest();\
                  xhttp.onreadystatechange = function() {\
                    if (this.readyState == 4 && this.status == 200) {\
                      document.getElementById('volts').innerHTML =\
                      this.responseText;\
                    }\
                  };\
                  xhttp.open('GET', 'read', true);\
                  xhttp.send();\
                }\
            </script>\
         </head>\
         <body>\
             <div id='volts' class='status'>%.2f</div>\
             <div class='control'>&nbsp</div>\
             <div class='control'>\
                <form action='/update' method='GET'>\
                    <span> \
                    <input type = 'submit' id = 'button1' name = 'button1' value= 'F1' class='buttonStyle'>\
                    <input type = 'submit' id = 'button2' name='button2' value= 'F2' class='buttonStyle'>\ 
                    <input type = 'submit' id = 'button3' name='button3' value= 'F3' class='buttonStyle'>\ 
                    <input type = 'submit' id = 'button4' name='button4' value= 'F4' class='buttonStyle'>\ 
                    </span>\
                </form>\
            </div>\
         </body>";

    sprintf(pageBuffer, page, temp);
    return pageBuffer;
}

// Handle call to base URL
void handleRoot()
{
    int val = analogRead(A0);
    Serial.println(val);

    server.send(200, "text/html", buildPage(val));
    digitalWrite(led, 0);
    delay(50);
    digitalWrite(led, 1);
}

// URL not found
void handleNotFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void setup(void)
{
    pinMode(led, OUTPUT);
    digitalWrite(led, 1);

    Serial.begin(115200);
    WiFi.mode(MODE);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin(DEVICE_NAME))
    {
        MDNS.addService("http", "tcp", 80);
        // Start the mDNS responder for esp8266.local
        Serial.println("mDNS responder started");
    }
    else
    {
        Serial.println("Error setting up MDNS responder!");
    }

    // Setup handlers
    server.on("/", handleRoot);

    server.on("/read", sendAsyncData);

    server.onNotFound(handleNotFound);

    server.on("/update", update);   

    server.begin();
    Serial.println("HTTP server started");
}

// Main loop
void loop(void)
{
    static unsigned long lastTime = 0;
    _timestamp = millis() ;
    if (millis() - lastTime > 500)
    {
        lastTime = millis();
    }

    server.handleClient();
    MDNS.update();
}


// Send  data to client via AJAX call
void sendAsyncData(void)
{
    char tempData[20];
    int v = analogRead(A0);
    sprintf(tempData,"%.2f", (float)v);
    server.send(200, "text/plain", tempData);

    Serial.print("AJAX call to send temperature data: "); Serial.print(v);Serial.print(" ");Serial.println(tempData);
}

// Get data from client
void update(void)
{
    static String values[] = {String("F1"), String("F2"), String("F3"), String("F4")};
    static Function *functionList[] = {new F1(), new F2(), new F3(), new F4()};

    String message = "Number of args received: ";
    message += server.args();
    Serial.println(server.arg(0));

    for (int i = 0; i < sizeof(values)/sizeof(values[0]); i++)
    {
        if (server.arg(0).equals(values[i]) )
        {
            functionList[i]->Execute(nullptr);
            break;
        }
    }

    // Redirect back home
    server.sendHeader("Location", String("/"), true);
    server.send ( 302, "text/plain", "");
    Serial.println(message);
}
