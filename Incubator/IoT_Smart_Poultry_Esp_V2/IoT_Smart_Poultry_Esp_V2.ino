// Relays sensor readings from arduino to an IoT server
// by: Stephan Ofosuhene
 
// ======= flush the serial before sending data to remove old data ==============
//#include <ESP8266WiFi.h>
#include<SoftwareSerial.h>
SoftwareSerial mySerial1(0,4); //Rx,Tx pins // Rx, TX
//SoftwareSerial mySerial2(16,17); //Rx,Tx pins // Rx, TX
//SoftwareSerial mySerial3(14,15); //Rx,Tx pins // Rx, TX
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
const char* ssid     = "manu";
const char* password = "living2020";

//const char* ssid = "Buffalo-G-11D0";
//const char* password = "xh6h8bsbhern6";

String apiKey = "CSXUL887Y7O40MJB"; // api for Thingspeak write
const char* serverThinkspeak = "api.thingspeak.com";
String output; // outpout on webpage
float humidity, temperature, temperatureC;
int fanState,lightState,windowsState,eggRotatorState,humidifierState;
WiFiServer server(80);
//WebServer server;
// for MQTT
#define mqtt_port 1883
#define MQTT_USER "mqtt username"
#define MQTT_PASSWORD "mqtt password"
#define MQTT_SERIAL_PUBLISH_CH "/ic/esp32/serialdata/uno/"

WiFiClient client;

//PubSubClient client(wifiClient);



// function setup starts
void setup() {
      Serial.begin(115200);
      Serial1.begin(9600);
      delay(100);
     
      // We start by connecting to a WiFi network
       WiFi.begin(ssid,password);
      Serial.begin(115200);
      while(WiFi.status()!=WL_CONNECTED)
      {
        Serial.print(".");
        delay(500);
      }
      Serial.println("");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());

      mySerial1.begin(9600);

     
   
}
 
void loop() {
  
  String msg= mySerial1.readStringUntil('\r');

/*  
 if(msg==""){
    msg= mySerial2.readStringUntil('\r');
    if(msg==""){
      msg= mySerial3.readStringUntil('\r');
    }
  }
  
  */
  
   Serial.print("Message:");
  Serial.print(msg);
  //split msg
  const char *str1 = msg.c_str();
  char newString[10][10]; 
    int i,j,ctr;
    j=0; ctr=0;
    for(i=0;i<=(strlen(str1));i++)
    {
        // if space or NULL found, assign NULL into newString[ctr]
        if(str1[i]==' '||str1[i]=='\0')
        {
            newString[ctr][j]='\0';
            ctr++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else
        {
            newString[ctr][j]=str1[i];
            j++;
        }
    }

  temperature=atof(newString[0]);
  temperatureC=atof(newString[1]);
  humidity=atof(newString[2]);
  fanState=atoi(newString[3]);
  lightState=atoi(newString[3]);
  eggRotatorState=atoi(newString[4]);
  windowsState=atoi(newString[5]);
   humidifierState=atoi(newString[6]);
    Serial.println("");
   Serial.print("temp:");
    Serial.println(temperatureC);
   Serial.print("Hum:");
    Serial.println(humidity);
    
  //Prepare the data for serving it over HTTP
   output="temperature:"+String(temperature)+"\n";
  output+="temperatureC:"+String(temperatureC);
  output+="fanState:"+String(fanState);
  output+="lightState:"+String(lightState);
  output+="eggRotatorState:"+String(eggRotatorState);
  output+="windowsState:"+String(windowsState);
  output+="humidifierState:"+String(humidifierState);
  //serve the data as plain text, for example
  //server.send(200,"text/plain",output);


   //send data to Thingspeak
  sendDataToThingSpeak();
  
 WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
             String htmlPage =
            String("HTTP/1.1 200 OK\r\n") +
            "Content-Type: text/html\r\n" +
            "Connection: close\r\n" +  // the connection will be closed after completion of the response
            "Refresh: 5\r\n" +  // refresh the page automatically every 5 sec
            "\r\n" +
            "<!DOCTYPE HTML>" +
            "<html>" + output +
            "</html>" +
            "\r\n";
             client.println(htmlPage);
          break;
        }
      }
    }
  
 
   }
 

}

//https://www.youtube.com/watch?v=6-RXqFS_UtU
//https://github.com/acrobotic/AI_Tips_ESP8266

//function to send data to thingspeak
void sendDataToThingSpeak(){
  
   if(client.connect(serverThinkspeak,80)) {
        String postStr = apiKey;
        
        postStr +="&field1=";
        postStr += String(temperature);
        
        postStr +="&field2=";
        postStr += String(temperatureC);
        
        postStr +="&field3=";        
        postStr += String(humidity);
        
        postStr +="&field4=";
        postStr += String(fanState);
        
        postStr +="&field5=";
        postStr += String(lightState);
        
        postStr +="&field6=";
        postStr += String(windowsState);
        
        postStr +="&field7=";
        postStr += String(eggRotatorState);
        
        postStr +="&field8=";
        postStr += String(humidifierState);
        postStr += "\r\n\r\n";
    
        Serial.println("Sending data to Thingspeak");
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);
        Serial.println(postStr);
    }
    else{
      Serial.println("Fails to send data to Thingspeak");
    }
    //client.stop();
  }   



/*
//function for MQTT
void reconnect() {
  client.setServer(mqtt_server, mqtt_port);
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    client.publish("smart_poultry", "Hello KIC");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) {
      Serial.println("connected");
      //Once connected, publish an announcement...
      client.publish("/temperatureF", temperature);
       client.publish("/temperatureC", temperatureC);
       client.publish("/humidity", humidity);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void publishSerialData(char *serialData){
  if (!client.connected()) {
    reconnect();
  }
  client.publish(MQTT_SERIAL_PUBLISH_CH, serialData);
}
*/
