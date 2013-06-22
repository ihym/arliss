#include <SoftwareSerial.h>

#include <TinyGPS.h>
// TO DO X Y Z ACCELARATIONS !!!!!!!!!!!!!!!!!!!!!!!!!
TinyGPS gps;
SoftwareSerial nss(3, 4);
static void plsr(char c);//Epistrefei tin kateuthinsi se moires apo ton borra tou magnitometrou
static void gpsdump();//Epistrefei oles tis metablites se sxesi me to gps pou xreiazomaste
static bool feedgps();
float ftarget_angle;
float ftarget_course;
void setup()
{
  Serial.begin(115200);
  nss.begin(9600);
}

void loop()
{
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

static void gpsdump()
{
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
  static int magnet_course =0;
  char *sentence="$PLSR,245,1,";
  if(i>11 && c!=','){
    if(i==12)
    magnet_course=((int)c-((int)'0'));
    if(i==13){
      int j=magnet_course;
      magnet_course = 10*j+((int)c-((int)'0'));
    }
    if(i==14){
      int k=magnet_course/10;
      int l=magnet_course%10;
      magnet_course = 100*k+10*l+((int)c-((int)'0'));
    }
    i++;
  }
  else if(i>11 && c==','){
      Serial.println("--------------------");
      Serial.println(magnet_course);
      ftarget_angle = ftarget_course - magnet_course;
      i=0;
      return;
  }
  else{
    magnet_course=0;
    if(sentence[i]==c){i++;}
    else i=0;
  }
  
}
