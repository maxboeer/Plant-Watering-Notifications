#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

const char *API_ENDPOINT = "http://api.simplepush.io/send";
char ssid[] = "YOUR_WIFI_NAME";
char password[] = "YOUR_WIFI_PASSWORD";
String key = "YOUR_APP_KEY";

WiFiClient client;

const int plant1_pin = 36;
const int plant2_pin = 35;
const int plant3_pin = 34;

int plant1 = 0;
int plant2 = 0;
int plant3 = 0;

int plant1_warned = 0;
int plant2_warned = 0;
int plant3_warned = 0;


void setup() {
  pinMode(plant1_pin, INPUT);
  pinMode(plant2_pin, INPUT);
  pinMode(plant3_pin, INPUT);

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  delay(100);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
}

void loop() {
  checkPlants();
  delay(1000);
}


void checkPlants(){
  plant1 = analogRead(plant1_pin);
  plant2 = analogRead(plant2_pin);
  plant3 = analogRead(plant3_pin);

  Serial.println(plant1);
  Serial.println(plant2);
  Serial.println(plant3);
  Serial.println("_______________________________________________");

  if(plant1 >= 2200 && plant1_warned == 1){
    send("1", plant1, 1);
    plant1_warned = 2;
  }else if(plant1 >= 2000 && plant1_warned == 0){
    send("1", plant1, 0);
    plant1_warned = 1;
  }else if(plant1 < 1500){
    plant1_warned = 0;
  }

  if(plant2 >= 2200 && plant2_warned == 1){
    send("2", plant2, 1);
    plant2_warned = 2;
  }else if(plant2 >= 2000 && plant2_warned == 0){
    send("2", plant2, 0);
    plant2_warned = 1;
  }else if(plant2 < 1500){
    plant2_warned = 0;
  }

  if(plant3 >= 2200 && plant3_warned == 1){
    send("3", plant3, 1);
    plant3_warned = 2;
  }else if(plant3 >= 2000 && plant3_warned == 0){
    send("3", plant3, 0);
    plant3_warned = 1;
  }else if(plant3 < 1500){
    plant3_warned = 0;
  }
}

void send(String number, int measurement, bool important){
  
  Serial.println("Sending - " + number + " - " + measurement);
  String message, event;

  String title = "Pflanze " + number;

  if(important){
    message = "Ich verduste! - " + String(measurement);
    event = "WICHTIG!";
  }
  else{
    message = "Ich bin durstig! - " + String(measurement);
    event = "";
  }

  sendHttpPost(title, message, event);
}

void sendHttpPost(String title, String message, String event) {
  HTTPClient http;
	http.begin(client, API_ENDPOINT);
	http.addHeader("Content-Type", "application/x-www-form-urlencoded");

	String body = "key=" + key;

  if(title)
    body += "&title=" + title;
  
  body += "&msg=" + message;

  if(event)
    body += "&event=" + event;

	http.POST(body);
	http.end();
}