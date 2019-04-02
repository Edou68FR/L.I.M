// Code made by Edouard 
// Output
int redPin = 11;   // Red LED,   connected to digital pin 3
int grnPin = 10;  // Green LED, connected to digital pin 5
int bluPin = 9;  // Blue LED,  connected to digital pin 6
int photor = A0; // Photo résistance, connected to analog pin A1

// Color arrays

int black[3]  = { 0, 0, 0 };
int white[3]  = { 100, 100, 100 };
int red[3]    = { 100, 0, 0 };
int green[3]  = { 0, 100, 0 };
int purple[3]  = { 40, 0, 100 };
int lgreen[3]  = { 0, 100, 0 };
int blue[3]   = { 0, 0, 100 };
int lblue[3]   = { 0, 80, 100 };
int yellow[3] = { 100, 100, 0 };
int dimWhite[3] = { 30, 30, 30 };

//Color arrays for notifications

int insta[3]  = { 0, 100, 0 };
int facebook[3]  = { 0, 0, 100 };
int twitter[3]  = { 0, 80, 100 };
int sms[3]  = { 0, 100, 0 };
int snap[3]  = { 100, 100, 0 };


int redVal = black[0];
int grnVal = black[1]; 
int bluVal = black[2];

int wait = 100;      // 10µs internal crossFade delay; increase for slower fades
int hold = 1;       // Optional hold when a color is complete, before the next crossFade
int repeat = 0;     // How many times should we loop before stopping? (0 for no stop)
int j = 0;          // Loop counter for repeat

int mode = 2;       // mode var for knowing actual mode ( 1 = Black, 2 = animation, 4 = Snap, 5 = Facebook, 6 = Twitter, 7 = SMS, 8 = Instagram, 9 = LightMode  )

// Initialize color variables
int prevR = redVal;
int prevG = grnVal;
int prevB = bluVal;

void serialEvent() // If data is available from Serial port
{
  char r;
  while(Serial.available()) 
  {
    r = (char)Serial.read();
    Serial.println(millis());
    Serial.println(r);
     switch(r){
      case 'b':
        mode = 1;
      break;
      case 'a':
        mode = 2;
      break;
      case 'n':
        mode = 4;
      break;
      case 'f':
        mode = 5;
      break;
      case 't':
        mode = 6;
      break;
      case 's':
        mode = 7;
      break;
      case 'i':
        mode = 8;
      break;
      case 'l':
        mode = 9;
      break;
    }
    Serial.end();  
    Serial.begin(9600);
  }
}


void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(grnPin, OUTPUT);   
  pinMode(bluPin, OUTPUT); 
  pinMode(photor, INPUT);
  Serial.begin(9600);
}

void loop()
{
  switch (mode) {
    wait = 100;
    case 1: // Black / Turn off
      while( !Serial.available() )
      {
        digitalWrite(redPin, LOW);
        digitalWrite(grnPin, LOW);
        digitalWrite(bluPin, LOW);
      }
    break;
    case 2: // Colorful animation
      wait = 2000;
      crossFade(blue);
      crossFade(lblue);
      crossFade(green);
      crossFade(lgreen);
      crossFade(yellow);
      crossFade(purple);
      crossFade(red);
      crossFade(white);
    break;
    case 4: // Snapchat notification
      for(int k; k <= 2; k++)
      {
        crossFade(snap);
        delay(900);
        crossFade(black);
        delay(900);
      }
    break;
    case 5: // Facebook notification
      for(int k; k <= 2; k++)
      {
        crossFade(facebook);
        delay(900);
        crossFade(black);
        delay(900);
      }
    break;
    case 6: // Twitter notification
      for(int k; k <= 2; k++)
      {
        crossFade(twitter);
        delay(900);
        crossFade(black);
        delay(900);
      }
    break;
    case 7: // SMS notification 
      for(int k; k <= 2; k++)
      {
        crossFade(sms);
        delay(900);
        crossFade(black);
        delay(900);
      }
    break;
    case 8: // Instagram notification
      for(int k; k <= 2; k++)
      {
        crossFade(insta);
        delay(900);
        crossFade(black);
        delay(900);
      }
    break;
    case 9:
      while( !Serial.available() )
      {
        if(analogRead(A0) < 50)
        {
          crossFade(red);
        }
        else if(analogRead(A0) > 50 && analogRead(A0) < 120 )
        {
          crossFade(purple);
        }
        else if(analogRead(A0) > 120)
        {
          crossFade(white);
        }
        delay(50);
      }
    break;
  }
  mode = 2; // Set mode to Colorful animation by default
}

// LightMode, color change from photoresistor, or a simple AnalogRead
void LightMode() 
{
  int val = analogRead(A1);
  if(val <= 600)
  {
    crossFade(red);
  }
  if(val >= 600 && val <= 800)
  {
    crossFade(green);
  }
   if(val >= 800 && val <= 1024)
  {
    crossFade(blue);
  }
  delay(100);
  if (repeat) { // Do we loop a finite number of times?
    j += 1;
    if (j >= repeat) { // Are we there yet?
      exit(j);         // If so, stop.
    }
  }
}



/* 
 DON'T TOUCH ANYTHING BELOW THIS LINE, COLORCROSSFADE FOR SMOOTH COLOR CHANGING
 ------------------------------------------------------------------------------------------
 */

int calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) {                      // If its non-zero, 
    step = 1020/step;              //   divide by 1020
  } 
  return step;
}

int calculateVal(int step, int val, int i) {

  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              //   increment the value if step is positive...
      val += 1;           
    } 
    else if (step < 0) {         //   ...or decrement it if step is negative
      val -= 1;
    } 
  }
  // Defensive driving: make sure val stays in the range 0-255
  if (val > 255) {
    val = 255;
  } 
  else if (val < 0) {
    val = 0;
  }
  return val;
}

void crossFade(int color[3]) {
  // Convert to 0-255
  int R = (color[0] * 255) / 100;
  int G = (color[1] * 255) / 100;
  int B = (color[2] * 255) / 100;

  int stepR = calculateStep(prevR, R);
  int stepG = calculateStep(prevG, G); 
  int stepB = calculateStep(prevB, B);

  for (int i = 0; i <= 1020 && !Serial.available(); i++) { // Serial available for non blocking function, exit this loop if a Serial data is available
    redVal = calculateVal(stepR, redVal, i);
    grnVal = calculateVal(stepG, grnVal, i);
    bluVal = calculateVal(stepB, bluVal, i);

    analogWrite(redPin, redVal);   // Write current values to LED pins
    analogWrite(grnPin, grnVal);      
    analogWrite(bluPin, bluVal); 
    
    delayMicroseconds(wait); // Pause for 'wait' milliseconds before resuming the loop
  }
  // Update current values for next loop
  prevR = redVal; 
  prevG = grnVal; 
  prevB = bluVal;
  delay(hold); // Pause for optional 'wait' milliseconds before resuming the loop
}



