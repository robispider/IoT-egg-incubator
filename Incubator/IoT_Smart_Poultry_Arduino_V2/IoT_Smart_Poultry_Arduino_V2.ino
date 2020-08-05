#include <Servo.h>
#include <Stepper.h> // Include the header file
#include <DHT.h> 
#include <LiquidCrystal.h>
//#include <DueTimer.h>

unsigned long beginTime;
String execProfile="chick";

#define DHTPIN1 26

#define DHTPIN2 27
#define DHTTYPE DHT11

DHT dht1=DHT(DHTPIN1,DHTTYPE);
DHT dht2=DHT(DHTPIN2,DHTTYPE);

// Create an LCD object. Parameters: (RS, E, D4, D5, D6, D7):(2, 3, 4, 5, 6, 7);
LiquidCrystal lcd = LiquidCrystal(7,6,11,10,9,8);

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
int light=13;
int fan =20;
int stepCount;
int uTrigPin=29;
int uEchoPin=28;//green
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
int incomingByte = 0; // for incoming serial data
String msg="";

//function setup 
void setup() {
   //noInterrupts();       
  
    // put your setup code here, to run once:
   Serial.begin(9600);
  // Serial1.begin(9600);
      // Specify the LCD's number of columns and rows. Change to (20, 4) for a 20x4 LCD:
  lcd.begin(16, 2);
    lcd.setCursor(2, 0);
  // Print the string 'Hello World!':
  lcd.print("Self Test");
   //Serial.println("Serial monitor started...");
    dht1.begin();
    dht2.begin();
   doorOpen=false;
   windowsState=0;
   
   pinMode(light,OUTPUT);
    pinMode(fan,OUTPUT);    
     pinMode(uTrigPin,OUTPUT);
   pinMode(uEchoPin,INPUT);
   
    myStepper.setSpeed(5);
   // stepCount=stepsPerRevolution/2;

   //Serial.println(Distance());
float d=(Distance()+Distance())/2;
    Serial.print(d);
    
     myStepper.step(-2000);
   Serial.println(String(d-((Distance()+Distance())/2)));
   
    myStepper.step(2000); 
    
         lcd.setCursor(2, 1);
  // Print the string 'Hello World!':
  lcd.print("Calibrating");
    
    GoToStepperPosition(4.0);

   eggRotatorState=0;

   eggRotatorState=1;
   lcd.setCursor(0, 1);
     lcd.print("Window A Test        ");
   myservo1.attach(22); 

   // scale it to use it with the servo (value between 0 and 180)
   myservo1.write(153); // max close
   delay(1000);
   myservo1.write(70); //  max open
   delay(1000);

   myservo1.write(153); //  max close
 lcd.setCursor(0, 1);
     lcd.print("Window B Test        ");

      // scale it to use it with the servo (value between 0 and 180)
    myservo2.attach(24); 
   
    myservo2.write(169); // max close
    delay(1000);
    myservo2.write(90); //  max open

   delay(1000);
   myservo2.write(169); // max close
     lcd.setCursor(0, 1);
     lcd.print("Light on        ");
 LightOn(true);
 delay(2000);
 lcd.print("Light Off        ");
 LightOn(false);
 lcd.setCursor(0, 1);
 lcd.print("Fan Off        ");
 digitalWrite(fan,HIGH);
 delay(2000);
 lcd.setCursor(0, 1);
 lcd.print("Test Complete        ");
  fanState=1;
  humidifierState=0;  
lcd.clear();
}

