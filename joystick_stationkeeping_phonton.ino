#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>


const char* speed1Publish = "speedinput1";

static char station_keeping_status_send[10];

static char speed1_out[10];
static char speed2_out[10];
static char speed3_out[10];
static char speed4_out[10];

static char speed1m_out[10];
static char speed2m_out[10];
static char speed3m_out[10];
static char speed4m_out[10];

static char speed5_out[10];
static char speed6_out[10];
char buf[10];
const char* motor1Publish = "Steering_DP 1";
const char* motor2Publish = "Steering_DP 2";

const char* motor3Publish = "Steering_DP 3";
const char* motor4Publish = "Steering_DP 4";
const char* motor5Publish = "Steering_Central Kiri";
const char* motor6Publish = "Steering_Central Kanan";


const String turn_leftID = "Kiri";
const String turn_rightID = "Kanan";
const String Hold = "Tahan";

// DP1
const int speed1PIN = A2; //A1
const int button_L1 = 19; //2
const int button_R1 = 12; //3
int buttonstate1 = 0; 
int buttonstate2 = 0; 
int speed1;
int speed1_m;


//DP2
const int speed2PIN = A3; //a2
const int button_L2 = 10; //6
const int button_R2 = 9; //7
int buttonstate3 = 0; 
int buttonstate4 = 0;
int speed2;
int speed2_m;

//DP3
const int speed3PIN = A4; //a3
const int button_L3 = 7; //10  
const int button_R3 = 6;  //9
int buttonstate5 = 0; 
int buttonstate6 = 0; 
int speed3;
int speed3_m;
//DP4
const int speed4PIN = A1; //a4 
const int button_L4 = 3; //12  
const int button_R4 = 2;  //19
int buttonstate7 = 0; 
int buttonstate8 = 0;
int speed4;
int speed4_m;

//DP CENTRAL KANAN
const int speed5PIN = A5;
const int button_L5 = 14; 
const int button_R5 = 15;  
int buttonstate9 = 0; 
int buttonstate10 = 0; 
int speed5;


//DP CENTRAL KIRI
const int speed6PIN = A6;
const int button_L6 = 17; 
const int button_R6 = 18;  
int buttonstate11 = 0; 
int buttonstate12 = 0; 
int buttonstate10_prev;
int speed6;

unsigned long station_keeping_time;
unsigned long station_keeping_time_prev;
bool station_keeping_state;
bool station_keeping_state_prev;
bool station_keeping_status = 0;

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xE8 };
IPAddress ip(123, 45, 0, 108);
IPAddress server(123, 45, 0, 10);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
EthernetClient ethClient;
PubSubClient client(ethClient);
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("JoystickClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic","hello world");
      // ... and resubscribe
      client.subscribe("MainControl");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  } 
}
void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  // DP1
  digitalWrite(speed1PIN, INPUT);
  digitalWrite(button_L1, INPUT_PULLUP);
  digitalWrite(button_R1, INPUT_PULLUP);
  //DP2
  digitalWrite(speed2PIN, INPUT);
  digitalWrite(button_L2, INPUT_PULLUP);
  digitalWrite(button_R2, INPUT_PULLUP);
  //DP3
  digitalWrite(speed3PIN, INPUT);
  digitalWrite (button_L3, INPUT_PULLUP);  
  digitalWrite (button_R3, INPUT_PULLUP);  

  //DP4
  digitalWrite(speed4PIN, INPUT);
  digitalWrite (button_L4, INPUT_PULLUP);  
  digitalWrite (button_R4, INPUT_PULLUP); 
  //DP CENTRAL KANAN
  digitalWrite(speed5PIN, INPUT);
  digitalWrite (button_L5,  INPUT_PULLUP);  
  digitalWrite (button_R5, INPUT_PULLUP);  
  //DP CENTRAL KIRI
  digitalWrite(speed6PIN, INPUT);
  digitalWrite (button_L6, INPUT_PULLUP );  
  digitalWrite (button_R6, INPUT_PULLUP);  

  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);

  delay(1500);

 
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

     buttonstate1 = digitalRead(button_L1);  // DP LCT Propeller 1
     buttonstate2 = digitalRead(button_R1);  // DP LCT Propeller 1
     
     buttonstate3 = digitalRead(button_L2);  // DP LCT Propeller 2
     buttonstate4 = digitalRead(button_R2);  // DP LCT Propeller 2

     buttonstate5 = digitalRead(button_L3);  // DP  Propeller 3
     buttonstate6 = digitalRead(button_R3);  // DP  Propeller 3
     
     buttonstate7 = digitalRead(button_L4);  // DP  Propeller 4
     buttonstate8 = digitalRead(button_R4);  // DP  Propeller 4

     
     buttonstate9 = digitalRead(button_L5);  // DP  Propeller central Kanan
     buttonstate10 = digitalRead(button_R5);  // DP  Propeller central kanan
     
     buttonstate11 = digitalRead(button_L6);  // DP  Propeller central kiri
     buttonstate12 = digitalRead(button_R6);  // DP  Propeller central kiri

  speed1 = analogRead (speed1PIN);
  speed1 = map(speed1,0,1023,0,100);
  //analogWrite(speed1,map(speed1, 0,1023,0,100)); 
  


  speed1_m = map(speed1,0,100,132,220);
  //analogWrite(speed1_m,map(speed1_m, 0,1023,132,220)); 
 
  speed2 = analogRead (speed2PIN);
  speed2 = map(speed2,0,1023,100,0);
  //analogWrite(speed2,map(speed2, 0,1023,0,100)); 

  speed2_m = map(speed2,0,100,132,220); 
  //analogWrite(speed2_m,map(speed2_m, 0,1023,132,220)); 

  speed3 = analogRead (speed3PIN);
  speed3 = map(speed3,0,1023,0,100);
  //analogWrite(speed3,map(speed3, 0,1023,0,100)); 

  
  speed3_m = map(speed3,0,100,132,220); 
  //analogWrite(speed3_m,map(speed3_m, 0,1023,132,220)); 
  
  speed4 = analogRead (speed4PIN);
  speed4 = map(speed4,0,1023,0,100);
 // analogWrite(speed4,map(speed4, 0,1023,0,100)); 


  
  speed4_m = map(speed4,0,100,132,220); 
  //analogWrite(speed4_m,map(speed4_m, 0,1023,132,220)); 



  speed5 = analogRead (speed5PIN);
  speed5 = map(speed5,0,1023,703,388);

  speed6 = analogRead (speed6PIN);
  speed6 = map(speed6,0,1023,703,388);

