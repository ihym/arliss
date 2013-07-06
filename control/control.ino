#include <SoftwareSerial.h>
#include <Navigation.h>
#include <TinyGPS.h>

#define TARGET_LAT 38.24
#define TARGET_LON 23
#define MIN_DIST 10
#define TIME_CTRL 1000

TinyGPS gps;
SoftwareSerial nss(3, 4);
void XYZ_accelerations(char); //Returns the angle in degrees from north
bool feedgps();
float controller(float,float);


int pinI1A=6; //define I1 port DC motor A
int pinI2A=7; //define I2 port DC motor A
int pinI1B=8; //define I1 port DC motor B
int pinI2B=9; //define I2 port DC motor B
int speedpinA=10; //define EA(PWM speed regulation)port
int speedpinB=11; //define EB(PWM speed regulation)port

void setup(){
    pinMode(pinI1A,OUTPUT); //define this port as output
    pinMode(pinI2A,OUTPUT);
    pinMode(speedpinA,OUTPUT);

    pinMode(pinI1B,OUTPUT); //define this port as output
    pinMode(pinI2B,OUTPUT);
    pinMode(speedpinB,OUTPUT);

    Serial.begin(115200);
    nss.begin(9600);
}

void loop(){
	unsigned long start = millis();
	long age;
    float flat,flon,faltitude,fcourse,fspeed,fdistance,ftarget_course;
    unsigned long start = millis();
    // ******************* loop timing control *******************
    while (millis() - start < TIME_CTRL){
    	// ********************* Sensor Acquisition *******************
		if (feedgps()){
			gps.f_get_position(&flat,&flon,&age);
			faltitude = gps.f_altitude();
			fcourse = gps.f_course();
			ftarget_course = TinyGPS::course_to(flat, flon, TARGET_LAT, TARGET_LON);
			fdistance = TinyGPS::distance_between(flat, flon, TARGET_LAT , TARGET_LON);
			// ******************* Angle Control & Motor Drive & Debugging Info *******************
			if (fdistance<MIN_DIST){
				stop_rotating(pinI1A,pinI2A,pinI1B,pinI2B);
				Serial.print("Stop Rotating");
			}
			else{
				if(controller(fcourse,ftarget_course)>20 && controller(fcourse,ftarget_course)<=180){
					turn_right(speedpinA,speedpinB,pinI1A,pinI2A,pinI1B,pinI2B);
					Serial.print("Turn Left");
				}
				else if(controller(fcourse,ftarget_course)>180 && controller(fcourse,ftarget_course)<340){
					turn_left(speedpinA,speedpinB,pinI1A,pinI2A,pinI1B,pinI2B);
					Serial.print("Turn Right");
				}
				else{
					move_fowards(speedpinA,speedpinB,pinI1A,pinI2A,pinI1B,pinI2B);
					Serial.print("Move Forwards");
				}
			}
		}
	}
}

bool feedgps(){
	while (nss.available()){
		char c = nss.read(); 
		if (gps.encode(c))
			return true;
	}
	return false;
}

float controller(float course,float target_course){
	if (course-target_course<0)
		return 360+course-target_course;
	else 
		return course-target_course;
}

/*
void XYZ_accelerations(char c){
	static int i = 0;
	static int ia = 0;
	static int ib = 0;
	static int ic = 0;
	static int X = 0;
	static int Y = 0;
	static int Z = 0;
	static int comma_count = 0;
	char *sentence="$PLSR,245,1,";
  
	if(i>11){
		if(c == ',')
			comma_count++;
		if(comma_count == 2){
			if(ia == 0)
				ia = i + 1;
			if(i == ia)
				X = ((int)c-((int)'0'));
			if(i == ia + 1){
				int j = X;
				X = 10*j+((int)c-((int)'0'));
			}
			if(i == ia + 2){
				int k = X/10;
				int l = X%10;
				X = 100*k+10*l+((int)c-((int)'0'));
			}
		}	
		if(comma_count == 3){
			if(ib == 0)
				ib = i + 1;	
			if(i == ib)
				Y = ((int)c-((int)'0'));
			if(i == ib + 1){
				int j = Y;
				Y = 10*j+((int)c-((int)'0'));
			}
			if(i == ib + 2){
				int k = Y/10;
				int l = Y%10;
				Y = 100*k+10*l+((int)c-((int)'0'));
			}
		}
		if(comma_count == 4){
			if(ic == 0)
				ic = i + 1;
			if(i == ic)
				Z = ((int)c-((int)'0'));
			if(i == ic + 1){
				int j = Z;
				Z = 10*j+((int)c-((int)'0'));
			}
			if(i == ic + 2){
				int k = Z/10;
				int l = Z%10;
				Z = 100*k+10*l+((int)c-((int)'0'));
			}
		}
		i++;
		if(c == ',' && comma_count > 4){
			Serial.println("--------------------");
			Serial.println(magnet_course);
			Serial.print("X: ");
			Serial.println(X*(2.0/512.0));
			Serial.print("Y: ");
			Serial.println(Y*(2.0/512.0));
			Serial.print("Z: ");
			Serial.println(Z*(2.0/512.0));
			i=0;
			comma_count = 0;
			return;
		}
	}
	else{
		if(sentence[i]==c)
			i++;
		else 
			i=0;
	}
}
*/