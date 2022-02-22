#include <PS2X_lib.h> //for v1.6
PS2X ps2x;            // create PS2 Controller Class

// hn3raf el pinat
int Motor_1a = 4; // MOTORS RIGHT (A)
int Motor_1b = 5; // MOTORS RIGHT (B)
int Motor_2a = 6; // MOTORS LEFT (A)
int Motor_2b = 7; // MOTORS LEFT (B)
int valve = 2;    // WATER PUMP (A)
int sarena = 32;  // SARENA
int EN_1 = 8;    // Enable (A)
int EN_2 = 9;    // Enable (B)
char val;
int z = 0;
int error = 0;
byte type = 0;
byte vibrate = 0;

void setup()
{
    Serial.begin(57600);
    pinMode(Motor_1a, OUTPUT);
    pinMode(Motor_1b, OUTPUT);
    pinMode(Motor_2a, OUTPUT);
    pinMode(Motor_2b, OUTPUT);
    pinMode(EN_1, OUTPUT);
    pinMode(EN_2, OUTPUT);
    pinMode(valve, OUTPUT);
    pinMode(13, OUTPUT);
    Serial.begin(9600);

    // flash pin 13 5 times
    for (int i = 0; i < 5; i++)
    {
        digitalWrite(13, HIGH);
        delay(200);
        digitalWrite(13, LOW);
        delay(200);
    }
}

void forward()
{
    Serial.println("Forward");
    digitalWrite(Motor_1a, HIGH);
    digitalWrite(Motor_1b, LOW);
    digitalWrite(Motor_2a, HIGH);
    digitalWrite(Motor_2b, LOW);
    digitalWrite(EN_1, HIGH);
    digitalWrite(EN_2, HIGH);
}

void back()
{
    Serial.println("Backward");
    digitalWrite(Motor_1a, LOW);
    digitalWrite(Motor_1b, HIGH);
    digitalWrite(Motor_2a, LOW);
    digitalWrite(Motor_2b, HIGH);
    digitalWrite(EN_1, HIGH);
    digitalWrite(EN_2, HIGH);
}

void left()
{
    Serial.println("Left");
    digitalWrite(Motor_1a, HIGH);
    digitalWrite(Motor_1b, LOW);
    digitalWrite(Motor_2a, LOW);
    digitalWrite(Motor_2b, HIGH);
    digitalWrite(EN_1, HIGH);
    digitalWrite(EN_2, HIGH);
}

void right()
{
    Serial.println("Right");
    digitalWrite(Motor_1a, LOW);
    digitalWrite(Motor_1b, HIGH);
    digitalWrite(Motor_2a, HIGH);
    digitalWrite(Motor_2b, LOW);
    digitalWrite(EN_1, HIGH);
    digitalWrite(EN_2, HIGH);
}
void Stop()
{
    Serial.println("Stop");
    digitalWrite(Motor_1a, LOW);
    digitalWrite(Motor_1b, LOW);
    digitalWrite(Motor_2a, LOW);
    digitalWrite(Motor_2b, LOW);
}

void valve_on()
{
    Serial.println("Valve on");
    digitalWrite(valve, HIGH);
}

void valve_off()
{
    Serial.println("Valve on");
    digitalWrite(valve, LOW);
}

void Sarena_on()
{
    Serial.println("Sarena on");
    digitalWrite(sarena, HIGH);
}

void Sarena_off()
{
    Serial.println("Sarena off");
    digitalWrite(sarena, LOW);
}

void loop()
{
    digitalWrite(EN_1, HIGH);
    digitalWrite(EN_2, HIGH);

    digitalWrite(Motor_1a, LOW);
    digitalWrite(Motor_1a, HIGH);
    digitalWrite(Motor_2b, HIGH);
    digitalWrite(Motor_2b, LOW);
    digitalWrite(13, HIGH);
    delay(5000);
    digitalWrite(13, LOW);
    digitalWrite(Motor_1a, HIGH);
    digitalWrite(Motor_1a, LOW);
    digitalWrite(Motor_2b, LOW);
    digitalWrite(Motor_2b, HIGH);
    delay(5000);
}