/*
  Serial.print("speed1 :");
  Serial.println(speed1);
  //Serial.println("");
   Serial.print("speed2 :");
  Serial.println(speed2);
  //Serial.println("");
   Serial.print("speed3 :");
  Serial.println(speed3);
  //Serial.println("");
   Serial.print("speed4 :");
  Serial.println(speed4);
  //Serial.println("");
   Serial.print("speed5 :");
  Serial.println(speed5);
  //Serial.println("");
   Serial.print("speed6 :");
  Serial.println(speed6);
  Serial.println("");
  
  //delay(1000);
  */
// steering DP1
  if (buttonstate1 == LOW)
  {
    //digitalWrite(led, HIGH);
  //  Serial.print("Steering DP1 -->");
  //  Serial.println ("kiri");
    client.publish(motor1Publish,String(turn_leftID).c_str());
  }
  else{}
  
 

  if (buttonstate2 == LOW)
  {
    //digitalWrite(led, HIGH);
  //  Serial.print("Steering DP1 -->");
  //  Serial.println ("kanan");
    client.publish(motor1Publish,String(turn_rightID).c_str());
  }
  else{}
  
   if (buttonstate2 == HIGH && buttonstate1 == HIGH){
        //digitalWrite(led,LOW);
 //   Serial.print("Steering DP1 -->");
 //   Serial.println("stop");
    client.publish(motor1Publish,String(Hold).c_str());
    
   }
   else{}
  

  // steering DP2
  if (buttonstate3 == LOW)
  {
    //digitalWrite(led, HIGH);
  //  Serial.print("Steering DP2 -->");
  //  Serial.println ("kiri");
    client.publish(motor2Publish,String(turn_leftID).c_str());
  }
  else{}
 

  if (buttonstate4 == LOW)
  {
    //digitalWrite(led, HIGH);
  //  Serial.print("Steering DP2 -->");
  //  Serial.println ("kanan");
    client.publish(motor2Publish,String(turn_rightID).c_str());
  }
  else{}
   if (buttonstate3 == HIGH && buttonstate4 == HIGH){
        //digitalWrite(led,LOW);
  //  Serial.print("Steering DP2 -->");
  //  Serial.println("stop");
    client.publish(motor2Publish,String(Hold).c_str());
    
   }
   else{}


  
// steering DP3 error issue
   if (buttonstate5 == LOW)
  {
    //digitalWrite(led, HIGH);
    //Serial.print("Steering DP3 -->");
    //Serial.println ("kiri");
    client.publish(motor3Publish,String(turn_leftID).c_str());
  }
  else{}
 

  
   if (buttonstate6 == LOW)
  {
    //digitalWrite(led, HIGH);
    //Serial.print("Steering DP3 -->");
    //Serial.println ("kanan");
    client.publish(motor3Publish,String(turn_rightID).c_str());
  }
 else{}
  if (buttonstate5 == HIGH && buttonstate6 == HIGH){
        //digitalWrite(led,LOW);
    //Serial.print("Steering DP3 -->");
    //Serial.println("stop");
    client.publish(motor3Publish,String(Hold).c_str());
    
   }
   else{}

