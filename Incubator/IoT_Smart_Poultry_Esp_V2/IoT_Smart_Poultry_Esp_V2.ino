// Relays sensor readings from arduino to an IoT server
// by: Emmanuel Agossou
 
// ======= flush the serial before sending data to remove old data ==============
//#include <ESP8266WiFi.h>
#include<SoftwareSerial.h>
SoftwareSerial mySerial1(0,4); //Rx,Tx pins // Rx, TX
//SoftwareSerial mySerial2(16,17); //Rx,Tx pins // Rx, TX
//SoftwareSerial mySerial3(14,15); //Rx,Tx pins // Rx, TX
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#include <Wire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>

const char* ssid     = "manu";
const char* password = "living2020";

//const char* ssid = "Buffalo-G-11D0";
//const char* password = "xh6h8bsbhern6";

String apiKey = "CSXUL887Y7O40MJB"; // api for Thingspeak write
const char* serverThinkspeak = "api.thingspeak.com";
String output; // outpout on webpage
float humidity, temperature, temperatureC;
int fanState,lightState,windowsState,eggRotatorState,humidifierState,flameOdor;
WiFiServer server(80);
//WebServer server;
// for MQTT
#define mqtt_port 1883
#define mqtt_server "broker.hivemq.com"
#define MQTT_USER "smartpoultry"
#define MQTT_PASSWORD "smartpoultry"
#define MQTT_SERIAL_PUBLISH_CH "/ic/esp32/serialdata/uno/"

//****** For the online webplatform *********************************************
char status;
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
 
// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;
String dateTime;
// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://smartpoultry.atwebpages.com/post-data.php";
// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";  //The apiKeyValue is just a random string that you can modify. It’s used for security reasons, so only anyone that knows your API key can publish data to your database
String groupName = "Smart_Poultry";
String location = "HIH";
//**********************************************************


WiFiClient client;
//WiFiClient MQTTclient;
//PubSubClient client(MQTTclient);



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

     // Initialize a NTPClient to get time
          timeClient.begin();
          // Set offset time in seconds to adjust for your timezone, for example:
          // GMT +1 = 3600
          // GMT +8 = 28800
          // GMT -1 = -3600
          // GMT 0 = 0
          timeClient.setTimeOffset(32400);
   
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
   //flameOdor=atoi(newString[7]);
   flameOdor=0;
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
  output+="flameOdor:"+String(flameOdor);
  //serve the data as plain text, for example
  //server.send(200,"text/plain",output);


	   //send data to Thingspeak
	  sendDataToThingSpeak();
	  
	  //send data to the Online Platforme at http://smartpoultry.atwebpages.com/
	  sendDataToWebPage();

    //send data to MQTT host: test.mosquitto.org , topic: smartpoultry
    sendDataToMQTT();
  
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
		
		postStr +="&field9=";
        postStr += String(flameOdor);
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

// function to send data to the online platform
//function to send data to online WebPage

void sendDataToWebPage(){   
      HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
     
        // Your Domain name with URL path or IP address with path
        http.begin(serverName);        
        // Specify content-type header
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
         while(!timeClient.update()) {
            timeClient.forceUpdate();
          }
            // The formattedDate comes with the following format:
          // 2018-05-28T16:00:13Z
          // We need to extract date and time
          formattedDate = timeClient.getFormattedDate();
          // Extract date
          int splitT = formattedDate.indexOf("T");
          dayStamp = formattedDate.substring(0, splitT);
        
          // Extract time
          timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
         
          dateTime=dayStamp+" "+timeStamp;
          
          // Prepare your HTTP POST request data
          String httpRequestData = "api_key=" + apiKeyValue + "&User=" + groupName+ "&location=" + String(location)+ "&dateTime=" + String(dateTime)+ "&temperature=" + String(temperature) + "&temperatureC=" + String(temperatureC) + "&humidity=" + String(humidity) + "&fanState=" + String(fanState) + "&eggRotatorState=" + String(eggRotatorState)+ "&windowsState=" + String(windowsState)+ "&humidifierState=" + String(humidifierState)+ "&flameOdor=" + String(flameOdor)+" ";

		  
		  
		  Serial.print("httpRequestData: ");
          Serial.println(httpRequestData);
          // Send HTTP POST request
          int httpResponseCode = http.POST(httpRequestData);
                    
          if (httpResponseCode>0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            Serial.print("Data sent to the database");
          }
          else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
            Serial.print("Data not sent to the database");
          }
          // Free resources
          http.end();
}



//function for MQTT
void sendDataToMQTT() {  
  WiFiClient MQTTclient;
  PubSubClient client(MQTTclient);
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");    
    client.setServer(mqtt_server, mqtt_port);   
    // Create a random client ID
    String clientId = "19870321";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) {
      Serial.println("\n Successfully connected to MQTT");
      //Once connected, publish an announcement...
       client.publish("smartpoultry", "Hello from Smart poultry");
      String temperatureStr=String(temperature)+" F";
      String temperatureCStr=String(temperatureC)+" °C";
      String humidityStr=String(humidity)+" %";
      String fanStateStr,eggRotatorStateStr,windowsStateStr,humidifierStateStr,flameOdorStr;
      if(fanState==1) fanStateStr="Fan ON";
      else fanStateStr="Fan OFF";

      if(eggRotatorState==1) eggRotatorStateStr="Egg Rotator ON";
      else fanStateStr="Egg Rotator OFF";

       if(windowsState==1) windowsStateStr="Windows ON";
      else fanStateStr="Windows OFF";

       if(humidifierState==1) humidifierStateStr="Humidifier ON";
      else fanStateStr="Humidifier OFF";

      if(flameOdor==1) flameOdorStr="FLAME ODOR";
      else flameOdorStr="NO FLAME ODOR";

      char temperature_buff[20],temperatureC_buff[20],humidity_buff[20],fanState_buff[20],eggRotatorState_buff[20],windowsState_buff[20],humidifierState_buff[20],flameOdor_buff[20];
      
       temperatureStr.toCharArray(temperature_buff, temperatureStr.length()+1);
       temperatureCStr.toCharArray(temperatureC_buff, temperatureCStr.length()+1);
       humidityStr.toCharArray(humidity_buff, humidityStr.length()+1);
       fanStateStr.toCharArray(fanState_buff, fanStateStr.length()+1);
       eggRotatorStateStr.toCharArray(eggRotatorState_buff, eggRotatorStateStr.length()+1);
       windowsStateStr.toCharArray(windowsState_buff, windowsStateStr.length()+1);
       humidifierStateStr.toCharArray(humidifierState_buff, humidifierStateStr.length()+1);
       flameOdorStr.toCharArray(flameOdor_buff, flameOdorStr.length()+1);
       
      client.publish("smartpoultry", "Hello From Smart poultry");
      
       client.publish("smartpoultry/temperatureF", temperature_buff);
       client.publish("smartpoultry/temperatureC", temperatureC_buff);
       client.publish("smartpoultry/humidity", humidity_buff);
       client.publish("smartpoultry/fanState", fanState_buff);
       client.publish("smartpoultry/eggRotatorState", eggRotatorState_buff);
       client.publish("smartpoultry/windowsState", windowsState_buff);
       client.publish("smartpoultry/humidifierState", humidifierState_buff);
       client.publish("smartpoultry/flameOdor", flameOdor_buff);
       
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}
