#include <PS2X_lib.h> //for v1.6

/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
//#define pressures   true
#define pressures false
//#define rumble      true
#define rumble false

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you connect the controller,
//or call config_gamepad(pins) again after connecting the controller.

int error = 0;
byte type = 0;
byte vibrate = 0;

int motor1_forward = 5;
int motor1_backward = 6;
int motor2_forward = 9;
int motor2_backward = 10;

void setup()
{

  Serial.begin(57600);

  delay(300); //added delay to give wireless ps2 module some time to startup, before configuring it

  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************

  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(11, 13, 12, 4, true, true); //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  if (error == 0)
  {
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
  }

  else if (error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  //Serial.print(ps2x.Analog(1), HEX);

  type = ps2x.readType();
  switch (type)
  {
  case 0:
    Serial.println("Unknown Controller type");
    break;
  case 1:
    Serial.println("DualShock Controller Found");
    break;
  case 2:
    Serial.println("GuitarHero Controller Found");
    break;
  }

}

void mv_forward(int speed)
{
  analogWrite(motor1_backward, 0);
  analogWrite(motor1_forward, speed);
  analogWrite(motor2_backward, 0);
  analogWrite(motor2_forward, speed);
}
void mv_backward(int speed)
{
  analogWrite(motor1_forward, 0);
  analogWrite(motor1_backward, speed);
  analogWrite(motor2_forward, 0);
  analogWrite(motor2_backward, speed);
}
void mv_stop()
{
  analogWrite(motor1_backward, 0);
  analogWrite(motor1_forward, 0);
  analogWrite(motor2_backward, 0);
  analogWrite(motor2_forward, 0);
}

void spin_right() {
  analogWrite(motor1_forward, 255);
  analogWrite(motor1_backward, 0);
  analogWrite(motor2_backward, 255);
  analogWrite(motor2_forward, 0);
}

void spin_left()
{
  analogWrite(motor1_forward, 0);
  analogWrite(motor1_backward, 255);
  analogWrite(motor2_backward, 0);
  analogWrite(motor2_forward, 255);
}

void loop()
{
  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */
  if (error == 1)
  { //skip loop if no controller found
    return;
  }
  else
  {
    ps2x.read_gamepad(false, 0); //read controller and set large motor to spin at 'vibrate' speed
    
    if (ps2x.Analog(PSS_RY) == 255)
    {
      mv_stop();
    }
    else if (ps2x.Button(PSB_PAD_UP))
    {
      mv_forward(255 - ps2x.Analog(PSS_RY));
    }
    else if (ps2x.Button(PSB_PAD_DOWN))
    {
      mv_backward(255 - ps2x.Analog(PSS_RY));
    }
    else if (ps2x.Button(PSB_PAD_RIGHT))
    {
      spin_right();
    }
    else if (ps2x.Button(PSB_PAD_LEFT))
    {
      spin_left();
    }
    else
    {
      mv_stop();
    }
    delay(50);
  }
}