//-------------------------------------------------------------------------------------------------------------------------------------------------
/*
   // steering DP3 
   if (buttonstate5 == LOW)
  {
    //digitalWrite(led, HIGH);
    Serial.print("Steering DP3 -->");
    Serial.println ("kiri");
    client.publish("Steering3","Kiri");
  }
  else{}
 

  
   if (buttonstate6 == LOW)
  {
    //digitalWrite(led, HIGH);
    Serial.print("Steering DP3 -->");
    Serial.println ("kanan");
      client.publish("Steering3","Kanan");

  }
 else{}
  if (buttonstate5 == HIGH && buttonstate6 == HIGH){
        //digitalWrite(led,LOW);
    Serial.print("Steering DP3 -->");
    Serial.println("stop");
    client.publish("Steering3","Tahan");
    
   }
   else{}



*/



//steering DP4
     if (buttonstate7 == LOW)
  {
    //digitalWrite(led, HIGH);
    //Serial.print("Steering DP4 -->");
    //Serial.println ("kiri");
    client.publish(motor4Publish,String(turn_leftID).c_str());
  }
  else{}
  if (buttonstate8 == LOW)
  {
    //digitalWrite(led, HIGH);
   // Serial.print("Steering DP4 -->");
   // Serial.println ("kanan");
    client.publish(motor4Publish,String(turn_rightID).c_str());
  }
  else{}
    if (buttonstate7 == HIGH && buttonstate8 == HIGH){
        //digitalWrite(led,LOW);
   // Serial.print("Steering DP4 -->");
   // Serial.println("stop");
    client.publish(motor4Publish,String(Hold).c_str());
    
   }
   else{}


  //steering DP CENTRAL KANAN
     if (buttonstate9 == LOW)
  {
    //digitalWrite(led, HIGH);
  //  Serial.print("Steering CENTRAL Kanan -->");
  //  Serial.println ("kiri");
    client.publish(motor5Publish,String(turn_leftID).c_str());
  }

  else{}
 
      if (buttonstate10 == LOW)
  {
    //digitalWrite(led, HIGH);
    //Serial.print("Steering CENTRAL Kanan -->");
    //Serial.println ("kanan");
    client.publish(motor5Publish,String(turn_rightID).c_str());
  }
  else{}
   if (buttonstate9 == HIGH && buttonstate10 == HIGH){
        //digitalWrite(led,LOW);
    //Serial.print("Steering CENTRAL Kanan -->");
    //Serial.println("stop");
    client.publish(motor5Publish,String(Hold).c_str());
    
   }
   else{}


//steering DP CENTRAL KIRI 
     if (buttonstate11 == LOW)
  {
    //digitalWrite(led, HIGH);
    //Serial.print("Steering CENTRAL Kiri -->");
    //Serial.println ("kiri");
    client.publish(motor6Publish,String(turn_leftID).c_str());
  }
  else{}

  /*
      if (buttonstate12 == LOW) // station_keeping command
  {
    //digitalWrite(led, HIGH);
    Serial.print("Steering CENTRAL Kiri -->");
    Serial.println ("kanan");
    client.publish(motor6Publish,String(turn_rightID).c_str());
  }

  */
//  else{}
   if (buttonstate11 == HIGH && buttonstate12 == HIGH){
        //digitalWrite(led,LOW);
    //Serial.print("Steering CENTRAL Kiri -->");
    //Serial.println("stop");
    client.publish(motor6Publish,String(Hold).c_str());
    
   }
   else{}
   
  
   


  client.publish("speedinput1", dtostrf(speed1,3,0,speed1_out));
  client.publish("speedinput2", dtostrf(speed2,3,0,speed2_out));
  client.publish("speedinput3", dtostrf(speed3,3,0,speed3_out));
  client.publish("speedinput4", dtostrf(speed4,3,0,speed4_out));

  
  client.publish("Set_Speed1M", dtostrf(speed1_m,3,0,speed1m_out));
  client.publish("Set_Speed2M", dtostrf(speed2_m,3,0,speed2m_out));
  client.publish("Set_Speed3M", dtostrf(speed3_m,3,0,speed3m_out));
  client.publish("Set_Speed4M", dtostrf(speed4_m,3,0,speed4m_out));


  
  client.publish("Set_Speed5", dtostrf(speed5,3,0,speed5_out));
  client.publish("Set_Speed6", dtostrf(speed6,3,0,speed6_out));
  client.loop();


  station_keeping_time = millis() - station_keeping_time_prev;
  if (buttonstate10 == LOW) // station_keeping command
  {
   if (station_keeping_time > 2000){
    station_keeping_state = 1;
    if (station_keeping_state != station_keeping_state_prev){
      station_keeping_status = !station_keeping_status;
    }
   }
   
  } else {
    station_keeping_state = 0;
    station_keeping_time_prev = millis(); 
  }

  client.publish("station_keeping", dtostrf(station_keeping_status,3,0,station_keeping_status_send));

  station_keeping_state_prev = station_keeping_state;
  buttonstate10_prev = buttonstate10;
  Serial.print(station_keeping_time);
  Serial.print(" ");
  Serial.print(station_keeping_state);
   Serial.print(" ");
  Serial.println(station_keeping_state_prev);
}