float Distance()
{
  delay(10);
  digitalWrite(uTrigPin,LOW);
  delay(2);
  digitalWrite(uTrigPin,HIGH);
  delay(10);
  digitalWrite(uTrigPin,LOW);

  float duration=pulseIn(uEchoPin,HIGH);
  float distance=(duration*0.0343)/2;
  //Serial.print("distance: ");
  //Serial.println(distance);
  return distance;
  
}
bool GoToStepperPosition(float f)
{
  if (f<4.0 || f>13.0)
{  return false;
}
  float StartingDistance=(Distance()+Distance())/2;
  float targetDistance=StartingDistance-f;
  int CurrentStep=0;
  int errorCounter=1;
  if (targetDistance>0)
  {
    CurrentStep=3;
  }
  else
  {
    CurrentStep=-3;
  }
  float currentPos=StartingDistance;
  float lastDistance=0;
       while(abs(currentPos-f)>0.1)
   {
    
    targetDistance=currentPos-f;
       if (targetDistance>0)
    {
      CurrentStep=3;
    }
    else
    {
      CurrentStep=-3;
    }
    myStepper.step(CurrentStep);
    float error=4.0;//initial false value
    float measure=0.0;
    int i=0;

    for (i=0;i<=10;i++)
    {
      float measure=(Distance());
      error=abs(currentPos-measure);
      if (error<=(errorCounter*0.12))
      {
       // Serial.print("less error");
        currentPos=measure;
        break;
      }
    }
    if (i>=10)
    {
      errorCounter++;
    }
    

   
    //lcd.setCursor(2, 1);
    //lcd.print(String(currentPos));
    
   }
}
String currentPos="begin";
void EggRotate()
{
  

   
    float d=(Distance()+Distance())/2;
  if (currentPos=="begin")
  {
    if (abs(d-4.0)>1.5)
    {
      Serial.print("egg rotator position error");
    }
    myStepper.step(-2000);
    //GoToStepperPosition(12.0);
    currentPos="end";
    d=(Distance()+Distance())/2;
       if (abs(d-12.5)>1.5)
    {
      Serial.print("egg rotator position error");
    }
  }
  else
  {
    if (abs(d-12.5)>1.5)
    {
      Serial.print("egg rotator position error");
    }
    
    myStepper.step(2000); 
    //GoToStepperPosition(5.0);
    currentPos="begin";
       if (abs(d-4.0)>1.5)
    {
      Serial.print("egg rotator position error");
    }
 
  }
  
  //  Serial.println("position:"+String(d));
}

String currentState="off";
int RunningSpan=300;
int RotationTime=30;
int RotationSpan=240;
float tempThreshold=100.5;
float humidityThreshold=55.0;
int windowTimer=300;
int windowSpan=10;
void LightOn(bool state)
{
  if (state && lightState==0)
  {
         digitalWrite(light,HIGH);
       lightState=1;
  }
  else if (!state && lightState==1)
  {
         digitalWrite(light,LOW);
       lightState=0;
  }
}

void SwitchOff()
{
    if (doorOpen)
          {
            myservo1.write(153); //  max close
            myservo2.write(168); // max close
            doorOpen=false;
            windowsState=0;
          }
     LightOn(false);
     //lcd.clear();
}
int loopCounter=0;

