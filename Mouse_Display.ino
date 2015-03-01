#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include <ps2.h>

//#include "imagens.h"

#define RESOLUCAO_X 48
#define RESOLUCAO_Y 48

 /*
 * Pin 9 - data (Orange)
 * Pin 10 - clk (white)
 */
PS2 mouse(9, 10);

// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)

Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 7, 6);

int X = 24, Y = 24;


/*
 * initialize the mouse. Reset it, and place it into remote
 * mode, so we can get the encoder data on demand. Uses Magic to setup microsoft intelimouse 0x03
 */
void mouse_init()
{
  mouse.write(0xff);  // reset
  mouse.read();  // ack byte
  mouse.read();  // blank */
  mouse.read();  // blank */
  mouse.write(0xf0);  // remote mode
  mouse.read();  // ack
  delayMicroseconds(100);
  
  //Magic Sequence
  mouse.write(0xF3); //Set Sample Rate
  mouse.read(); //ack
  mouse.write(0xC8); // send 200
  mouse.read();
  mouse.write(0x64); // send 100
  mouse.read();
  mouse.write(0x50); //send 80
  mouse.read(); 
  
  
  mouse.write(0xF2);  //Asks for mouse type
  unsigned char mstat = mouse.read(); //ack
  unsigned char mtype = mouse.read();  //type
  //Serial.print(mstat, HEX);
  Serial.print("Mouse type: 0x");
  Serial.print(mtype, HEX);
  Serial.println();
}



void display_init()
{
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  
  display.println("TESTE!");
  display.display();
  delay(200);
  display.clearDisplay();
  display.display();
  display.command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
}


void drawCursor(int dx, int dy)
{
  int i, j;
  for (i = (dx - 3); i < (dx + 3); i++)
    for (j = (dy - 3); j < (dy + 3); j++)
      {
         display.drawPixel(i, j, BLACK);
      }
}
void setup()
{
  
  Serial.begin(9600);
  mouse_init();
  display_init();
  display.clearDisplay();  
  
}
 
void MouseMove(int mx, int my)
{
  if (X >= 0 && X <= 84)
  {
    X += mx;
  }
  if (X < 0) X = 0;
  if (X > 84) X = 84;
 
 if (Y >= 0 && Y <= 48)
  {
    Y += my;
  }
  if (Y < 0) Y = 0;  
  if(Y > 48) Y = 48;  
}

void loop()
{
  char mstat;
  char mx; //dx
  char my; //dy
  char ms; //scr
  

  /* get a reading from the mouse */
  mouse.write(0xeb);  // give me data!
  mouse.read();      // ignore ack
  mstat = (mouse.read() & 0x0F);
  mx = mouse.read();
  my = mouse.read();
  ms = mouse.read();
  
  MouseMove(mx, my);
  
  display.clearDisplay();
  
  drawCursor(X, 48 - Y);
  
  display.display();  
  
  if (mx != 0 || my != 0 || mstat != 8 || ms != 0)
  {
  /* send the data back up */
    Serial.print(mstat, HEX);
    Serial.print("\tmX=");
    Serial.print(mx, DEC);
    Serial.print("\tmY=");
    Serial.print(my, DEC);
    Serial.print("\tX=");
    Serial.print(X, DEC);
    Serial.print("\tY=");
    Serial.print(Y, DEC); 
    Serial.print("\tms=");
    Serial.print(ms, DEC); 
    
    Serial.println();
   
  }
}
