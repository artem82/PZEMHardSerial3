/*
Скетч для считывания pzem004t показаний с трех фаз. 
для трех фаз на основе Arduino 2560 (4 uart)
ver. 1.0
*/

// Arduino Mega пример использования всех 4 последовательных портов
// (Serial, Serial1, Serial2, Serial3)


#include <SoftwareSerial.h> // Arduino IDE <1.6.6
#include <PZEM004T.h>
#include <SPI.h>                //w5100
#include <Ethernet.h>           //w5100
#include <PubSubClient.h>       //mqtt

/****************  UART INFORMATION  **********************/
PZEM004T pzem1(&Serial1);
PZEM004T pzem2(&Serial2);
PZEM004T pzem3(&Serial3);
IPAddress ip1(192,168,10,1);
IPAddress ip2(192,168,10,2);
IPAddress ip3(192,168,10,3);

// Задаём mac и ip адреса в Локальной сети
byte mac[]    = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte server[] = { 192, 168, 1, 101 };  //ip Адрес для MQTT Брокера
byte ip[]     = { 192, 168, 1, 70 };   //ip Адрес Ethernet Shild'a Arduino

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
PubSubClient client(ethClient);       //PubSubClient client(server, 1883, callback, ethClient);  //у инсталятора

/*
************ MQTT INFORMATION (CHANGE THESE FOR YOUR SETUP) ******************

#define mqtt_server "192.168.1.101" // Enter your MQTT server adderss or IP. I use my DuckDNS adddress (yourname.duckdns.org) in this field
#define mqtt_user "iobroker" //enter your MQTT username
#define mqtt_password "1234" //enter your password
******************************************************************************
*/

float v1,v2,v3;
float i1,i2,i3;
float p1,p2,p3;
float e1,e2,e3;
unsigned long ttime;


void reconnect() {
    if (client.connect("PZEM004t_3","iobroker","1234")) {
      client.publish("mypower/connection", "true");
      
	 
	  //PubTopic();
      client.subscribe("myhome1/#");      
    }
}


/*******************************************/
/************* VOID SETUP ******************/
/*******************************************/
void setup() {
  Serial.begin(9600);      //компьютер rx-tx USB
  Serial1.begin(9600);     //rx1-tx1  
  Serial2.begin(9600);     //rx2-tx2
  Serial3.begin(9600);     //rx3-tx3
  pzem1.setAddress(ip1);
  pzem2.setAddress(ip2);
  pzem3.setAddress(ip3);
  
  client.setServer(server, 1883);
  client.setCallback(callback);
  Ethernet.begin(mac, ip);
  delay(250);
}
/*******************************************/
/************** VOID LOOP ******************/
/*******************************************/
void loop() {


if (!client.connected()) {
    reconnect();
  }
  
if (client.connected()) {
  client.loop(); 
  pzemstart();  
  }
 

  
  Serial.println();
  delay(9500);
}

void pzemstart() {
//1
 v1 = pzem1.voltage(ip1);
 i1 = pzem1.current(ip1);
 p1 = pzem1.power(ip1);
 e1 = pzem1.energy(ip1);
 
 //2 
 v2 = pzem2.voltage(ip2);
 i2 = pzem2.current(ip2);  
 p2 = pzem2.power(ip2);
 e2 = pzem2.energy(ip2);
//3
 v3 = pzem3.voltage(ip3);
 i3 = pzem3.current(ip3);  
 p3 = pzem3.power(ip3);
 e3 = pzem3.energy(ip3);
 
 ttime = millis();
 delay(100);
 
client.publish("mypower/connection", "true");
client.publish("mypower/phase1/voltage1", String(v1).c_str());	
delay(10); 
client.publish("mypower/phase2/voltage2", String(v2).c_str());
delay(10); 	
client.publish("mypower/phase3/voltage3", String(v3).c_str());
delay(10); 
client.publish("mypower/phase1/current1", String(i1).c_str());
delay(10); 
client.publish("mypower/phase2/current2", String(i2).c_str());
delay(10); 
client.publish("mypower/phase3/current3", String(i3).c_str());
delay(10); 
client.publish("mypower/phase1/power1", String(p1).c_str());
delay(10); 
client.publish("mypower/phase2/power2", String(p2).c_str());
delay(10); 
client.publish("mypower/phase3/power3", String(p3).c_str());
delay(10); 
client.publish("mypower/phase1/energy1", String(e1).c_str());
delay(10); 
client.publish("mypower/phase2/energy2", String(e2).c_str());
delay(10); 
client.publish("mypower/phase3/energy3", String(e3).c_str());
delay(10); 
client.publish("mypower/z_millis", String(ttime/1000).c_str());
delay(10); 
}








/*
 i1 = pzem1.current(ip1);
  if(i1 >= -0.1){ Serial.print(i);Serial.print("A; "); }  
  p1 = pzem1.power(ip1);
  if(p1 >= -0.1){ Serial.print(p);Serial.print("W; "); }  
  e1 = pzem1.energy(ip1);
  if(e1 >= -0.1){ Serial.print(e);Serial.print("Wh; "); }

*/


/* инсталятора
void callback(char* topic, byte* payload, unsigned int length) {
    payload[length] = '\0';
    String strTopic = String(topic);
    String strPayload = String((char*)payload);
    callback_iobroker(strTopic, strPayload);
}
*/