void loop() {

loopCounter++;

        if (Serial.available() > 0) {
          int cmd=Serial.parseInt();
          switch(cmd)
          {
            case 1://start
             if (currentState!="off")
            {
            return;//avoid double running command
            }
            if (execProfile=="test")
            {
              RunningSpan=300;
              RotationTime=30;
              RotationSpan=240;
              tempThreshold=82.0;
              humidityThreshold=55.0;
              windowTimer=50;
              windowSpan=10;
            }
            else if (execProfile=="chick")
            {
              RunningSpan=60*60*24*21;//seconds
              RotationTime=60*60*4;
              RotationSpan=60*60*24*17;
              tempThreshold=100.5;
              humidityThreshold=55.0;
              windowTimer=60*3;
              windowSpan=20;
            }
            else
            {
              return;
            }
            lcd.clear();
            currentState="heat";
            beginTime= millis();
            break;
            case 2://reset
            beginTime= millis();
            currentState="off";
            break;
            case 3://off
            currentState="off";
            SwitchOff();
            
           // Timer1.stop();
            break;
          
            case 4://test
  
            if (currentState="off")
            {
                execProfile="test";
            }
            break;
            case 5://chicken
             if (currentState="off")
            {
            execProfile="chick";
            }
            break;
            default:
            lcd.clear();
          }

  
    }
    if( Serial.available())
    {
    float d = Serial.parseFloat();
    if (d>0)
    {
      Serial.print("I received: ");
      Serial.println(String(d));
      GoToStepperPosition(d);
      lcd.clear();
      Serial.println("done positin");
    }
    }
  unsigned long  currentTime= (millis()-beginTime)/1000;
  
  bool FlipFlop=false;
 if ((millis()-beginTime)%1000==00)
 {
  FlipFlop=true;
  
 }
 else
 {
  FlipFlop=false;
 }
      float avg=0.0;
      float avgC=0.0;
      float humi=0.0;
  if (currentState=="heat")
  {
     if (FlipFlop)
     {
      LightOn(true);
       humi=dht1.readHumidity();
      float tempC=dht1.readTemperature();
      float tempF=dht1.readTemperature(true);
      float tempF2=dht2.readTemperature(true);
       float tempC2=dht2.readTemperature();
       avg=(tempF+tempF2)/2;
       avgC=(tempC+tempC2)/2;
      humidity=humi;
      temperature=avg;
      temperatureC=avgC;
      if (avg>=(tempThreshold-0.5))
      {
        currentState="run";
        beginTime=millis();
      }
     }
  }
  else if (currentState=="run")
  {
    if ( currentTime>RotationSpan)
      {
        currentState="hatch";
         if (execProfile=="test")
                {
                  RunningSpan=300;
                  RotationTime=5000;
                  RotationSpan=240;
                  tempThreshold=100.5;
                  humidityThreshold=70.0;
                  windowTimer=50;
                  windowSpan=15;
                }
                else if (execProfile=="chick")
                {
                  RunningSpan=60*60*24*21;//seconds
                  RotationTime=60*60*20000000;//make the length useless
                  RotationSpan=60*60*24*17;
                  tempThreshold=100.5;
                  humidityThreshold=70.0;
                  windowTimer=60*3;
                  windowSpan=15;
                }
      }

     
     if (currentTime%RotationTime==0)
     {
      EggRotate();
     }
     
     if (currentTime%windowTimer==0)
     {
        if (!doorOpen)
        {
           myservo1.write(70); //  max open
           myservo2.write(100); //  max open
           doorOpen=true;
           windowsState=1;
        }
     }
     if (doorOpen)
          {
     if (currentTime%windowSpan==0)
     {
          
            myservo1.write(153); //  max close
            myservo2.write(168); // max close
            doorOpen=false;
            windowsState=0;
          }
     }
     if (FlipFlop)
     {
       humi=dht1.readHumidity();
      float tempC=dht1.readTemperature();
      float tempF=dht1.readTemperature(true);
      float tempF2=dht2.readTemperature(true);
       float tempC2=dht2.readTemperature();
       avg=(tempF+tempF2)/2;
       avgC=(tempC+tempC2)/2;
      humidity=humi;
      temperature=avg;
      temperatureC=avgC;
      
      
      if (avg>=(tempThreshold-0.5))
      {
        
          LightOn(false);
      }
      else
      {
       
         LightOn(true);
      }
      if (humi>(humidityThreshold+5))
      {
      
      }
      else if (humi<(humidityThreshold-5))
      {
        
      }
       }
        //Monitor serial communication
    
         
    }
    else if (currentState=="hatch")
    {
          if ( currentTime>RunningSpan)
    {
    currentState="off";
       SwitchOff();
              return;
    }
      
       if (currentTime%windowTimer==0)
         {
          if (!doorOpen)
          {
             myservo1.write(70); //  max open
             myservo2.write(100); //  max open
             doorOpen=true;
             windowsState=1;
          }
         }
          if (doorOpen)
            {
           if (currentTime%windowSpan==0)
           {
             
                myservo1.write(153); //  max close
                myservo2.write(168); // max close
                doorOpen=false;
                windowsState=0;
              }
         }
         if (FlipFlop)
         {
         humi=dht1.readHumidity();
        float tempC=dht1.readTemperature();
        float tempF=dht1.readTemperature(true);
        float tempF2=dht2.readTemperature(true);
         float tempC2=dht2.readTemperature();
         avg=(tempF+tempF2)/2;
         avgC=(tempC+tempC2)/2;
        humidity=humi;
        temperature=avg;
        temperatureC=avgC;
     
        
        if (avg>=tempThreshold-0.5)
        {
          
           
           LightOn(false);
        }
        else
        {
          
          LightOn(true);
        }
        if (humi>(humidityThreshold+5))
        {
        
        }
        else if (humi<(humidityThreshold-5))
        {
          
        }
       }
  }
  else
  {
    SwitchOff();
  }
  if (FlipFlop)
  {
    String t="";
    if ( currentState=="heat")
    {
       t=" M"+String(currentTime/60);
    }
    else if( currentState!="off")
    {
      t=" M"+String( (RunningSpan/60)- currentTime/60);
    }
    
    String top=execProfile+":"+currentState+t;
    
    for (int i =top.length();i<=16;i++)
    {
      top+=" ";
    }
    lcd.setCursor(0, 0);
    lcd.print(top);
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
      String states="T:"+String(avg)+"H:"+String((int)humi);
      if (doorOpen)
      {
        states=states+"W";
      }
      if (lightState==1)
      {
        states=states+"L";
      }
      if (fanState==1)
      {
        states=states+"F";
      }
      
      lcd.setCursor(0, 1);
      lcd.print(states);
  }
  
}
