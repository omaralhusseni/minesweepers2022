// --------------------- RC ------------------

#define p_ch_1      8
#define p_ch_2      9
#define p_ch_3      10
#define p_ch_4      11
#define p_ch_6      12

#define ch_1_max    1890
#define ch_1_tdzone 1550
#define ch_1_nor    1481
#define ch_1_bdzone 1400
#define ch_1_min    1080

#define ch_2_max    1885
#define ch_2_tdzone 1550
#define ch_2_nor    1481
#define ch_2_bdzone 1400
#define ch_2_min    1074

#define ch_3_max    1900
#define ch_3_tdzone 1600
#define ch_3_nor    1481
#define ch_3_bdzone 1350
#define ch_3_min    1092

#define ch_4_max    1900
#define ch_4_tdzone 1550
#define ch_4_nor    1481
#define ch_4_bdzone 1400 
#define ch_4_min    1088

#define ch_6_max    1900
#define ch_6_tdzone 1550
#define ch_6_nor    1481
#define ch_6_bdzone 1400
#define ch_6_min    1080

int ch_1=0;
int ch_2=0;
int ch_3=0;
int ch_4=0;
int ch_6=0;


// -------------------- Driver -------------
// h-bridge
#define F_R     2  // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
#define R_R     3 // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)
#define F_L     4  // Arduino PWM output pin 3; connect to IBT-2 pin 1 (RPWM)
#define R_L     5  // Arduino PWM output pin 4; connect to IBT-2 pin 2 (LPWM)
#define A_up    6 //pwm
#define A_dn    7 //pwm
#define grip    A8
#define buzz    A0
#define sens_1_pin    A1
#define sens_2_pin    A2


int F_R_PWM;
int R_R_PWM;
int F_L_PWM;
int R_L_PWM;
int A_up_PWM;
int A_dn_PWM;

int sens_1=0;
int sens_2=0;

int top_speed;
int arm_speed;

#define f_topspeed 255
#define f_minspeed 0
#define b_topspeed 255
#define b_minspeed 0

#define arm_topspeed 90
#define arm_minspeed 30


#define sens_limit 3
int sens_cont=0;


int diff;
int maxdiff=180;

int rspeed;
int lspeed;
int l_dir;
int r_dir;
int tin;


