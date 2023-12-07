/*copyright 2018 by Kenny Chapman
   visit batbuddy.org for full project information
   If you require a license, see
    www.batbuddy.org/contactus.php 
  T O'Neill 12/3/2023
  Added randum numbers for which track to play. search for whichMP3. 
  Ir sensor to turn on when some one is near.
  Could not find DFPlayer library that was used so I used the one from 
   DF Robot. https://github.com/DFRobot/DFRobotDFPlayerMini 
   Had to make TrueRandom and DFRobotDFPlayerMini work with IDE
*/
//Edit the following variables as necessary. Make sure they are accurate to your configuration

int trackDelay = 1000; //1000 is one second, 5000 is five seconds, etc. this is the minimum delay between retrigger of the prop.
int trackCount = 7; //this should be the total number of tracks in folder 01 and must include any preceding zeros to three places i.e. 2 tracks is 002, 5 tracks is 005, 12 tracks is 012

// nothing below here needs to be modified in order to get this to work
#include "Arduino.h"
#include "SoftRcPulseOut.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "TrueRandom.h"   ///https://github.com/sirleech/TrueRandom
SoftRcPulseOut jaw;
SoftwareSerial mySoftwareSerial(16, 17); // TX, RX RX has a 1K resistor inline to limit noise.
DFRobotDFPlayerMini myDFPlayer;

int audioVal;
int oldSwitchState = HIGH;
unsigned long switchPressTime;
const unsigned long debounceTime = 10; //10 Mseconds of debounce time for button
const int potPin = 0;  // analog pin used to connect the potentiometer
const int bPin = 5; //Button pin assigned momentary switch between pin 2 and Ground
int mode = 1;
int busy;
int track = 001;
int whichMP3 = 001;
// Define connection pir pins
int pirPin = 23;
int dataPin = 21;
int pirval = LOW ;
bool motionState = false; 
int lpin1 = 4;
int lpin2 = 5;
int lpin3 = 5;
void setup()
{
  pinMode(bPin, INPUT_PULLUP); //This is set up to be a readable momentary switch to control inputs 
  jaw.attach(6);  //Attaches pin 6 to jaw servo
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  Serial.println(F("DFRobot DFPlayer Mini "));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  
  //----Set volume----
  myDFPlayer.volume(27);  //Set volume value (0~30).
  
  
  //----Set different EQ----
 myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
 // myDFPlayer.EQ(DFPLAYER_EQ_POP);
//  myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
//  myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
//  myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
//  myDFPlayer.EQ(DFPLAYER_EQ_BASS);
  
  //----Set device we use SD as default----

  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
 
  //----Read information----
  Serial.print("MP3 Status: ");
  Serial.println(myDFPlayer.readState()); //read mp3 state
  Serial.print("Volume: ");
  Serial.println(myDFPlayer.readVolume()); //read current volume
  Serial.print("EQ: ");
  Serial.println(myDFPlayer.readEQ()); //read EQ setting
  Serial.print("Total File Count: ");
  Serial.println(myDFPlayer.readFileCounts()); //read all file counts in SD card
  Serial.print("Current File: ");
  Serial.println(myDFPlayer.readCurrentFileNumber()); //read current play file number
  Serial.print("File count in folder:(folder 1 specified) ");
  Serial.println(myDFPlayer.readFileCountsInFolder(01)); //read file counts in folder SD:/01
 // Serial.print("I threw a TrueRandom die and got ");
 // whichMP3 = TrueRandom.random(1, 7);
 // Serial.println(whichMP3);
  //myDFPlayer.play(whichMP3);  //Play the first mp3
    // Configure the pins as input or output:
  
}

void loop()
{ 
   Serial.print("I threw a TrueRandom die and got ");
  whichMP3 = TrueRandom.random(1, 7);
  Serial.println(whichMP3);

   // pir Read out the pirPin and store as val:
   pirval = digitalRead(pirPin);
  // If motion is detected (pirPin = HIGH), do the following:
  if (pirval == HIGH) {
    digitalWrite(dataPin, HIGH); // Turn on the trigger to allow for mp3 player to start.
    // Change the motion state to true (motion detected):
    if (motionState == false) {
      Serial.println("Motion detected!");
      motionState = true;
    }
  }
  // If no motion is detected (pirPin = LOW), do the following:
  else {
    digitalWrite(dataPin, LOW); // Turn off the mp3 player.

    // Change the motion state to false (no motion):
    if (motionState == true) {
      Serial.println("Motion ended!");
      motionState = false;
    }
  }
 if (motionState == true){
   myDFPlayer.play(whichMP3);  //Play the first mp3
   mp3; // read light bar inputs
  }
}

void mp3()
{
busy=analogRead(4);
  audioVal = -1;
  if(analogRead(lpin1) < 341) audioVal += 667;
  if(analogRead(lpin2) < 341) audioVal += 667;
  if(analogRead(lpin3) < 341) audioVal += 667;
  if(audioVal > 0) 
    {
    
  int jaw = audioVal;
    }
  int switchState = digitalRead (bPin);// see if switch is open or closed
  
  // has it changed since last time?
  if (switchState != oldSwitchState)
     {
    // debounce
    if (millis () - switchPressTime >= debounceTime)
       {
       switchPressTime = millis();  // when we closed the switch 
       oldSwitchState =  switchState;  // remember for next time 
       
     if (analogRead(4) <= 300) //busy pin from mp3 player
      {
        Serial.print ("Busy: ");
        Serial.println (busy);
      }
     else if (analogRead(4) >= 301)
        {
           mode= mode+1; 
           Serial.print("Mode: ");
           Serial.println(mode);
           delay(15); 
           if (mode==1) 
           {track= TrueRandom.random(1, 6);
           Serial.println(track);
           myDFPlayer.playFolder(01, track);}
           if (mode==2) 
           {delay(trackDelay);}
           if (mode==3)
           {mode=0;}
        }
 /* if (myDFPlayer.available()) 
      {
       printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
      }*/
      }
    }
}

void printDetail(uint8_t type, int value);
void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
