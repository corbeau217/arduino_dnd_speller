/**
 * @file screen_experimentation.ino
 * @author your name (you@domain.com)
 * @version 0.1
 * @date 2023-11-14
 * 
 * @brief UTFT library source: http://www.rinkydinkelectronics.com/library.php?id=51
 * @brief URTOUCH library source: http://www.rinkydinkelectronics.com/library.php?id=92 // doesnt currently have touch
 * @brief 
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// #include <cstdint>
#include <UTFT.h>



#define BUTTON_PIN_UPWARD 4
#define BUTTON_PIN_DOWNER 2
#define BUTTON_PIN_LEFTER 3
#define BUTTON_PIN_RIGHTR 5

#define BUTTON_PIN_SELECT 6
#define BUTTON_PIN_CANCEL 7

#define TFTLCD_PIN_RS 38
#define TFTLCD_PIN_WR 39
#define TFTLCD_PIN_CS 40
#define TFTLCD_PIN_RST 41

#define LOOP_SNOOZE_TIME 10

// screen instance thinger
//  uses pins for the screen
UTFT screenInstance(
  ILI9486,
  TFTLCD_PIN_RS,
  TFTLCD_PIN_WR,
  TFTLCD_PIN_CS,
  TFTLCD_PIN_RST
);

// screen size stuff
static const unsigned SCREEN_WIDTH = 480, SCREEN_HEIGHT = 320;

// mouse positioning
int mouseX = SCREEN_WIDTH/2, mouseY = SCREEN_HEIGHT/2;
int prevMouseX = mouseX, prevMouseY = mouseY;

#define MOUSEMODE_NONE 0
#define MOUSEMODE_DRAW 1
#define MOUSEMODE_CLEAR 2
// mouse mode var
int mouseMode = MOUSEMODE_NONE;
int mouseModeSetter = MOUSEMODE_DRAW;

// Declare which fonts we will be using
extern uint8_t SmallFont[];


void fillGrider(){
  doGrid(&screenInstance,10,10,0,255,65);
  doGrid(&screenInstance,50,50,50,50,200);
  doGrid(&screenInstance,100,100,255,0,0);
}
void setupLCD(){
  // Setup the LCD
  screenInstance.InitLCD();
  screenInstance.setFont(SmallFont);

  // Clear the screen and draw the frame
  screenInstance.clrScr();

  fillGrider();


  screenInstance.setColor(200,200,200);
}

void setupButtons(){
  // button modes
  pinMode(BUTTON_PIN_UPWARD, INPUT_PULLUP);
  pinMode(BUTTON_PIN_DOWNER, INPUT_PULLUP);
  pinMode(BUTTON_PIN_LEFTER, INPUT_PULLUP);
  pinMode(BUTTON_PIN_RIGHTR, INPUT_PULLUP);

  pinMode(BUTTON_PIN_SELECT, INPUT_PULLUP);
  pinMode(BUTTON_PIN_CANCEL, INPUT_PULLUP);
}

void setup()
{
  randomSeed(analogRead(0));
  
  setupLCD();
  setupButtons();
  // idk tbh
  // setupInteraction();
}





void doGrid(UTFT *screenInstance, int xStep, int yStep, unsigned char r, unsigned char g, unsigned char b){
  int x, y;
  int widtherr, heighter;

  if(!screenInstance) return;

  widtherr = SCREEN_WIDTH;
  heighter = SCREEN_HEIGHT;

  // set color
  screenInstance->setColor(r,g,b);

  for(x = xStep-1; x < widtherr; x+=xStep){
    screenInstance->drawLine(x, 0, x, heighter);
  }

  for(y = yStep-1; y < heighter; y+=yStep){
    screenInstance->drawLine(0, y, widtherr, y);
  }
}




void doButtonUpdate(){
  if(digitalRead(BUTTON_PIN_UPWARD)==LOW){ mouseY--; }
  if(digitalRead(BUTTON_PIN_DOWNER)==LOW){ mouseY++; }
  if(digitalRead(BUTTON_PIN_LEFTER)==LOW){ mouseX--; }
  if(digitalRead(BUTTON_PIN_RIGHTR)==LOW){ mouseX++; }


  // BIGTODO: have a state cycle
  if(digitalRead(BUTTON_PIN_SELECT)==LOW){
    mouseMode = mouseModeSetter;
  }
  if(digitalRead(BUTTON_PIN_CANCEL)==LOW){
    // mouseModeSetter = (mouseModeSetter==MOUSEMODE_CLEAR)? MOUSEMODE_DRAW : (mouseModeSetter==MOUSEMODE_DRAW)? MOUSEMODE_NONE : MOUSEMODE_CLEAR;
    mouseModeSetter = (mouseModeSetter==MOUSEMODE_NONE)? MOUSEMODE_DRAW : MOUSEMODE_NONE;
  }
  // if(digitalRead(BUTTON_PIN_SELECT)==LOW){ screenInstance.clrScr(); }else{ fillGrider(); }
  // if(digitalRead(BUTTON_PIN_CANCEL)==LOW){ screenInstance.clrScr(); }else{ fillGrider(); }
}

void doDrawingUpdate(){


  if(mouseMode==MOUSEMODE_DRAW){
    screenInstance.setColor(200,200,200);
    screenInstance.drawPixel(mouseX,mouseY);
    // screenInstance.drawLine(mouseX,mouseY,prevMouseX,prevMouseY);
  }
  

  // update the previous positionss
  prevMouseX = mouseX;
  prevMouseY = mouseY;
}

void loop()
{

  doButtonUpdate();

  // if(mouseMode==MOUSEMODE_DRAW){
    doDrawingUpdate();
  // }

  // int xStep = 10;
  // int yStep = 10;
  // int widtherr = screenInstance.getDisplayXSize();
  // int heighter = screenInstance.getDisplayYSize();
  // // bright green
  // screenInstance.setColor(0,255,65);
  // for(int x = xStep-1; x < widtherr; x+=xStep){
  //   screenInstance.drawLine(x, 0, x, heighter);
  // }

  // // dark green
  // screenInstance.setColor(0,143,17);
  // for(int y = yStep-1; y < heighter; y+=yStep){
  //   screenInstance.drawLine(0, y, widtherr, y);
  // }

  // screen is 480x320





  // delay(2000);


  // scammed
  // doDemo();


  // rn bc why not
  delay(LOOP_SNOOZE_TIME);
}

void doDemo(){
  int buf[318];
  int x, x2;
  int y, y2;
  int r;


// Clear the screen and draw the frame
  screenInstance.clrScr();

  screenInstance.setColor(255, 0, 0);
  screenInstance.fillRect(0, 0, 479, 13);
  screenInstance.setColor(64, 64, 64);
  screenInstance.fillRect(0, 306, 479, 319);
  screenInstance.setColor(255, 255, 255);
  screenInstance.setBackColor(255, 0, 0);
  screenInstance.print("* it's a thiinger thinger *", CENTER, 1);
  screenInstance.setBackColor(64, 64, 64);
  screenInstance.setColor(255,255,0);
  screenInstance.print("why would we have a url here", CENTER, 307);

  screenInstance.setColor(0, 0, 255);
  screenInstance.drawRect(0, 14, 479, 305);

// Draw crosshairs
  screenInstance.setColor(0, 0, 255);
  screenInstance.setBackColor(0, 0, 0);
  screenInstance.drawLine(239, 15, 239, 304);
  screenInstance.drawLine(1, 159, 478, 159);
  for (int i=9; i<470; i+=10)
    screenInstance.drawLine(i, 157, i, 161);
  for (int i=19; i<220; i+=10)
    screenInstance.drawLine(237, i, 241, i);

// Draw sin-, cos- and tan-lines  
  screenInstance.setColor(0,255,255);
  screenInstance.print("Sin", 5, 15);
  for (int i=1; i<478; i++)
  {
    screenInstance.drawPixel(i,159+(sin(((i*1.13)*3.14)/180)*95));
  }
  
  screenInstance.setColor(255,0,0);
  screenInstance.print("Cos", 5, 27);
  for (int i=1; i<478; i++)
  {
    screenInstance.drawPixel(i,159+(cos(((i*1.13)*3.14)/180)*95));
  }

  screenInstance.setColor(255,255,0);
  screenInstance.print("Tan", 5, 39);
  for (int i=1; i<478; i++)
  {
    screenInstance.drawPixel(i,159+(tan(((i*1.13)*3.14)/180)));
  }

  delay(2000);

  screenInstance.setColor(0,0,0);
  screenInstance.fillRect(1,15,478,304);
  screenInstance.setColor(0, 0, 255);
  screenInstance.setBackColor(0, 0, 0);
  screenInstance.drawLine(239, 15, 239, 304);
  screenInstance.drawLine(1, 159, 478, 159);

// Draw a moving sinewave
  x=1;
  for (int i=1; i<(478*15); i++) 
  {
    x++;
    if (x==479)
      x=1;
    if (i>479)
    {
      if ((x==239)||(buf[x-1]==159))
        screenInstance.setColor(0,0,255);
      else
        screenInstance.setColor(0,0,0);
      screenInstance.drawPixel(x,buf[x-1]);
    }
    screenInstance.setColor(0,255,255);
    y=159+(sin(((i*0.7)*3.14)/180)*(90-(i / 100)));
    screenInstance.drawPixel(x,y);
    buf[x-1]=y;
  }

  delay(2000);
  
  screenInstance.setColor(0,0,0);
  screenInstance.fillRect(1,15,478,304);

// Draw some filled rectangles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        screenInstance.setColor(255,0,255);
        break;
      case 2:
        screenInstance.setColor(255,0,0);
        break;
      case 3:
        screenInstance.setColor(0,255,0);
        break;
      case 4:
        screenInstance.setColor(0,0,255);
        break;
      case 5:
        screenInstance.setColor(255,255,0);
        break;
    }
    screenInstance.fillRect(150+(i*20), 70+(i*20), 210+(i*20), 130+(i*20));
  }

  delay(2000);
  
  screenInstance.setColor(0,0,0);
  screenInstance.fillRect(1,15,478,304);

// Draw some filled, rounded rectangles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        screenInstance.setColor(255,0,255);
        break;
      case 2:
        screenInstance.setColor(255,0,0);
        break;
      case 3:
        screenInstance.setColor(0,255,0);
        break;
      case 4:
        screenInstance.setColor(0,0,255);
        break;
      case 5:
        screenInstance.setColor(255,255,0);
        break;
    }
    screenInstance.fillRoundRect(270-(i*20), 70+(i*20), 330-(i*20), 130+(i*20));
  }
  
  delay(2000);
  
  screenInstance.setColor(0,0,0);
  screenInstance.fillRect(1,15,478,304);

// Draw some filled circles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        screenInstance.setColor(255,0,255);
        break;
      case 2:
        screenInstance.setColor(255,0,0);
        break;
      case 3:
        screenInstance.setColor(0,255,0);
        break;
      case 4:
        screenInstance.setColor(0,0,255);
        break;
      case 5:
        screenInstance.setColor(255,255,0);
        break;
    }
    screenInstance.fillCircle(180+(i*20),100+(i*20), 30);
  }
  
  delay(2000);
  
  screenInstance.setColor(0,0,0);
  screenInstance.fillRect(1,15,478,304);

// Draw some lines in a pattern
  screenInstance.setColor (255,0,0);
  for (int i=15; i<304; i+=5)
  {
    screenInstance.drawLine(1, i, (i*1.6)-10, 304);
  }
  screenInstance.setColor (255,0,0);
  for (int i=304; i>15; i-=5)
  {
    screenInstance.drawLine(478, i, (i*1.6)-11, 15);
  }
  screenInstance.setColor (0,255,255);
  for (int i=304; i>15; i-=5)
  {
    screenInstance.drawLine(1, i, 491-(i*1.6), 15);
  }
  screenInstance.setColor (0,255,255);
  for (int i=15; i<304; i+=5)
  {
    screenInstance.drawLine(478, i, 490-(i*1.6), 304);
  }
  
  delay(2000);
  
  screenInstance.setColor(0,0,0);
  screenInstance.fillRect(1,15,478,304);

// Draw some random circles
  for (int i=0; i<100; i++)
  {
    screenInstance.setColor(random(255), random(255), random(255));
    x=32+random(416);
    y=45+random(226);
    r=random(30);
    screenInstance.drawCircle(x, y, r);
  }

  delay(2000);
  
  screenInstance.setColor(0,0,0);
  screenInstance.fillRect(1,15,478,304);

// Draw some random rectangles
  for (int i=0; i<100; i++)
  {
    screenInstance.setColor(random(255), random(255), random(255));
    x=2+random(476);
    y=16+random(289);
    x2=2+random(476);
    y2=16+random(289);
    screenInstance.drawRect(x, y, x2, y2);
  }

  delay(2000);
  
  screenInstance.setColor(0,0,0);
  screenInstance.fillRect(1,15,478,304);

// Draw some random rounded rectangles
  for (int i=0; i<100; i++)
  {
    screenInstance.setColor(random(255), random(255), random(255));
    x=2+random(476);
    y=16+random(289);
    x2=2+random(476);
    y2=16+random(289);
    screenInstance.drawRoundRect(x, y, x2, y2);
  }

  delay(2000);
  
  screenInstance.setColor(0,0,0);
  screenInstance.fillRect(1,15,478,304);

  for (int i=0; i<100; i++)
  {
    screenInstance.setColor(random(255), random(255), random(255));
    x=2+random(476);
    y=16+random(289);
    x2=2+random(476);
    y2=16+random(289);
    screenInstance.drawLine(x, y, x2, y2);
  }

  delay(2000);
  
  screenInstance.setColor(0,0,0);
  screenInstance.fillRect(1,15,478,304);

  for (int i=0; i<10000; i++)
  {
    screenInstance.setColor(random(255), random(255), random(255));
    screenInstance.drawPixel(2+random(476), 16+random(289));
  }

  delay(2000);

  screenInstance.fillScr(0, 0, 255);
  screenInstance.setColor(255, 0, 0);
  screenInstance.fillRoundRect(160, 70, 319, 169); 
  
  screenInstance.setColor(255, 255, 255);
  screenInstance.setBackColor(255, 0, 0);
  screenInstance.print("That's it!", CENTER, 93);
  screenInstance.print("Restarting in a", CENTER, 119);
  screenInstance.print("few seconds...", CENTER, 132);
  
  screenInstance.setColor(0, 255, 0);
  screenInstance.setBackColor(0, 0, 255);
  screenInstance.print("Runtime: (msecs)", CENTER, 290);
  screenInstance.printNumI(millis(), CENTER, 305);
  
  delay (10000);
}