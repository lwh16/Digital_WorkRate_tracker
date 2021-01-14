//inclusion of the FastLED library (this has all the information to work the LEDs)
#include <FastLED.h>

//This is the pin on the arduino that gives the information for the LED strip
#define LED_PIN     3
//This says how many LEDs there are in the strip
#define NUM_LEDS    60

CRGB leds[NUM_LEDS];

bool flag = true;
int timer[NUM_LEDS][3];
int focusSplit[NUM_LEDS][3];
int Mode;
float Pi = 3.1415;
int RedVal;
int j;
int DeNom;

int firstVal;
int secondVal;
int thirdVal;

String dataInput[] = {"","",""};

bool focusDisplay = true;

void setFocusArray(int focusSplit[][3], float focusPercent)
{
  Serial.println("in focus array boi");
  int focusValSep = round(60*focusPercent) - 1;
  Serial.println(focusValSep);
  Serial.println("in focus array boi  22222");
  for (int i=0 ; i<NUM_LEDS ; i++)
  {
    if (i <= focusValSep)
    {
      focusSplit[i][0] = 0;
      focusSplit[i][1] = 255;
      focusSplit[i][2] = 0;
    }
    else
    {
      focusSplit[i][0] = 255;
      focusSplit[i][1] = 0;
      focusSplit[i][2] = 0;
    }
  }
}

void setTimeArray(int timer[][3], int sitTime)
{
  //take in the time in minutes
  Serial.println("sitTime = " + sitTime);
  for (int i=0 ; i<NUM_LEDS ; i++)
  {
    if (i <= sitTime)
    {
      timer[i][0] = 0;
      timer[i][1] = 0;
      timer[i][2] = 255;
    }
    else
    {
      timer[i][0] = 0;
      timer[i][1] = 0;
      timer[i][2] = 0;
    }
  }
 
}

int ShiftValue(int firstVal, int secondVal, int LEDVal)
{
  //move in the direction from the first val to the second val 
  if (firstVal < secondVal)
  {
    return LEDVal - 1;
  }
  if (secondVal < firstVal)
  {
    return LEDVal + 1;
  }
  if (firstVal == secondVal)
  {
    return LEDVal;
  }
  return LEDVal;
}

void blendArrays(int firstArray[][3], int secondArray[][3])
{
  //with time intervals transfer the led array from the first one to the second
  bool contFlag = true;
  bool checkLEDs = true;
  /*
  while (contFlag)
  {
    //check if the leds array has reached the secondArray values
    if (checkLEDs)
    {
      for (int i=0;i<NUM_LEDS;i++)
      {
        for (int j=0;j<3;j++)
        {
          //compare the values
          if (leds[i][j] != secondArray[i][j])
          {
            Serial.println("contFlag section break test");
            //not finished yet
            contFlag = true;
            checkLEDs = false;
            break;
          }
        }
      }
      //all LED values checked and correct
      contFlag = false;
      FastLED.show();
      return;
    }
    */
    //alter values from first-secondLEDs
    for (int j=0; j<255; j++)
    {
      for (int i=0; i<NUM_LEDS;i++)
      {
        int R;
        int G;
        int B;
        R = ShiftValue(firstArray[i][0], secondArray[i][0], leds[i].red);
        G = ShiftValue(firstArray[i][1], secondArray[i][1], leds[i].green);
        B = ShiftValue(firstArray[i][2], secondArray[i][2], leds[i].blue);
        leds[i].setRGB(R,G,B);
      }
      FastLED.show();
      delay(25);
  }
}

void setArray(int firstArray[][3])
{
  for (int i=0; i<NUM_LEDS;i++)
  {
    int R;
    int G;
    int B;
    R = firstArray[i][0];
    G = firstArray[i][1];
    B = firstArray[i][2];
    leds[i].setRGB(R,G,B);
  }
  FastLED.show();
}


void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(9600);
}


void loop() {
  if (Serial.available() > 0) {
    //Serial.println("Working");
  //collect the string data
    String data = Serial.readStringUntil('\n');
    Serial.println(data);
    //split it up
    int j = 0;
    int initial = 0;
    for (int i=0; i<data.length(); i++)
    {
      if (data.substring(i,i+1) == ",")
      {
        dataInput[j] = data.substring(initial,i);
        initial = i + 1;
        j +=1;
      }
    }

    //what is the user's state
    if (dataInput[0] == "still sitting")
    {
      String focusPercentage = dataInput[2];
      float focusPercent = focusPercentage.toFloat();
      Serial.println(focusPercentage);
      Serial.println("setting focus split");
      Serial.println(focusPercent);
      setFocusArray(focusSplit, focusPercent);
      String sitLong = dataInput[1];
      Serial.println("data = " + dataInput[1]);
      Serial.println(sitLong);
      int sitLength = sitLong.toInt();
      Serial.println(sitLength);
      setTimeArray(timer, sitLength);
      Serial.println(focusPercent);
      if (focusDisplay)
      {
        Serial.println(leds[5]);
        Serial.println("focus");
        //focusDisplay is on already
        //blendArrays(focusSplit, timer);
        setArray(focusSplit);
        focusDisplay = false;
      }
      else
      {
        Serial.println(leds[5]);
        Serial.println("time");
        setArray(timer);
        //blendArrays(timer, focusSplit);
        focusDisplay = true;
      }
      FastLED.show();
      delay(10000);
    }
    else
    {
      fill_solid( leds, NUM_LEDS, CRGB(0,0,0));
    }
  }
}
  
    
