//Working mqtt code for sih 2019
#include <MQTT.h>
#include <MQTTClient.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
 
const char* ssid     = "iQube";
const char* password = "itsnewagain";
int inputPin = 13; //d7
int val = 0; 
int count=0;
char flag='1';
unsigned long last_sec=0;
unsigned long current_sec=0;
WiFiClient WiFiclient;
MQTTClient client;
Servo myservo;
unsigned long times;

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

 void onmessageReceived(MQTTClient *client, char topic[], char payload[], int length) {
  
//char string[length];
//memcpy(string, payload, sizeof payload);
//string[sizeof payload] = '\0';
int x; 
sscanf(payload, "%d", &x); 
Serial.println(x);
myservo.write(x);   
  }

  
void setup() {
 Serial.begin(9600);
 delay(10);
 Serial.println();
 Serial.println();
 pinMode(inputPin, INPUT);
 Serial.print("Connecting to ");
 Serial.println(ssid);

 myservo.attach(2);  // attaches the servo on pin D1 to the servo object
  
myservo.write(0);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }
 
 Serial.println("");
 Serial.println("WiFi connected");  
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
 delay(2000);
 
 Serial.print("connecting to MQTT broker...");
 client.begin("10.1.75.43", WiFiclient);
 //client.onMessage(messageReceived);
  client.onMessageAdvanced(onmessageReceived);

 connect();
// myservo.attach(9);  // attaches the servo on pin 9 to the servo object  
//  myservo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);      
}
 
void connect() {
 while (!client.connect("nodemcu", "try", "try")) {
   Serial.print(".");
 }
 
 Serial.println("\nconnected!");
 client.subscribe("servo");
}
 
void loop() {

 client.loop();
 if(!client.connected()) {
   connect();
 }
 times =millis()

  val = digitalRead(inputPin); 
   if (val == LOW)
   { 
      if(flag=='1')
      {
        count++; 
        flag='2';
        int c = (times-last_sec)/1000;
        Serial.println(c);
        last_sec=times;
        int d = 60/c;
        client.publish("/app", ""+c);
      }  
   } 
   else {
    flag='1';
   }
  delay(100);
}
