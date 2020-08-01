
#include <Servo.h>
#include <Stepper.h> // Include the header file
#include <DHT.h>
#define DHTPIN1 26

#define DHTPIN2 27
#define DHTTYPE DHT11

DHT dht1=DHT(DHTPIN1,DHTTYPE);
DHT dht2=DHT(DHTPIN2,DHTTYPE);

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
int light=13;
int fan =20;
int stepCount;
   Servo myservo1; // create servo object to control a servo
   Servo myservo2; // create servo object to control a servo
   Stepper myStepper=Stepper(stepsPerRevolution, 42, 46,44,48);
   
   int potpin = 0; // analog pin used to connect the potentiometer
   int val; // variable to read the value from the analog pin
   bool doorOpen;

// serial data
String esp_output;
float humidity, temperature, temperatureC;
int fanState, lightState,windowsState,eggRotatorState,humidifierState;

String msg="";

//function setup 
void setup() {
    // put your setup code here, to run once:
   Serial.begin(9600);
  // Serial1.begin(9600);
      
   //Serial.println("Serial monitor started...");
    dht1.begin();
    dht2.begin();
   doorOpen=false;
   windowsState=0;
   
   pinMode(light,OUTPUT);
    pinMode(fan,OUTPUT);
    myStepper.setSpeed(5);
   // stepCount=stepsPerRevolution/2;
   myStepper.step(-1024);
   eggRotatorState=0;
   
   delay(3000);
   myStepper.step(1024);
   eggRotatorState=1;
   myservo1.attach(22); 

   // scale it to use it with the servo (value between 0 and 180)
   myservo1.write(154); // max close
   delay(1000);
   myservo1.write(60); //  max open
   delay(1000);

   myservo1.write(154); //  max close


      // scale it to use it with the servo (value between 0 and 180)
    myservo2.attach(24); 
   
    myservo2.write(169); // max close
    delay(1000);
    myservo2.write(90); //  max open

   delay(1000);
   myservo2.write(169); // max close

 // digitalWrite(light,HIGH);
  digitalWrite(fan,HIGH);
  fanState=1;
  
  humidifierState=0;  
 // delay(3000);
 // digitalWrite(light,LOW);
 // digitalWrite(fan,LOW);
}


void loop() {
      //Serial.println("Reading DTH ");
    delay(1000);
    //get temperature and humidity values and compute temperature average
    float humi=dht1.readHumidity();
    float tempC=dht1.readTemperature();
    float tempF=dht1.readTemperature(true);
    float tempF2=dht2.readTemperature(true);
     float tempC2=dht2.readTemperature();
    //Serial.println("Humidity: ");
    //Serial.println(humi);
   // Serial.println("Temperature");
    //Serial.println(tempC);
      //Serial.println(tempF);
      // Serial.println(tempF2);
    float avg=(tempF+tempF2)/2;
    float avgC=(tempC+tempC2)/2;
    humidity=humi;
    temperature=avg;
    temperatureC=avgC;
    
    if (avg>=99.5)
    {
     digitalWrite(light,LOW);
    // digitalWrite(fan,LOW);
     lightState=0;
    }
    else
    {
      digitalWrite(light,HIGH);
     // digitalWrite(fan,HIGH);
     lightState=1;
    }
    if (humi>50)
    {
      if (!doorOpen){
       myservo1.write(60); //  max open
       myservo2.write(90); //  max open
       doorOpen=true;
       windowsState=1;
      }
    }
    else if (humi<45)
    {
      if (doorOpen)
      {
      myservo1.write(154); //  max close
      myservo2.write(169); // max close
      doorOpen=false;
      windowsState=0;
      }
    }

      
      
      //Monitor serial communication
    
            msg=String(temperature);
            msg+=" "+String(temperatureC);
            msg+=" "+String(humidity);          
            msg+=" "+String(fanState);
            msg+=" "+String(lightState);
            msg+=" "+String(eggRotatorState);
            msg+=" "+String(windowsState);
           msg+=" "+String(humidifierState); 
            //Serial.println("message:");
            Serial.println(msg);
           // Serial1.print(msg);
            //if(Serial1.available()) Serial1.print(msg);
            //if(Serial2.available()) Serial2.print(msg);
            //if(Serial3.available()) Serial3.print(msg);
}
