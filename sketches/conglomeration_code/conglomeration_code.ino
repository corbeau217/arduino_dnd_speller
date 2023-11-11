/* I2C LCD with Arduino example code. More info: https://www.makerguides.com */

// Include the libraries:
// LiquidCrystal_I2C.h: https://github.com/johnrickman/LiquidCrystal_I2C
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

// BIGTODO ADD IN THE FIRST ROW DISPLAY BLOCK COLUMN IDX PRE-PROCESSOR VARS
#define COLS 16
#define ROWS 2

#define FIRSTROW_IDX 0
#define SECONROW_IDX 1

#define BACKLIGHT_BYTE 0xff
#define USING_CODE_CHUNK false

/*
Usually, they are either black, red, and white, or they are brown, red, and orange/yellow:
  brown or black = ground (GND, battery negative terminal)
  red = servo power (Vservo, battery positive terminal)
  orange, yellow, white, or blue = servo control signal line.
*/

// [ ] get the documentation for the motor driver stuff
// [ ] reorganise pins as per pin diagram
// BIGTODO PINS ON THE PIN DIAGRAM GIVEN
#define MOTOR_LIT_PIN 2 // light pin
#define MOTOR_LIT_PINMODE INPUT_PULLUP // light pin

#define MOTOR_DIR_PIN 4
#define MOTOR_DIR_PINMODE OUTPUT
#define MOTOR_DIR_VAL HIGH

#define MOTOR_PWM_PIN 5
#define MOTOR_PWM_PINMODE OUTPUT
#define MOTOR_ANALOG_VAL 0xff


#define INFRARED_THRESHOLD 10

#define INFRARED_ONE_PIN A0
#define INFRARED_ONE_PINMODE INPUT
#define INFRARED_TWO_PIN A1
#define INFRARED_TWO_PINMODE INPUT

// placeholder val for the pin iinput 
#define FAKESENSOR_IINPUT_PIN 8
#define FAKESENSOR_IINPUT_PINMODE INPUT_PULLUP
#define FAKESENSOR_OUTPUT_PIN 9
#define FAKESENSOR_OUTPUT_PINMODE INPUT_PULLUP

#define FAKESENSOR_TEST_VAL LOW

#define FAKEPIXEL_PIN 12
#define FAKEPIXEL_PINMODE OUTPUT
#define FAKEPIXEL_VAL_ON HIGH
#define FAKEPIXEL_VAL_OFF LOW

#define SENSOR_IINPUT_TYPE 0
#define SENSOR_OUTPUT_TYPE 1

#define USING_STRING_FIRSTROW false
#define USING_STRING_SECONROW false

#define EMPTY_BLOCK_CHAR ' '
#define EMPTY_BLOCK_STR " "

#define SCROLL_FRAME_SNOOZE 0x7

// nth frame to update the display, must be at least 1
#define FRAMEUPDATE_DISPLAY_COUNTER 0x1

// #define UPDATE_FACTOR_TEXTSCROLL 0x3
#define UPDATE_FACTOR_CLOCK_BLINK 0xa


// BIGTODO CAN USE WIRES TO BREADBOARD FOR SWITCH ON OFF THE BACKLIIGHT LED
// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, COLS, ROWS); // Change to (0x27,20,4) for 20x4 LCD.

static String hexRow = "0123456789abcdef";

#define FIRST_ROW_FRAME "[     |     |  ]"
#define TEAM_NAME "T3C1-T3M1-T3S8 0123456789abcdefghijklmnopqrstuvwxyz"

// BIGTODO NEED TO MOVE OVER TO USING AN ARRAY FOR EASY ADDRESSIING
// line string stores printed to display
static String firstRow = FIRST_ROW_FRAME;
static String seconRow = TEAM_NAME;

/* arr initialisation notes:

  int myInts[6];
  int myPins[] = {2, 4, 8, 3, 6};
  int mySensVals[5] = {2, 4, -8, 3, 2};
  char message[6] = "hello";
*/

static unsigned loopCounter = 0;

