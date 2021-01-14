/*
  HC-SR04 with Temp and Humidity Demonstration
  HC-SR04-Temp-Humid-Demo.ino
  Demonstrates enhancements of HC-SR04 Ultrasonic Range Finder
  With DHT22 Temperature and Humidity Sensor
  Displays results on Serial Monitor
 
  DroneBot Workshop 2017
  http://dronebotworkshop.com
*/
 
// Include DHT Libraries from Adafruit
// Dependant upon Adafruit_Sensors Library
#include <ros.h>
#include <ros/time.h>
#include <sensor_msgs/Range.h>
#include <std_msgs/String.h>
#include "DHT.h";
#include <LiquidCrystal.h>
  
//Include NewPing Library
#include "NewPing.h"

// Define Constants
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
 
#define DHTPIN 7       // DHT-22 Output Pin connection
#define DHTTYPE DHT11   // DHT Type is DHT 22 (AM2302)
#define TRIGGER_PIN  10
#define ECHO_PIN     13
#define MAX_DISTANCE 400
 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
 
// Define Variables
 
float hum;    // Stores humidity value in percent
float temp;   // Stores temperature value in Celcius
float duration; // Stores HC-SR04 pulse duration value
float distance; // Stores calculated distance in cm
float soundsp;  // Stores calculated speed of sound in M/S
float soundcm;  // Stores calculated speed of sound in cm/ms
int iterations = 5;
 
// Initialize DHT sensor for normal 16mhz Arduino
 
DHT dht(DHTPIN, DHTTYPE); 

sensor_msgs::Range range_data;
std_msgs::String str_msg;

ros::NodeHandle nh;

void messageCb( const std_msgs::String& incoming_msg )
{
  str_msg.data = incoming_msg.data;
  lcd.clear();
  lcd.setCursor(0, 0);
   lcd.print(str_msg.data);
   
}

ros::Publisher pub_range( "/ultrasound_range", &range_data);
ros::Subscriber<std_msgs::String> sub("Browser_Command", &messageCb );


 
void setup() {
  nh.initNode();
  nh.advertise(pub_range);
  nh.subscribe(sub);
  String command;
  
  //Serial.begin (9600);
  dht.begin();
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("Temp--Hum--Dist");
  
}
 
void loop()
{

 
  delay(500);  // Delay so DHT-22 sensor can stabalize
  
   
    hum = dht.readHumidity();  // Get Humidity value
    temp= dht.readTemperature();  // Get Temperature value
    
    // Calculate the Speed of Sound in M/S
    soundsp = 331.4 + (0.606 * temp) + (0.0124 * hum);
    
    // Convert to cm/ms
    
    soundcm = soundsp / 10000;
    
  duration = sonar.ping_median(iterations);
  
  // Calculate the distance
  distance = (duration / 2) * soundcm;


    //////////// fill up the message data ///////////
    

    range_data.min_range = temp;
    range_data.max_range   = hum;
    range_data.range   = distance;
    pub_range.publish(&range_data);
    nh.spinOnce();


     // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);
  
  lcd.print(temp,1);
  lcd.print("  ");
  lcd.print(hum,1);
  lcd.print(" ");
  lcd.print(distance,2);
  
    delay(500);
   
  
  
}
