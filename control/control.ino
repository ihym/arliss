#include <SoftwareSerial.h>
#include <Move.h>
#include <TinyGPS.h>
// TO DO X Y Z ACCELARATIONS !!!!!!!!!!!!!!!!!!!!!!!!!
TinyGPS gps;
SoftwareSerial nss(3, 4);
static void plsr(char c);//Returns the angle in degrees from north
static void gpsdump();//Returns GPS data
static bool feedgps();
float ftarget_angle;
float ftarget_course;
int pinI1A=6;//define I1 port DC motor A
int pinI2A=7;//define I2 port DC motor A
int pinI1B=8;//define I1 port DC motor B
int pinI2B=9;//define I2 port DC motor B
int speedpinA=10;//define EA(PWM speed regulation)port
int speedpinB=11;//define EB(PWM speed regulation)port

void setup(){
    pinMode(pinI1A,OUTPUT);//define this port as output
    pinMode(pinI2A,OUTPUT);
    pinMode(speedpinA,OUTPUT);

    pinMode(pinI1B,OUTPUT);//define this port as output
    pinMode(pinI2B,OUTPUT);
    pinMode(speedpinB,OUTPUT);

    Serial.begin(115200);
    nss.begin(9600);
}

void loop(){
  bool newdata = false;
  unsigned long start = millis();
  
  // Every second we print an update
  while (millis() - start < 1000)
  {
    if (feedgps()){
      gpsdump();
    }
    else;//STOP AND WAIT.
  }
}

static void gpsdump(){
  float flat, flon,faltitude,fcourse,fspeed,fdistance;
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  static const float TARGET_LAT = 38.24, TARGET_LON = 23;
  
  gps.f_get_position(&flat, &flon, &age);
  faltitude = gps.f_altitude();
  fcourse = gps.f_course();
  fspeed = gps.f_speed_kmph()/3.6;
  fdistance = TinyGPS::distance_between(flat, flon, TARGET_LAT , TARGET_LON);
  ftarget_course = TinyGPS::course_to(flat, flon, TARGET_LAT, TARGET_LON);
  
  Serial.print("LAT:  ");
  Serial.println(flat);
  Serial.print(" LON: ");
  Serial.println(flon);
  Serial.println(faltitude);
  Serial.println(fcourse);
  Serial.println(fspeed);
  Serial.println(fdistance);
  Serial.println(ftarget_course);
}

static bool feedgps()
{
  while (nss.available())
  {
    char c = nss.read(); 
    if (gps.encode(c))
      return true;
    else
      plsr(c);
  }
  
  return false;
}

static void plsr(char c){
  static int i = 0;
  static int ia = 0;
  static int ib = 0;
  static int ic = 0;
  static int magnet_course =0;
  static int X = 0;
  static int Y = 0;
  static int Z = 0;
  static int J = 0;
  static int K = 0;
  static int comma_count = 0;
  char *sentence="$PLSR,245,1,";
  
  if(i>11){
    
    if(c == ',')
    {
      comma_count++;
    }
    
    if(comma_count == 0)
    {
      if(i == 12)
      {
        magnet_course = ((int)c-((int)'0'));
      }
      if(i == 13)
      {
        int j = magnet_course;
        magnet_course = 10*j+((int)c-((int)'0'));
      }
      if(i == 14)
      {
        int k = magnet_course/10;
        int l = magnet_course%10;
        magnet_course = 100*k+10*l+((int)c-((int)'0'));
      }
    }
    
    if(comma_count == 2)
    {
      if(ia == 0)
      {
        ia = i + 1;
      }
      
      if(i == ia)
      {
        X = ((int)c-((int)'0'));
      }
      if(i == ia + 1)
      {
        int j = X;
        X = 10*j+((int)c-((int)'0'));
      }
      if(i == ia + 2)
      {
        int k = X/10;
        int l = X%10;
        X = 100*k+10*l+((int)c-((int)'0'));
      }
    }
    
    if(comma_count == 3)
    {
      if(ib == 0)
      {
        ib = i + 1;
      }
      
      if(i == ib)
      {
        Y = ((int)c-((int)'0'));
      }
      if(i == ib + 1)
      {
        int j = Y;
        Y = 10*j+((int)c-((int)'0'));
      }
      if(i == ib + 2)
      {
        int k = Y/10;
        int l = Y%10;
        Y = 100*k+10*l+((int)c-((int)'0'));
      }
    }
    
    if(comma_count == 4)
    {
      if(ic == 0)
      {
        ic = i + 1;
      }
      
      if(i == ic)
      {
        Z = ((int)c-((int)'0'));
      }
      if(i == ic + 1)
      {
        int j = Z;
        Z = 10*j+((int)c-((int)'0'));
      }
      if(i == ic + 2)
      {
        int k = Z/10;
        int l = Z%10;
        Z = 100*k+10*l+((int)c-((int)'0'));
      }
    }
    
    i++;
    
    if(c == ',' && comma_count > 4)
    {
      Serial.println("--------------------");
      Serial.println(magnet_course);
      Serial.print("X: ");
      Serial.println(X*(2.0/512.0));
      Serial.print("Y: ");
      Serial.println(Y*(2.0/512.0));
      Serial.print("Z: ");
      Serial.println(Z*(2.0/512.0));
      ftarget_angle = ftarget_course - magnet_course;
      i=0;
      comma_count = 0;
      return;
    }
  }

  else{
    magnet_course=0;
    if(sentence[i]==c){i++;}
    else i=0;
  }
  
}