static unsigned marbleCount_currInternal = 0;
static unsigned marbleCount_totalServiced = 0;

// // used for what each char is on first row
// static char firstRowBlockChars[COLS];
// // used for what each char is on second row
// static char seconRowBlockChars[sizeof(seconRow)];

// static int firstRowWriteFrom = 0;
// static int seconRowWriteFrom = 0;

void setup() {
  // Initiate the LCD:
  lcd.init();
  lcd.setBacklight(BACKLIGHT_BYTE);
  // lcd.backlight();

  // pixel thingy
  pinMode(FAKEPIXEL_PIN, FAKEPIXEL_PINMODE);
  // [ ] figure out pwm pin handling
  pinMode(MOTOR_LIT_PIN, MOTOR_LIT_PINMODE);
  pinMode(MOTOR_DIR_PIN, MOTOR_DIR_PINMODE);
  pinMode(MOTOR_PWM_PIN, MOTOR_PWM_PINMODE);
  // [ ] figure out sensor pin handling
  // set for internal resistor
  pinMode(FAKESENSOR_IINPUT_PIN, FAKESENSOR_IINPUT_PINMODE);
  pinMode(FAKESENSOR_OUTPUT_PIN, FAKESENSOR_OUTPUT_PINMODE);

  // infrared sensor code
  pinMode(INFRARED_ONE_PIN, INFRARED_ONE_PINMODE);
  pinMode(INFRARED_TWO_PIN, INFRARED_TWO_PINMODE);
  
  setupRowStrings();
}

// [ ] reorganise functions as per UML / necessity

// WARN: may give idx out of boundss if needs null char
void emptyScreen(){
  // loop all and empty
  for (int i = 0; i < sizeof(firstRow)-1; i++)
  {
    firstRow.setCharAt(i,EMPTY_BLOCK_CHAR);
  }

  // same for second
  for (int i = 0; i < sizeof(seconRow)-1; i++)
  {
    seconRow.setCharAt(i,EMPTY_BLOCK_CHAR);
  }
}

void fillRows(){
  // // loop all and empty
  // for (int i = 0; i < sizeof(firstRowBlockChars); i++)
  // {
  //   firstRowBlockChars[i] = firstRow.charAt(i);
  // }
  // // loop all and empty
  // for (int i = 0; i < sizeof(seconRowBlockChars); i++)
  // {
  //   seconRowBlockChars[i] = firstRow.charAt(i);
  // }
  firstRow = FIRST_ROW_FRAME;
  seconRow = TEAM_NAME;
}

// setup the default things in our char arrays
void setupRowStrings(){
  // fill with the strings?
  fillRows();
}

// BIGTODO: look into making this a pre-processor macro
// for snoozing frame timeoutss
unsigned testFrameTimeout(unsigned frameTimeout){
  return !(loopCounter%frameTimeout);
}

// BIGTODO: change this to have the different pin inputs handle
//          also iinclude that we need to map what pins have what values as standard
//          so HIIGH or LOW for theiir normal thing
int receivedSensorInput(int sensorValue){
  if(sensorValue == FAKESENSOR_TEST_VAL){
    return 1; // 1 as true
  }
  return 0; // 0 as false
}

// advance by one character in the line
void scrollText(int lineIdx){
  // ordered so compiler will optimise
  switch (lineIdx) {
    case FIRSTROW_IDX:
      firstRow = firstRow.substring(1)+firstRow.substring(0,1);
      break;
    case SECONROW_IDX:
      seconRow = seconRow.substring(1)+seconRow.substring(0,1);
      break;
    default:
      seconRow = seconRow.substring(1)+seconRow.substring(0,1);
      break;
  }
}

// only grabs 0-15
//  WARN: gives an index out of bounds if more than 15
char getHexChar(unsigned val){
  return hexRow.charAt(val);
}

