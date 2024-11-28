/ Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPL6XLlfa3kl"
#define BLYNK_TEMPLATE_NAME "Monitoring Suhu dan Jarak"
#define BLYNK_AUTH_TOKEN "v1okzVekidmclcQ0R0LwPnlK087P_TdD"
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHTesp.h"
DHTesp dht;

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ABZ";
char pass[] = "Z1d4n3J4b4r!";
//char ssid[] = "Binus-LabGAT";
//char pass[] = "qazxswedc123";
int rpm, a;
int i = 0;
String noted, note, notes;

#include <SoftwareSerial.h>
BlynkTimer timer;
SoftwareSerial NodeMCU(D1, D2);

#define SensorPin A0          // the pH meter Analog output is connected with the Arduino’s Analog
//for ph detect
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;

void pHSensor() {
 for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*3.3/1024/6; //convert the analog into millivolt (avgValue*3.86/1024/6)
  phValue=2.2*phValue;                      //convert the millivolt into pH value
  Serial.print("pH Aquarium:");  
  Serial.print(phValue,2);
  Serial.println(" ");
  Blynk.virtualWrite(V0, phValue);
}

void Turbidity () {
  while (NodeMCU.available() > 0) {
    rpm = NodeMCU.parseInt()*10;
    if (NodeMCU.read() == '\n') {
      Serial.println("Turbidity: " + String(rpm) + " NTU");
    }
  }
  Blynk.virtualWrite(V1, rpm);
  Blynk.virtualWrite(V11, noted);
  Serial.println(noted);

  if(rpm <= 900){
  noted ="Cloudy";
    
  }

  else if(rpm >= 1000){
  noted = "Sunny";
  }

  else {
  noted = "";
  }
  delay(50);
}

BLYNK_WRITE(V3){ ////LED Merah - Pompa Isi Air
  int pinValue = param.asInt();
  if(pinValue == 1){
    digitalWrite(D3, HIGH);

  }
  if(pinValue == 0){
     digitalWrite(D3, LOW);
  }
}

BLYNK_WRITE(V2){ ////LED Merah - Lampu UV
  int pinValue = param.asInt();
  if(pinValue == 1){
    digitalWrite(D4, HIGH);

  }
  if(pinValue == 0){
     digitalWrite(D4, LOW);
  }
}

BLYNK_WRITE(V7){ //LED Kuning - Pompa AB Mix
  int pinValue = param.asInt();
  if(pinValue == 1){
    digitalWrite(D5, HIGH);

  }
  if(pinValue == 0){
     digitalWrite(D5, LOW);
  }
}

BLYNK_WRITE(V8){ //LED Kuning - Pompa Utama
  int pinValue = param.asInt();
  if(pinValue == 1){
    digitalWrite(D6, HIGH);

  }
  if(pinValue == 0){
    digitalWrite(D6, LOW);
  }
}

void SensorTemp()
{  
  delay(dht.getMinimumSamplingPeriod());   
  float temperature = dht.getTemperature()+0.2; 
  /* Get temperature value */  
  Serial.print(dht.getStatusString()); 
  /* Print status communication */
  Blynk.virtualWrite(V5, temperature);
  Blynk.virtualWrite(V6, notes);
  Serial.print("\t\t");  
  Serial.print(temperature, 1);  
  Serial.print("\t\t");
  Serial.println(notes);
  Serial.print("\t\t");

  if(temperature <= 20){
  notes = "Cool"; 
  }
  
  else if(temperature >= 25 && temperature <= 30){
  notes = "Normal";
  }
  
  else if(temperature >= 35){
  notes = "Hot";  
  }
  
  else {
  notes = "";
  }
  delay(500);

}

void SensorHum()
{  
  delay(dht.getMinimumSamplingPeriod());   
  /* Delay of amount equal to sampling period */ 
  float humidity = dht.getHumidity()+0.2;    
  /* Get humidity value */ 
  Serial.print(dht.getStatusString()); 
  /* Print status communication */
  Blynk.virtualWrite(V4, humidity);
  Blynk.virtualWrite(V9, note);

  Serial.print("\t");  
  Serial.print(humidity, 1);  
  Serial.print("\t\t");  
  Serial.println(note);
  Serial.print("\t\t");

  if(humidity <= 50){
  note = "Dry";
  }
  
  else if(humidity >= 60 && humidity <= 80){
  note = "Normal";
  }
  
  else if(humidity >= 90){
  note = "Moist";
  }
  
  else {
  note = "";
  }
  delay(500);

}

void setup()
{
  // Debug console
  Serial.begin(9600);
  NodeMCU.begin(9600);
  
  pinMode(D1, INPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT); //LED Merah - Pompa Asam
  pinMode(D4, OUTPUT); //LED Merah - Pompa Basa
  pinMode(D5, OUTPUT); //LED Kuning - Pompa Isi Air
  pinMode(D6, OUTPUT); //LED Kuning - Pompa Buang Air
  
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t");  
  dht.setup(D7, DHTesp::DHT22);;  

  timer.setInterval(400L, Turbidity);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(10,22,228,129), 8080);
}

void loop()
{
  Blynk.run();
  pHSensor();
  SensorHum();
  SensorTemp();
  timer.run();
}