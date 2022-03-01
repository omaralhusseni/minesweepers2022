#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "Omar"  //change this to your WiFi network name
#define STAPSK "123456789000" //change this to your WiFi password
#endif
#define echoPin 2 
#define trigPin 3
#define detector1 4
#define detector2 5

const char *ssid = STASSID;
const char *password = STAPSK;
long duration ;
int distance ;

const char *host = "192.168.244.203";  //change to your ip address
const uint16_t port = 5054;
bool connected = false;

WiFiClient client;

void setup()
{
    Serial.begin(9600);

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("connecting...");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    pinMode(13, INPUT);
    pinMode(trigPin , OUTPUT);
    pinMode(echoPin , OUTPUT);

}

bool check_mine_location(){
    // check distance using ultrasonic sensor
    digtalWrite(trigPin , LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin , HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin , LOW);
    duration = pulseIn(echoPin , HIGH);
    distance = duration / 58.2;
    Serial.print("Distance: ");
    Serial.println(distance);
    if(distance < 10){
        return  true;
    }
    else{
        return false;
    }
}

bool check_mine(){
    // check mine
    if((digitalRead(detector1) == HIGH) || (digitalRead(detector2) == LOW)){
        return true;
    }
    else{
        return false;
    }
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
            return;
        }

        // This will send a string to the server
        Serial.println("Testing server connection");
        if (client.connected())
        {
            client.println("test");
            connected = true;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while (client.available())
    {
    //   put detection code here
        char c = client.read();
        Serial.print(c);
        if (c == '*') {
            if (check_mine()) {
                if (check_mine_location()) {
                    client.println("up");
                } else {
                    client.println("down")
                }
            } else {
                client.println("none")
            }
        }

    }
}
