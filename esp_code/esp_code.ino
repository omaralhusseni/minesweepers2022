#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "robot"
#define STAPSK "12345678"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

const char *host = "192.168.244.203";
const uint16_t port = 5054;
bool connected = false;

WiFiClient client;

void setup()
{
    Serial.begin(9600);
    // connect to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
        would try to act as both a client and an access-point and could cause
        network-issues with your other WiFi-devices on your WiFi-network. */
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("connected");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    pinMode(13, INPUT);
}

void loop()
{

    if (!connected)
    {
        Serial.print("connecting to ");
        Serial.print(host);
        Serial.print(':');
        Serial.println(port);

        // Use WiFiClient class to create TCP connections
        while (!client.connect(host, port))
        {
            Serial.println("connection failed");
            delay(1000);
        }

        // This will send a string to the server
        Serial.println("Testing server connection");
        if (client.connected())
        {
            client.println("hello from ROV robot");
            connected = true;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while (client.available())
    {
      Serial.println("Searching");
      delay(100);
      
        if (digitalRead(13))
        {
            client.println("1");
            Serial.println("1");
        }
        else
        {
            client.println("0");
        }
    }
}
