const int sensorPin = 2;
const int sensor2Pin=3;
const int sensor3Pin = 5;
const int ledPin = 9;

int lightCal;     //value of light sensors at initialization
int light2Cal;
int light3Cal;
int lightVal;     //value of light sensors at each cycle
int light2Val;
int light3Val;
String light1Status;
String light2Status;
String light3Status;
String directionGoing;

boolean thresholdStarted;
boolean threshold2Started;
boolean threshold3Started;
boolean light1Reset;
boolean light2Reset;
boolean light3Reset;
int thresholdCounter;
int threshold2Counter;
int threshold3Counter;
int light1Threshold;
int light2Threshold;
int light3Threshold;
int roomCounter;    
int thresholdBetweenPeople;

void setup()
{
  Serial.begin(38400);
  // We'll set up the LED pin to be an output.
  pinMode(ledPin, OUTPUT);
  lightCal = analogRead(sensorPin);
  light2Cal=analogRead(sensor2Pin);
  light3Cal=analogRead(sensor3Pin);
  light1Status="off";
  light2Status="off";
  light3Status="off";
  thresholdStarted=false;
  threshold2Started=false;
  threshold3Started=false;
  thresholdCounter=0;
  threshold2Counter=0;
  threshold3Counter=0;
  light1Threshold=100;
  light2Threshold=100;
  light3Threshold=100;
  roomCounter=0;
  light1Reset=true;
  light2Reset=true;
  light3Reset=true;
  thresholdBetweenPeople=100;
  
  
  
  //we will take a single reading from the light sensor and store it in the lightCal
  //variable. This will give us a prelinary value to compare against in the loop
}


void loop()
{
  //Take a reading using analogRead() on sensor pin and store it in lightVal
  lightVal = digitalRead(sensorPin);
  light2Val = digitalRead(sensor2Pin);
  light3Val = digitalRead(sensor3Pin);
  //Serial.print(lightVal);
  //Serial.print("      ");
  //Serial.println(light2Val);


  //if lightVal is less than our initial reading (lightCal) minus 50 it is dark and
  //turn pin 9 HIGH. The (-50) part of the statement sets the sensitivity. The smaller
  //the number the more sensitive the circuit will be to variances in light
  if (lightVal == LOW)
  {
    digitalWrite(9, HIGH);
    light1Status="connected";
    Serial.print(light1Status);
    Serial.print("     ");
    light1Reset=true;
  }
  //else, it is bright, turn pin 9 LOW
  else
  {
    digitalWrite(9, LOW);
    light1Status="broken";
    Serial.print(light1Status);
    Serial.print("     ");
    if(light1Reset==0 && thresholdStarted==0){}
    else{
      thresholdStarted=true;}
  }
  if(thresholdCounter>light1Threshold){
    thresholdCounter=0;
    thresholdStarted=false;
  }
  
  if (light2Val == LOW)
  {
    digitalWrite(9, HIGH);
    light2Status="connected";
    Serial.print(light2Status);
    Serial.print("     ");
    light2Reset=true;
  }
  else
  {
    digitalWrite(9, LOW);
    light2Status="broken";
    Serial.print(light2Status);
    Serial.print("     ");
    threshold2Started=true;
    //someone got in the middle
  }
   if(threshold2Counter>light2Threshold){
    threshold2Counter=0;
    threshold2Started=false;
    directionGoing="";
  }
  
  if (light3Val == LOW)
  {
    digitalWrite(9, HIGH);
    light3Status="connected";
    Serial.print(light3Status);
    Serial.print("     ");
    light3Reset=true;
  }
  else
  {
    digitalWrite(9, LOW);
    light3Status="broken";
    Serial.print(light3Status);
    Serial.print("     ");
    if(light3Reset==0 && threshold3Started==0){}
    else{
      threshold3Started=true;}
    
  }
  if(threshold3Counter>light3Threshold){
    threshold3Counter=0;
    threshold3Started=false;
  }




  
  //code for going in the forward direction
  if(thresholdStarted==true && light1Reset==true){
    thresholdCounter+=1;
    if(light2Status=="broken" && light1Reset==true && light2Reset==true){     
        if(threshold2Started==true){
          threshold2Counter+=1;
        }
        if(thresholdCounter>threshold2Counter){
          thresholdStarted=false;
          thresholdCounter=0;
          light1Reset=false;
          directionGoing="forward";
        }
    }    
  }

  if(threshold2Started==true && directionGoing=="forward"){
    threshold2Counter+=1;
    if(light3Status=="broken" && light2Reset==true && light3Reset==true){
        if(threshold3Started==true){
          threshold3Counter+=1;
        }
        threshold2Counter=0;
        threshold3Counter=0;
        light2Reset=false;
        light3Reset=false;
        threshold2Started=false;
        threshold3Started=false;
        directionGoing="";
        roomCounter+=1;
        Serial.print("Person entering..."); 
        delay(thresholdBetweenPeople); 
    }  
  }



  //code for going in reverse direction
  if(threshold3Started==true && light3Reset==true){
    threshold3Counter+=1;
    if(light2Status=="broken" && light3Reset==true && light2Reset==true){
        if(threshold2Started==true){
          threshold2Counter+=1;
        }
        if(threshold3Counter>threshold2Counter){
          threshold3Started=false;
          threshold3Counter=0;
          light3Reset=false;
          directionGoing="reverse";
        }
    }    
  }

  if(threshold2Started==true && directionGoing=="reverse"){
    threshold2Counter+=1;
    if(light1Status=="broken" && light2Reset==true && light1Reset==true){
        if(thresholdStarted==true){
          thresholdCounter+=1;
        }
        threshold2Started=false;
        thresholdStarted=false;
        threshold2Counter=0;
        thresholdCounter=0;
        light2Reset=false;
        light1Reset=false;
        directionGoing="";
        roomCounter-=1;
        Serial.print("Person leaving...");
        delay(thresholdBetweenPeople); 
    }  
  }

  Serial.print("      ");
  Serial.print(thresholdCounter);
  Serial.print("      ");
  Serial.print(threshold2Counter);
  Serial.print("      ");
  Serial.print(threshold3Counter);
  Serial.print("      Room Count: ");
  Serial.println(roomCounter);
//  Serial.print("      light3Reset: ");
//  Serial.print(light3Reset);
//  Serial.print("      threshold3Started: ");
//  Serial.println(threshold3Started);
  
}
