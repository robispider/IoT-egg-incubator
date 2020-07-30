
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
void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 Serial.println("Serial monitor started...");
 
 dht1.begin();
 dht2.begin();
 doorOpen=false;
 pinMode(light,OUTPUT);
  pinMode(fan,OUTPUT);
  myStepper.setSpeed(5);
 // stepCount=stepsPerRevolution/2;
 myStepper.step(-1024);
 delay(3000);
 myStepper.step(1024);
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
 // delay(3000);
 // digitalWrite(light,LOW);
 // digitalWrite(fan,LOW);
}

void loop() {
    Serial.println("Reading DTH ");
  delay(1000);
  float humi=dht1.readHumidity();
  float tempC=dht1.readTemperature();
  float tempF=dht1.readTemperature(true);
  float tempF2=dht2.readTemperature(true);
  Serial.println("Humidity: ");
  Serial.println(humi);
  Serial.println("Temperature");
  Serial.println(tempC);
    Serial.println(tempF);
     Serial.println(tempF2);
  float avg=(tempF+tempF2)/2;
  if (avg>=99.5)
  {
   digitalWrite(light,LOW);
  // digitalWrite(fan,LOW);
  }
  else
  {
    digitalWrite(light,HIGH);
   // digitalWrite(fan,HIGH);
  }
  if (humi>50)
  {
    if (!doorOpen){
     myservo1.write(60); //  max open
     myservo2.write(90); //  max open
     doorOpen=true;
    }
  }
  else if (humi<45)
  {
    if (doorOpen)
    {
    myservo1.write(154); //  max close
    myservo2.write(169); // max close
    doorOpen=false;
    }
  }


}
