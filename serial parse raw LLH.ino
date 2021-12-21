
String myString;


//#include <ESP8266WiFi.h>
#include <PubSubClient.h>
/* Ethernet Library */
#include <SPI.h>
#include <Ethernet.h>
//#include <UIPEthernet.h>


#define maxload 100
#define delimiter ' '

byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xE5 };
IPAddress ip(123, 45, 0, 105);  
IPAddress server(123, 45, 0, 10);
//void (*do_reset) (void) = 0;

EthernetClient ethClient;
PubSubClient client(ethClient);



String last_message;
String buffer_message;
char a;

int Index1,Index2,Index3,Index4,Index5,Index6, Index7;
String secondValue, thirdValue, fourthValue, fifthValue, sixthValue, firstValue;
float latitude, longitude;

bool stringComplete = false;

unsigned long message_time;
unsigned long message_time_prev;

//HardwareSerial Serial2(PD6, PD5);

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial1.begin(38400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  Serial.println("Goodnight moon!");
  // set the data rate for the SoftwareSerial port

  client.setServer(server, 1883);
  client.setCallback(callback);
  Ethernet.begin(mac, ip);
}


static char lat_out[13];
static char long_out[13];

void loop() // run over and over
{

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  serialEvent1();
  message_time = millis() - message_time_prev;
  if (message_time > 1000){
    client.publish("GPS/lat",dtostrf(latitude,6,5,lat_out));
    client.publish("GPS/long",dtostrf(longitude,6,5,long_out));
    message_time_prev = millis();
  }
    
  }

void serialEvent1() {
  while (Serial1.available()) {
  if (Serial1.available() > 0){
    a = Serial1.read();
    myString += String(a);
   
    if (String(a) == ":"){      
      
      
      if (myString.length()>60) {
       Serial.println(myString);
      Index1 = myString.indexOf(' ');
      Index2 = myString.indexOf(' ', Index1+1);
      Index3 = myString.indexOf(' ', Index2+1);
      Index4 = myString.indexOf(' ', Index3+1);
      Index5 = myString.indexOf(' ', Index4+1);
      Index6 = myString.indexOf(' ' , Index5+1);
      Index7 = myString.indexOf(' ', Index6+1);
      secondValue = myString.substring(Index1+1, Index2);
      thirdValue = myString.substring(Index2+1, Index3);
      fourthValue = myString.substring(Index3+1, Index4);
      fifthValue = myString.substring(Index4+1, Index5);
      sixthValue = myString.substring(Index5+1, Index6);
      firstValue = myString.substring(Index6+1, Index7);
      //Serial.println(Index1);
      Serial.print("length:");Serial.println(myString.length());
      latitude = fourthValue.toFloat();
      Serial.print("latitude:");Serial.println(latitude,5);
      longitude = sixthValue.toFloat();
      Serial.print("longitude:");Serial.println(longitude,5);
     
      /*
      Serial.print("3:");Serial.println(thirdValue.toFloat());
      Serial.print("4:");Serial.println(fourthValue.toFloat());
      Serial.print("5:");Serial.println(fifthValue.toFloat());
      Serial.print("6:");Serial.println(sixthValue.toFloat());
      */
      //Serial.print("d4:");Serial.println();
         
      
       
    }
    myString = "";
    
     }  
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection to ");
    //Serial.print(mqtt_server);
    //Serial.print(" : ");
    // Attempt to connect
    if (client.connect("GPS")) {
      Serial.println(" connected");
      /*client.publish(latitudePublish,"connected");
      client.publish(longitudePublish,"connected");
      */
     
      // Once connected, publish an announcement...
      // ... or not...
      // ... and resubscribe
      client.subscribe("MainControl");
    } else {
    //do_reset();
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 seconds");
      // Wait 1 seconds before retrying
      delay(1000);
    }
  }
}
