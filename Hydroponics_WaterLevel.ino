// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPL6XLlfa3kl"
#define BLYNK_TEMPLATE_NAME "Monitoring Suhu dan Jarak"
#define BLYNK_AUTH_TOKEN "v1okzVekidmclcQ0R0LwPnlK087P_TdD"
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "Siambaton";
//char pass[] = "@E1!290699DmnkSmbtn";
//char ssid[] = "Binus-LabGAT";
//char pass[] = "qazxswedc123";

char ssid[] = "ABZ";
char pass[] = "Z1d4n3J4b4r!";

int i = 0;
String notex, nott;

#include <OneWire.h>  
#include <DallasTemperature.h>  
#define ONE_WIRE_BUS D3  
OneWire oneWire(ONE_WIRE_BUS);  
DallasTemperature sensors(&oneWire);  
float Celcius=0;

#define triggerPin  D1 //hijau
#define echoPin     D2 //biru

long bacaJarakCM(){
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(triggerPin, LOW);
  int durasi = pulseIn(echoPin, HIGH);
  return (durasi / 2) / 29.1; 
}

void deteksi() {
  int jarak = bacaJarakCM();
  Serial.print("Kondisi Volume Air: ");
  Serial.print(bacaJarakCM());
  Serial.print(" Distance | ");
  Serial.println(notex);
  Blynk.virtualWrite(V12, bacaJarakCM());
  Blynk.virtualWrite(V13, notex);

  if (jarak <= 8){
  notex ="Full";
  }
  else if(jarak >= 9 && jarak <=15) {
  notex = "Enough";
  }
  else if(jarak >= 25){
  notex = "Empty";
  } 
  else {
  notex = "";
  }
  delay(50);
}

void Temperature()
{
  // Request temperature to all devices on the data line
  sensors.requestTemperatures(); 
  
  Serial.print("Celsius temperature: ");
  //We can connect more than one IC on the same data wire. 0 refers to the first IC on the wire
  Serial.print(sensors.getTempCByIndex(0)); 
  Serial.println("                      ");
  int tempC=sensors.getTempCByIndex(0);
  delay(1000);
  
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V10, tempC);
    Blynk.virtualWrite(V14, nott);
    delay(500);
      Serial.println(nott);

  if(tempC <= 20){
  //digitalWrite(D4, HIGH);
  nott ="Cool";
    
  }
  else if(tempC >= 25 && tempC <= 30){
  nott = "Normal";
  
  }
  else if(tempC >= 35){
  nott = "Hot";
  }

  else {
  nott = "";
  }
  delay(50);
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  
  pinMode(triggerPin, OUTPUT);//D3
  pinMode(echoPin, INPUT);//D4

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(10,22,228,129), 8080);
}

void loop()
{
  Blynk.run();
  deteksi();
  Temperature();
  timer.run();
}