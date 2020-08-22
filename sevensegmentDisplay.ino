#include <SevSeg.h>
#include <math.h>
#include <Wire.h>
SevSeg sevseg1;
SevSeg sevseg2;
SevSeg sevseg3;
SevSeg sevseg4;
int ledRedPin=13;
int ledGreenPin=12;
int resetPin = A5;
const int buttonON = 11;
int on = 0;
//STEP COUNTER
const int xpin=A2;
int ypin=A3;
int zpin=A4;
int powerpin=A0;
int gnd=A1;
float threshhold=454.20;
float xval[100]={0};
float yval[100]={0};
float zval[100]={0};
float xavg;
float yavg;
float zavg;
int steps,privStep,flag=0;
void setup() {
//SEVEN SEGMENT
  byte numDigits = 4;
  byte digitPins[]={};
  byte segmentPins4[]={53, 51, 21, 22, 19, 50, 52, 23};
  byte segmentPins3[]={49, 47, 25, 26, 24, 46, 48, 27};
  byte segmentPins2[]={45, 43, 29, 30, 28, 42, 44, 31};
  byte segmentPins1[]={41, 39, 33, 34, 32, 38, 40, 35};
  bool updateWithDelaysIn = true;
  bool resistorsOnSegments = true;
  digitalWrite(resetPin,HIGH);
  pinMode(resetPin,OUTPUT);
  byte hardwareConfig = COMMON_CATHODE;
  byte hardwareConfig1=COMMON_ANODE;
  sevseg1.begin(hardwareConfig1, numDigits,digitPins, segmentPins1, resistorsOnSegments);
  sevseg1.setBrightness(90);
  sevseg2.begin(hardwareConfig, numDigits,digitPins, segmentPins2, resistorsOnSegments);
  sevseg2.setBrightness(90);
  sevseg3.begin(hardwareConfig1, numDigits,digitPins, segmentPins3, resistorsOnSegments);
  sevseg3.setBrightness(90);
  sevseg4.begin(hardwareConfig, numDigits,digitPins, segmentPins4, resistorsOnSegments);
  sevseg4.setBrightness(90);
//ledPins for HEARTBEAT sensor output
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  Serial.begin(9600);
  //STEP COUNTER
  pinMode(powerpin,OUTPUT);
  pinMode(gnd,OUTPUT);
  digitalWrite(powerpin,HIGH);
  digitalWrite(gnd,LOW);
  pinMode(13,OUTPUT);
  calibrate();
}
void calibrate()
{

  digitalWrite(13,HIGH);
  
  float sum=0;
  float sum1=0;
  float sum2=0;
  for (int i=0;i<100;i++)
  {
    xval[i]=float(analogRead(xpin));

    sum=xval[i]+sum;
  }
  delay(100);
  xavg=sum/100.0;

  for (int j=0;j<100;j++)
  {
    yval[j]=float(analogRead(ypin));

    sum1=yval[j]+sum1;
  }
  yavg=sum1/100.0;

  delay(100);
  for (int i=0;i<100;i++)
  {
    zval[i]=float(analogRead(zpin));
    
    sum2=zval[i]+sum2;
  }
  zavg=sum2/100.0;
  delay(100);
  digitalWrite(13,LOW);

}

void loop() {
  if(digitalRead(buttonON)== HIGH)
  {
    
    on++;     
   
    if(on%2!=0){     
         sevseg4.setNumber(5, 0);
                        sevseg1.setNumber(privStep%10);
                      sevseg2.setNumber((privStep-100*(privStep/100))/10);
                      sevseg3.setNumber(privStep/100);
                      for(int j=0;j<10000;j++){
                        
                        sevseg4.refreshDisplay();
                        sevseg1.refreshDisplay();
                        sevseg2.refreshDisplay();
                        sevseg3.refreshDisplay();
                      }
    //STEP COUNTER
    float acc=0;
    float totvect[100]={0};
    float totave[100]={0};
   
    float xaccl[100]={0};
    float yaccl[100]={0};
    float zaccl[100]={0};
   
          
          for (int i=0;i<100;i++)
          {
                                    if(digitalRead(buttonON)== HIGH)
                                    {
                                      on++;
                                      if(on%2==0){
                                        on++;
                                      digitalWrite(resetPin,LOW);
                                      }
                                      
                                    }
                                    
            xaccl[i]=float(analogRead(xpin));
            delay(1);
            
            yaccl[i]=float(analogRead(ypin));
            delay(1);
            
            zaccl[i]=float(analogRead(zpin));
            delay(1);
            
            totvect[i] = sqrt(((xaccl[i]-xavg)* (xaccl[i]-xavg))+ ((yaccl[i] - yavg)*(yaccl[i] - yavg)) + ((zval[i] - zavg)*(zval[i] - zavg)));
            totave[i] = (totvect[i] + totvect[i-1]) / 2 ;
            delay(100);
     //Serial.println( totave[i] );
          //cal steps
           
            if (totave[i]>threshhold && flag==0)
            {
              steps=steps+1;
              privStep = steps;
              flag=1;
                   //Serial.println('\n');
                        Serial.print(steps);
                      sevseg4.setNumber(5, 0);
                        sevseg1.setNumber(privStep%10);
                      sevseg2.setNumber((privStep-100*(privStep/100))/10);
                      sevseg3.setNumber(privStep/100);
                      for(int j=0;j<10000;j++){
                        
                        sevseg4.refreshDisplay();
                        sevseg1.refreshDisplay();
                        sevseg2.refreshDisplay();
                        sevseg3.refreshDisplay();
                      }
            
            }
  
    
            
            if (totave[i] > threshhold && flag==1){
              //do nothing 
              sevseg4.setNumber(5, 0);
                        sevseg1.setNumber(privStep%10);
                      sevseg2.setNumber((privStep-100*(privStep/100))/10);
                      sevseg3.setNumber(privStep/100);
                      for(int j=0;j<10000;j++){
                        
                        sevseg4.refreshDisplay();
                        sevseg1.refreshDisplay();
                        sevseg2.refreshDisplay();
                        sevseg3.refreshDisplay();
              //        }
            }
            if (totave[i] < threshhold  && flag==1)
            {
              flag=0;
           sevseg4.setNumber(5, 0);
                        sevseg1.setNumber(privStep%10);
                      sevseg2.setNumber((privStep-100*(privStep/100))/10);
                      sevseg3.setNumber(privStep/100);
                      for(int j=0;j<10000;j++){
                        
                        sevseg4.refreshDisplay();
                        sevseg1.refreshDisplay();
                        sevseg2.refreshDisplay();
                        sevseg3.refreshDisplay();
              //        }
            }
            
             }          
            
            
            if(i == 99)
            {
                           float acc=0;
                float totvect[100]={0};
                float totave[100]={0};
               
                float xaccl[100]={0};
                float yaccl[100]={0};
                float zaccl[100]={0}; 
            }
          
                                    if(digitalRead(buttonON)== HIGH)
                                    {
                                      on++;
                                      if(on%2==0){
                                        on++;
                                      digitalWrite(resetPin,LOW);
                                      }
                                      
                                    }
                                    
          }
          
    }

    }
     else
          {
            on++;
            digitalWrite(resetPin,LOW);
            
          }
          
       
          delay(20);

}
}


    
  
