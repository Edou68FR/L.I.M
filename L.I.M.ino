// Output
int redPin = 3;   // Red LED,   connected to digital pin 3
int grnPin = 5;  // Green LED, connected to digital pin 5
int bluPin = 6;  // Blue LED,  connected to digital pin 6
int photor = A1; // Photo résistance, connected to analog pin A1

// Color arrays

int black[3]  = { 0, 0, 0 };
int white[3]  = { 100, 100, 100 };
int red[3]    = { 100, 0, 0 };
int green[3]  = { 0, 100, 0 };
int blue[3]   = { 0, 0, 100 };
int yellow[3] = { 40, 95, 0 };
int dimWhite[3] = { 30, 30, 30 };
// etc.

// Set initial color
int redVal = black[0];
int grnVal = black[1]; 
int bluVal = black[2];

int wait = 100;      // 10µs internal crossFade delay; increase for slower fades
int hold = 1;       // Optional hold when a color is complete, before the next crossFade
int repeat = 0;     // How many times should we loop before stopping? (0 for no stop)
int j = 0;          // Loop counter for repeat
int mode = 1;       // actual mode ( 1 = LightMode, 2 = animation, 3 = audio, 4 = manualMode ).
// Initialize color variables
int prevR = redVal;
int prevG = grnVal;
int prevB = bluVal;

// Set up the LED outputs
void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(grnPin, OUTPUT);   
  pinMode(bluPin, OUTPUT); 
  pinMode(photor, INPUT);
  Serial.begin(9600);
}

// Main program: list the order of crossfades
void loop()
{
  int bmode = digitalRead(buttonmode);
  if(bmode == HIGH)
  {
    while( bmode == HIGH){}
    mode = mode++;
  }
}

void serialEvent(){
    if(Serial.available()>0){
       var = Serial.read();
       switch (var) {
          case label1:
          break;
          case label2:
          break;
          default:
          break;
        }
    }
}

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

  for (int i = 0; i <= 1020; i++) {
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