// need to decide whether we write the number with function or not?
//  WARN gives an index out of bounds if the colIdx is >= length of row string
void addNumberToBuffer(int colIdx, int rowIdx, unsigned numberAddingToBuffer){
  // based on row
  //  ordered so compiler will optimise with pointer math
  switch (rowIdx) {
    // for the first row bracketables
    case FIRSTROW_IDX:
      // first hex digit
      firstRow.setCharAt(colIdx,getHexChar((0xf0&numberAddingToBuffer)>>4));
      // second hex digit
      firstRow.setCharAt(colIdx+1,getHexChar(0xf&numberAddingToBuffer));
      break;
    // incase we want to change the scrolling text
    case SECONROW_IDX:
    default:
      // first hex digit
      seconRow.setCharAt(colIdx,getHexChar((0xf0&numberAddingToBuffer)>>4));
      // second hex digit
      seconRow.setCharAt(colIdx+1,getHexChar(0xf&numberAddingToBuffer));
      break;
  }
}


void updateCounterDisplays(){
  // check that it's the current frame that we want it on
  if(testFrameTimeout(FRAMEUPDATE_DISPLAY_COUNTER)){
    // hand off number adding
    addNumberToBuffer(COLS-3,FIRSTROW_IDX,marbleCount_currInternal);
    addNumberToBuffer(COLS-6,FIRSTROW_IDX,marbleCount_totalServiced);
    
  }
}

void updateDisplay(){
  // Set the cursor on the first column and first row.
  lcd.setCursor(0, FIRSTROW_IDX);


  updateCounterDisplays();
  

  lcd.print(firstRow); // Print the striing way


  // Set the cursor on the first column and second row.
  lcd.setCursor(0, SECONROW_IDX);
  
  lcd.print(seconRow.substring(0,COLS)); // Print the striing way
}

// [ ] get the sensor documentation
// [ ] get code for the sensor stuff

// this handles the sensor
void handleFakeSensorInput(int inputType){
  // ordered so compiler will be happy and use
  //  pointer math
  switch (inputType) {
  // check input type
  case SENSOR_IINPUT_TYPE:
    marbleCount_currInternal++;
    break;
  // otherwise assume out
  case SENSOR_OUTPUT_TYPE:
  default:
    marbleCount_currInternal--;
    marbleCount_totalServiced++;
    break;
  }
}
// this handles the sensor
void handleInfraredSensorInput(){
  int sensorValueOne = analogRead(INFRARED_ONE_PIN);
  int sensorValueTwo = analogRead(INFRARED_TWO_PIN);
  if(sensorValueOne <= INFRARED_THRESHOLD){
    digitalWrite(FAKEPIXEL_PIN,FAKEPIXEL_VAL_ON);
  }else {
    digitalWrite(FAKEPIXEL_PIN,FAKEPIXEL_VAL_OFF);
  }
}

void motorUpdate(){
  // write to PWM pin?
  analogWrite(MOTOR_PWM_PIN, (marbleCount_currInternal&0xff));
  // DIR
  digitalWrite(MOTOR_DIR_PIN, MOTOR_DIR_VAL);
}



// main loop
void loop() {
  // loop ticker
  loopCounter++;
  // for the button1 input
  int iinputSensorVal = digitalRead(FAKESENSOR_IINPUT_PIN);
  // for the button2 input
  int outputSensorVal = digitalRead(FAKESENSOR_OUTPUT_PIN);


  // if(marbleCount_currInternal!=0){
  //   digitalWrite(FAKEPIXEL_PIN,FAKEPIXEL_VAL_ON);
  // } else{
  //   digitalWrite(FAKEPIXEL_PIN,FAKEPIXEL_VAL_OFF);
  // }

  // HANDOF FOR MOTOR
  motorUpdate();

  if(testFrameTimeout(SCROLL_FRAME_SNOOZE)){
    scrollText(SECONROW_IDX);
  }

  handleInfraredSensorInput();

  // reads input types input
  if(receivedSensorInput(iinputSensorVal)){
    handleFakeSensorInput(SENSOR_IINPUT_TYPE);
  }
  // reads output typed input
  if(receivedSensorInput(outputSensorVal)){
    handleFakeSensorInput(SENSOR_OUTPUT_TYPE);
  }

  // hand off for updating displays
  updateDisplay();
}