void setup() {
  
  Serial.begin(9600);
  
  pinMode(p_ch_1,INPUT);
  pinMode(p_ch_2,INPUT);
  pinMode(p_ch_3,INPUT);
  pinMode(p_ch_4,INPUT);
  pinMode(p_ch_6,INPUT);  


  pinMode(F_R, OUTPUT);
  pinMode(R_R, OUTPUT);
  pinMode(F_L, OUTPUT);
  pinMode(R_L, OUTPUT);
  pinMode(A_up, OUTPUT);
  pinMode(A_dn, OUTPUT);

  pinMode(grip, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(sens_1_pin, INPUT);
  pinMode(sens_2_pin, INPUT);
  
  
}

void loop() {


  read_rc();
//  debug();
  arm();
  motor ();    
  buzzer();
  gripper();
}
  
  
void read_rc()
{
  ch_1 = pulseIn(p_ch_1, HIGH);
  ch_2 = pulseIn(p_ch_2, HIGH);
  ch_3 = pulseIn(p_ch_3, HIGH);
  ch_4 = pulseIn(p_ch_4, HIGH);
  ch_6 = pulseIn(p_ch_6, HIGH);
  }

void debug()
{

  Serial.print("CH 1=");
  Serial.print(ch_1);
  Serial.print("  CH 2=");
  Serial.print(ch_2);
  Serial.print("  CH 3=");
  Serial.print(ch_3);
  Serial.print("  CH 4=");
  Serial.print(ch_4);
  Serial.print("  CH 6=");
  Serial.print(ch_6);

  Serial.print(" Top Speed=");
  Serial.print(top_speed);
  Serial.print(" dir=");
  Serial.print(l_dir);
  Serial.print(r_dir);

  Serial.print(" d=");
  Serial.print(diff);
  Serial.print(" l_s=");
  Serial.print(lspeed);
  Serial.print(" r_s=");
  Serial.print(rspeed);    

  Serial.print(" arm=");
  Serial.print(arm_speed);

  Serial.println("");  
  }

void motor ()
{

    // thr 
    if(ch_3>ch_3_tdzone){
      //Forward
      l_dir=1;
      r_dir=1;
      tin=0;
      top_speed=map(ch_3,ch_3_tdzone,ch_3_max,f_minspeed,f_topspeed);
      if(top_speed>f_topspeed)
      top_speed=255;
      if(top_speed<f_minspeed)
      top_speed=0;
  
    }
  
    
    else if(ch_3<ch_3_bdzone){
      //Backward
      l_dir=0;
      r_dir=0;
      tin=0;
      top_speed=map(ch_3,ch_3_bdzone,ch_3_min,b_minspeed,b_topspeed);
      if(top_speed>b_topspeed)
      top_speed=255;
      if(top_speed<b_minspeed)
      top_speed=0;
      
      }
    else if(ch_3>=ch_3_bdzone && ch_3<=ch_3_tdzone){
      //Stop
      l_dir=0;
      r_dir=0;
  
      top_speed=0;
    }
  // Steer Fn
  
    if(ch_4>ch_4_tdzone){
      //Right
  
      diff=map(ch_4,ch_4_tdzone,ch_4_max,0,maxdiff);
      if(diff>maxdiff)
      diff=maxdiff;
      if(diff<0)
      diff=0;
  
      rspeed=top_speed;
      lspeed=top_speed-diff;
      if(lspeed<70)
      lspeed=70;
  
    }
  
    
    if(ch_4<ch_4_bdzone){
      //Left
      diff=map(ch_4,ch_4_bdzone,ch_4_min,0,maxdiff);
      if(diff>maxdiff)
      diff=maxdiff;
      if(diff<0)
      diff=0;
      
      rspeed=top_speed-diff;
      lspeed=top_speed;    
      if(rspeed<70)
      rspeed=70;
    }
  
  //turn
   if(ch_4>ch_4_tdzone && top_speed==0){
      //Right
      tin=1;
      diff=map(ch_4,ch_4_tdzone,ch_4_max,0,maxdiff);
      if(diff>maxdiff)
      diff=255;
      if(diff<0)
      diff=0;
  
      rspeed=diff;
      lspeed=diff;
  
      F_R_PWM=rspeed;
      R_R_PWM=0;
      F_L_PWM=0;
      R_L_PWM=lspeed;
    }
  
    
    if(ch_4<ch_4_bdzone && top_speed==0){
      //Left
        tin=1;
      diff=map(ch_4,ch_4_bdzone,ch_4_min,0,maxdiff);
      if(diff>maxdiff)
      diff=255;
      if(diff<0)
      diff=0;
  
      rspeed=diff;
      lspeed=diff;
  
      F_R_PWM=0;
      R_R_PWM=rspeed;
      F_L_PWM=lspeed;
      R_L_PWM=0;
  
    
    }

  //ch_4 dead zone   
    if(ch_4>=ch_4_bdzone && ch_4<=ch_4_tdzone){
      //Stop
  
      diff=0;
      rspeed=top_speed;
      lspeed=top_speed;  
    }
  
  //----------------------action 
    if(l_dir==1 && r_dir==1 && tin==0){
      F_R_PWM=rspeed;
      R_R_PWM=0;
      F_L_PWM=lspeed;
      R_L_PWM=0;
      }
    if(l_dir==0 && r_dir==0 && tin==0){
      F_R_PWM=0;
      R_R_PWM=rspeed;
      F_L_PWM=0;
      R_L_PWM=lspeed;  
      }
    tin=0;

    //right motor
        analogWrite(F_R,F_R_PWM);
        analogWrite(R_R,R_R_PWM);
    //left motor
        analogWrite(F_L,F_L_PWM);
        analogWrite(R_L,R_L_PWM);
  
}


void arm()
{
    // Arm 
    if(ch_2>ch_2_tdzone){
      
      //Up
      arm_speed=map(ch_2,ch_2_tdzone,ch_2_max,arm_minspeed,arm_topspeed);
      if(arm_speed>arm_topspeed)
      arm_speed=255;
      else if(arm_speed<arm_minspeed)
      arm_speed=arm_minspeed;

      analogWrite(A_up,arm_speed);
      analogWrite(A_dn,NULL);

    }
  
    
    if(ch_2<ch_2_bdzone){
      
      //Down
      arm_speed=map(ch_2,ch_2_bdzone,ch_2_min,arm_minspeed,arm_topspeed);
      if(arm_speed>arm_topspeed)
      arm_speed=arm_topspeed;
      else if(arm_speed<arm_minspeed)
      arm_speed=arm_minspeed;

      analogWrite(A_up,NULL);
      analogWrite(A_dn,arm_speed);
      
      }
    if(ch_2>=ch_2_bdzone && ch_2<=ch_2_tdzone){
      
      //Stop
      arm_speed=0;
      analogWrite(A_up,NULL);
      analogWrite(A_dn,NULL);
    }

}

void gripper()
{
      if(ch_1>ch_1_nor)
      digitalWrite(grip,HIGH);
      else if(ch_1<ch_1_nor)
      digitalWrite(grip,LOW);
}
void buzzer()
{


      sens_1=digitalRead(sens_1_pin);
      sens_2=digitalRead(sens_2_pin);

      if(sens_1==1 ||sens_2==1)
      {
        sens_cont++;
        }
      else
      {
        sens_cont=0;
        }
       
      Serial.print("Sensor 1= ");
      Serial.print(sens_1);      
      Serial.print("  Sensor 2= ");
      Serial.print(sens_2);
      Serial.print("  count=");
      Serial.print(sens_cont);

      if(ch_6<ch_6_nor || sens_cont>sens_limit)
       { digitalWrite(buzz,LOW);
      Serial.print("   Buzzer ON");

       }
      else
      { 
        digitalWrite(buzz,HIGH);
      Serial.print("   Buzzer OFF");

      }
      Serial.println("");

}
