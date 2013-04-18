#include <LiquidCrystal.h>

#include <IRremote.h>

#include <Servo.h>







/* 

 * DOOR LIGHT

 * Reads data from light sensor module 
 
 * and then sends that data to the computer

 * so that you can see the sensor values

 * and the can use it as input to turn led on or off

 */

 

//Digital pin assignments
 
const boolean debug = true;  //set true for debug help - set false otherwise

const int doorLightPin = 7;  // doorLightPin connected to digital pin 4 green

const int roomLightPin = 4; // LED is connected to digital pin 13 red
 
const int IRrecvPin = 2;

const int windowServoPin = 9;




//Analog pin assignments

const int lightSensorPin = A3;

const int tempSensorPinAnalog = A1;              
 



//Other common settings

const int lightSensorThreshold = 30;      //Threshold for door light ON and OFF




//Temperature readings

int setTemperature = 22;  //Desired room temperature in celsius
 
int currentTemperature = 0;  //Actual room temperature in celsius




//Servo settings

const int servoWindowClose = 0; //degrees

const int servoWindowOpen = 180; //degrees
 
int currentServo = servoWindowClose;  //start with window closed




IRrecv irrecv(IRrecvPin);  //IR receiver object

Servo servo;







void setup()  
 
{  

       Serial.begin(9600); //initialize the serial port

       irrecv.enableIRIn(); // Start the receiver

       

       servo.attach(windowServoPin);

       servo.write(90);
 
       delay(1000);

       /*

       servoCloseWindow();

       delay(2000);

       servoOpenWindow();

       delay(2000);

       */

       pinMode(roomLightPin, OUTPUT); // sets the roomLightPin to be an output
 
       pinMode(doorLightPin, OUTPUT);      // sets the digital pin as output

       

       

       /*

       digitalWrite(roomLightPin, HIGH); // turn the LED on

       irrecv.enableIRIn(); // Start the receiver
 
       pinMode(doorLightPin, OUTPUT);    ue  // sets the digital pin as output

       */

}  

     

void loop()  // run over and over again 

{

  if(debug) Serial.println("Looping................................");

  controlDoorLight();

  controlRoomLight();

   readCurrentTemp();

  delay(500);

  

}//loop() =============================







void controlDoorLight(){

      int sensorValue = analogRead(lightSensorPin); // read the value from the sensor
 
      

      if(debug){

        Serial.print("Light sensor: ");

        Serial.println(sensorValue);

        delay(2000);

      }

      

       if( sensorValue > lightSensorThreshold ){

        if(debug) Serial.println("Door Light Off");

        digitalWrite(doorLightPin, LOW);
 
      }

      else

      {

        if(debug) Serial.println("Door Light On");

        digitalWrite(doorLightPin, HIGH);
 
      }

}//controlDoorLight() =======================










void controlRoomLight(){

  decode_results results;

  

  if(debug) Serial.println("Inside room light control");
 
  

  if (irrecv.decode(&results)) {    

    if(debug) Serial.println(results.value);

    

    switch (results.value) {

      case -1309689187:

        if(debug) Serial.println("Room Light On");
 
        digitalWrite(roomLightPin, HIGH);   // sets the lightBulb on

        break;

      

      case -555408703:

        if(debug) Serial.println("Room light Off");
 
        digitalWrite(roomLightPin, LOW);   // sets the lightBulb off

        break;

        

      default: 

        if(debug) Serial.println("Room Light No chance ...");
 
    }

    irrecv.resume(); // Receive the next value

  }  

}//controlRoomLight() ============================







void readCurrentTemp(){

  int temp = analogRead(tempSensorPinAnalog);    //get analog value  
 
  currentTemperature = temp/2;                    //refer to LM35 datasheet, celsius can get by devide by 2.  

  if(debug){

    Serial.print("Current temperature: ");

    Serial.print(currentTemperature);
 
    Serial.println(" Celsius");

  }

}//readCurrentTemp(){ =====================================




void servoCloseWindow(){

  if(currentServo == servoWindowClose){
 
    return;//do nothing

  }

  else if(currentServo > servoWindowClose)

  {

    for(int i=currentServo; i>=servoWindowClose; i--)

    {

      servo.write(i);
 
    }//for

  }

  else if(currentServo < servoWindowClose)

  {

    for(int i=currentServo; i<=servoWindowClose; i++)

    {

      servo.write(i);

     }//for

  }

}//servoCloseWindow() ==================







void servoOpenWindow(){

  servo.write(servoWindowOpen);

}//servoOpenWindow() =======================
