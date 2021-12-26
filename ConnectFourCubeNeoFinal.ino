//*******************************************************************************
// ConnectFourCubeNeoFinal.ino
// Progressive Version of the Cube
// Connect Four RGB Cube Game
// If encoder has threads on shaft, use ESPRotarySet.h
// by RCI
//
// Edits:
//      10-11-2016
//         Added Player color choice check and flash yellow when same color selected.
//      3-18-2017
//         Cleaned up variable types,  reversed encoder direction - RCI.
//      12-24-2019
//         Fixed counter loop issue while displaying random rain - RCI.
//      11-10-2020
//         Added RefreshCube() call to FlashRow Routine.  Not sure why flash stopped
//         working - RCI.
//      12-24-2020 Changed EncoderHalfStep.h to ErriezRotaryHalfStep.h
//      1-16-2021 Redesigned cube for WS2812 Neopixels  
//      8-9-2021 Changed Encoder Driver to ESPRotarySet.h for threaded shaft type
//                ESPRotarySet added setPosition(int pos)
//*******************************************************************************
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
//#include <ErriezRotaryHalfStep.h> 
#include <ESPRotarySet.h>
//***** Variables **************************
unsigned char flag;
unsigned char pos, next_slot, current_pos, last_pos;
unsigned char result;
signed int EncVal;
signed int countLast = 0;
unsigned char ColorChoice;
unsigned char next_player;
unsigned int counter;
unsigned int buttoncount;
unsigned char player;
unsigned char win[5];
unsigned char PlayerColor[3][3];
unsigned char MemoryColors[66][4]; 

#define PIN 4            // D2
#define BUTTON 14        // D5
#define ROTARY_PIN1  12  // D7
#define ROTARY_PIN2  13  // D6
#define CLICKS_PER_STEP   4   // this number depends on your rotary encoder 

#define MATRIX_WIDTH 4
#define MATRIX_HEIGHT 4
#define STARTING_PIXEL 1
#define MIN_ENC_VAL 1
#define MAX_ENC_VAL 64

#define LED_COUNT 64
#define num_of_pixels 64  //(sacrificial LED)

#define red       255, 0, 0
#define green     0, 255, 0
#define blue      0, 0, 255
#define black     0 ,0, 0
#define grey      1, 1, 1
#define dimwhite  1, 1, 1
#define midwhite  127, 127, 127
#define white     255, 255, 255
#define coolwhite 255, 255, 255
#define yellow    255, 138, 0
#define cyan      0, 255, 255
#define magenta   255, 0, 255
#define gold      255, 90, 0
#define orange    220, 40, 0
#define olive     100, 220, 47
#define turq      35, 255, 30
#define lime      15, 70, 10
#define skyblue   65, 176, 255
#define purple    70, 0, 120

//Matrix to Cube mapping 
unsigned char ZX[5][4]= 
{ 
 {0,0,0,0},
 {4,4,4,4},
 {8,8,8,8},
 {12,12,12,12}
};

//****************************************************                           
const unsigned char TempPos[65][4] = {
                            {0, 0, 0, 0},  // dummy
                            {0, 0, 0, 0},  // 1
                            {0, 1, 0, 0},  // 2
                            {0, 2, 0, 0},  // 3
                            {0, 3, 0, 0},  // 4
                            {0, 0, 1, 0},  // 5
                            {0, 1, 1, 0},  // 6
                            {0, 2, 1, 0},  // 7
                            {0, 3, 1, 0},  // 8
                            {0, 0, 2, 0},  // 9
                            {0, 1, 2, 0},  // 10
                            {0, 2, 2, 0},  // 11
                            {0, 3, 2, 0},  // 12
                            {0, 0, 3, 0},  // 13
                            {0, 1, 3, 0},  // 14
                            {0, 2, 3, 0},  // 15
                            {0, 3, 3, 0},  // 16 
                            {1, 0, 0, 0},  // 1
                            {1, 1, 0, 0},  // 2
                            {1, 2, 0, 0},  // 3
                            {1, 3, 0, 0},  // 4
                            {1, 0, 1, 0},  // 5
                            {1, 1, 1, 0},  // 6
                            {1, 2, 1, 0},  // 7
                            {1, 3, 1, 0},  // 8
                            {1, 0, 2, 0},  // 9
                            {1, 1, 2, 0},  // 10
                            {1, 2, 2, 0},  // 11
                            {1, 3, 2, 0},  // 12
                            {1, 0, 3, 0},  // 13
                            {1, 1, 3, 0},  // 14
                            {1, 2, 3, 0},  // 15
                            {1, 3, 3, 0},  // 16 
                            {2, 0, 0, 0},  // 1
                            {2, 1, 0, 0},  // 2
                            {2, 2, 0, 0},  // 3
                            {2, 3, 0, 0},  // 4
                            {2, 0, 1, 0},  // 5
                            {2, 1, 1, 0},  // 6
                            {2, 2, 1, 0},  // 7
                            {2, 3, 1, 0},  // 8
                            {2, 0, 2, 0},  // 9
                            {2, 1, 2, 0},  // 10
                            {2, 2, 2, 0},  // 11
                            {2, 3, 2, 0},  // 12
                            {2, 0, 3, 0},  // 13
                            {2, 1, 3, 0},  // 14
                            {2, 2, 3, 0},  // 15
                            {2, 3, 3, 0},  // 16 
                            {3, 0, 0, 0},  // 1
                            {3, 1, 0, 0},  // 2
                            {3, 2, 0, 0},  // 3
                            {3, 3, 0, 0},  // 4
                            {3, 0, 1, 0},  // 5
                            {3, 1, 1, 0},  // 6
                            {3, 2, 1, 0},  // 7
                            {3, 3, 1, 0},  // 8
                            {3, 0, 2, 0},  // 9
                            {3, 1, 2, 0},  // 10
                            {3, 2, 2, 0},  // 11
                            {3, 3, 2, 0},  // 12
                            {3, 0, 3, 0},  // 13
                            {3, 1, 3, 0},  // 14
                            {3, 2, 3, 0},  // 15
                            {3, 3, 3, 0},  // 16                                 
                            }; 

//****************************************************  
unsigned char LEDPos[65][4] = {
                            {0, 0, 0, 0},  // dummy
                            {0, 0, 0, 0},  // 1
                            {0, 1, 0, 0},  // 2
                            {0, 2, 0, 0},  // 3
                            {0, 3, 0, 0},  // 4
                            {0, 0, 1, 0},  // 5
                            {0, 1, 1, 0},  // 6
                            {0, 2, 1, 0},  // 7
                            {0, 3, 1, 0},  // 8
                            {0, 0, 2, 0},  // 9
                            {0, 1, 2, 0},  // 10
                            {0, 2, 2, 0},  // 11
                            {0, 3, 2, 0},  // 12
                            {0, 0, 3, 0},  // 13
                            {0, 1, 3, 0},  // 14
                            {0, 2, 3, 0},  // 15
                            {0, 3, 3, 0},  // 16 
                            {1, 0, 0, 0},  // 1
                            {1, 1, 0, 0},  // 2
                            {1, 2, 0, 0},  // 3
                            {1, 3, 0, 0},  // 4
                            {1, 0, 1, 0},  // 5
                            {1, 1, 1, 0},  // 6
                            {1, 2, 1, 0},  // 7
                            {1, 3, 1, 0},  // 8
                            {1, 0, 2, 0},  // 9
                            {1, 1, 2, 0},  // 10
                            {1, 2, 2, 0},  // 11
                            {1, 3, 2, 0},  // 12
                            {1, 0, 3, 0},  // 13
                            {1, 1, 3, 0},  // 14
                            {1, 2, 3, 0},  // 15
                            {1, 3, 3, 0},  // 16 
                            {2, 0, 0, 0},  // 1
                            {2, 1, 0, 0},  // 2
                            {2, 2, 0, 0},  // 3
                            {2, 3, 0, 0},  // 4
                            {2, 0, 1, 0},  // 5
                            {2, 1, 1, 0},  // 6
                            {2, 2, 1, 0},  // 7
                            {2, 3, 1, 0},  // 8
                            {2, 0, 2, 0},  // 9
                            {2, 1, 2, 0},  // 10
                            {2, 2, 2, 0},  // 11
                            {2, 3, 2, 0},  // 12
                            {2, 0, 3, 0},  // 13
                            {2, 1, 3, 0},  // 14
                            {2, 2, 3, 0},  // 15
                            {2, 3, 3, 0},  // 16 
                            {3, 0, 0, 0},  // 1
                            {3, 1, 0, 0},  // 2
                            {3, 2, 0, 0},  // 3
                            {3, 3, 0, 0},  // 4
                            {3, 0, 1, 0},  // 5
                            {3, 1, 1, 0},  // 6
                            {3, 2, 1, 0},  // 7
                            {3, 3, 1, 0},  // 8
                            {3, 0, 2, 0},  // 9
                            {3, 1, 2, 0},  // 10
                            {3, 2, 2, 0},  // 11
                            {3, 3, 2, 0},  // 12
                            {3, 0, 3, 0},  // 13
                            {3, 1, 3, 0},  // 14
                            {3, 2, 3, 0},  // 15
                            {3, 3, 3, 0},  // 16                                   
                            };
 
//****************************************************  
unsigned char Compare[77][4] = {
                            {0, 0, 0, 0},     // 0                           
                            {1, 18, 35, 52},  // 2
                            {5, 22, 39, 56},  // 3
                            {9, 26, 43, 60},  // 5 
                            {13, 30, 47, 64}, // 7
                            {4, 19, 34, 49},  // 4
                            {8, 23, 38, 53},  // 4
                            {12, 27, 42, 57}, // 6
                            {16, 31, 46, 61}, // 8                                                      
                            {1, 21, 41, 61},  // 0
                            {2, 22, 42, 62},  // 9
                            {3, 23, 43, 63},  // 10
                            {4, 24, 44, 64},  // 11
                            {16, 28, 40, 52}, // 12
                            {15, 27, 39, 51}, // 13
                            {14, 26, 38, 50}, // 14
                            {13, 25, 37, 49}, // 15
                            {1, 6, 11, 16},   // 16 
                            {4, 7, 10, 13},   // 1
                            {17, 22, 27, 32}, // 2
                            {20, 23, 26, 29}, // 3
                            {33, 38, 43, 48}, // 4
                            {36, 39, 42, 45}, // 5
                            {49, 54, 59, 64}, // 6
                            {52, 55, 58, 61}, // 7
                            {1, 22, 43, 64},  // 8
                            {16, 27, 38, 49}, // 9
                            {4, 23, 42, 61},  // 10
                            {13, 26, 39, 52}, // 11
                            {1, 2, 3, 4},     // 12
                            {5, 6, 7, 8},     // 13
                            {9, 10, 11, 12},  // 14
                            {13, 14, 15, 16}, // 15
                            {17, 18, 19, 20}, // 16 
                            {21, 22, 23, 24}, // 1
                            {25, 26, 27, 28}, // 2
                            {29, 30, 31, 32}, // 3
                            {33, 34, 35, 36}, // 4
                            {37, 38, 39, 40}, // 5
                            {41, 42, 43, 44}, // 6
                            {45, 46, 47, 48}, // 7
                            {49, 50, 51, 52}, // 8
                            {53, 54, 55, 56}, // 9
                            {57, 58, 59, 60}, // 10
                            {61, 62, 63, 64}, // 11
                            {4, 8, 12, 16},   // 12
                            {3, 7, 11, 15},   // 13
                            {2, 6, 10, 14},   // 14
                            {1, 5, 9, 13},
                            {20, 24, 28, 32}, // 15
                            {19, 23, 27, 31}, // 16 
                            {18, 22, 26, 30}, // 1
                            {17, 21, 25, 29}, // 2
                            {36, 40, 44, 48}, // 3
                            {35, 39, 43, 47}, // 4
                            {34, 38, 42, 46}, // 5
                            {33, 37, 41, 45}, // 6
                            {52, 56, 60, 64}, // 7
                            {51, 55, 59, 63}, // 8
                            {50, 54, 58, 62}, // 9
                            {49, 53, 57, 61}, // 10
                            {1, 17, 33, 49},  // 11
                            {2, 18, 34, 50},  // 12
                            {3, 19, 35, 51},  // 13
                            {4, 20, 36, 52},  // 64
                            {5, 21, 37, 53},
                            {6, 22, 38, 54},
                            {7, 23, 39, 55},
                            {8, 24, 40, 56},
                            {9, 25, 41, 57},
                            {10, 26, 42, 58},
                            {11, 27, 43, 59},
                            {12, 28, 44, 60},
                            {13, 29, 45, 61},
                            {14, 30, 46, 62},
                            {15, 31, 47, 63},
                            {16, 32, 48, 64},                      
                            };                                                        
unsigned char colors[10][3] = {
                            {255, 0, 0},     // red
                            {0, 0, 255},     // blue
                            {0, 255, 0},     // green                           
                            {255, 255, 255}, // white
                            {255, 255, 0},   // yellow
                            {255, 0, 255},   // magenta
                            {0, 255, 255},   // cyan                      
                            {255, 30, 0},    // orange 
                            {255, 100, 0},   // gold
                            {14, 255, 10},   // lime                                                 
                            }; 

//RotaryHalfStep rotary(ROTARY_PIN1, ROTARY_PIN2);
ESPRotary r = ESPRotary(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP);

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, 4, 1, PIN,
                            NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS + NEO_TILE_PROGRESSIVE +
                            NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
                            NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
                            NEO_RGB + NEO_KHZ800);
/*
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, 4, 1, PIN,
                            NEO_TILE_TOP + NEO_TILE_LEFT + NEO_TILE_ROWS + NEO_TILE_ZIGZAG +
                            NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
                            NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                            NEO_RGB + NEO_KHZ800);                            
*/
Adafruit_NeoPixel strip = Adafruit_NeoPixel(num_of_pixels, PIN, NEO_RGB + NEO_KHZ800);
//*********************************************************************************
//#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
//ICACHE_RAM_ATTR
//#endif
//*********************************************************************************
void rotate(ESPRotary& r) 
{

    EncVal = r.getPosition();
        // Assign countLast value when EncVal is changed
        // Limit count to a minimum and maximum value
        if (EncVal >= MAX_ENC_VAL) 
           {
           EncVal = MAX_ENC_VAL;  
           //r.setPosition(MAX_ENC_VAL);         
           }
        if (EncVal <= MIN_ENC_VAL) 
           {
           EncVal = MIN_ENC_VAL;
           //r.setPosition(MIN_ENC_VAL);
           }    
    if (countLast != EncVal) 
        {
        countLast = EncVal;
        }
       
}
//************************************************************************************************
// delayms (loops) Milliseconds if using ESPAlexa
void delayms(int loops) 
{
unsigned long time_now = 0;
    time_now = millis();
    
    while(millis() < time_now + loops)
      {
      r.loop();  
      delay(1);   
      }  
}
//***********************************************************************************
void InitVariables() 
{
  EncVal = 0;
  countLast = 0;
  counter = 0;
  next_player = 0;
  buttoncount = 0;
  win[0] = 0;
  win[1] = 0;
  win[2] = 0;
  win[3] = 0;
  win[4] = 0;
  next_slot = 0;
  pos = 1;
  flag = 0;
  matrix.fillScreen(0);
  RefreshCube();
}
//*******************************************************************************************  
// Rain Random Colors Upward
void RandomColor()
{
unsigned char xa, ya, x1, y1, x2, y2, y3, tempcolor, tempcolor1, tempcolor2, zpos;

 if(digitalRead(BUTTON) != 0)
    {
    while(digitalRead(BUTTON) != 0)   // Repeat
       {
          xa = random(4);
          ya = random(4);
          x1 = random(4);
          y1 = random(4);
          x2 = random(4);
          y3 = random(4);
          tempcolor = random(12);
          tempcolor1 = random(12);
          tempcolor2 = random(12);
          for(zpos = 0; zpos < 4; zpos++)
             {
             setPixelZXY(zpos, xa, ya, colors[tempcolor][0], colors[tempcolor][1], colors[tempcolor][2]);
             setPixelZXY(zpos + 1, x1, y1, colors[tempcolor1][0], colors[tempcolor1][1], colors[tempcolor1][2]);
             setPixelZXY(zpos + 2, x2, y2, colors[tempcolor2][0], colors[tempcolor2][1], colors[tempcolor2][2]);
             setPixelZXY(zpos + 3, xa, y3, colors[tempcolor2][0], colors[tempcolor2][1], colors[tempcolor2][2]);
             matrix.show();
             delayms(80);
             matrix.fillScreen(0);
             }
          if(digitalRead(BUTTON) == 0)
             {
             while(digitalRead(BUTTON) == 0)   
                {             
                }
                counter = 0;
                ClearCube();
                next_player = 0;
                delayms(2000);
                return;
                } 
       }
    }
}
//***********************************************************************************
void FlashRow(unsigned char pos1, unsigned char pos2, unsigned char pos3, unsigned char pos4)
{
    while(digitalRead(BUTTON) != 0)   // Hold Here
       {                 
       setPixelZXY(LEDPos[pos1][0], LEDPos[pos1][1], LEDPos[pos1][2], MemoryColors[pos1][0], MemoryColors[pos1][1],MemoryColors[pos1][2]);
       setPixelZXY(LEDPos[pos2][0], LEDPos[pos2][1], LEDPos[pos2][2], MemoryColors[pos2][0], MemoryColors[pos2][1],MemoryColors[pos2][2]);
       setPixelZXY(LEDPos[pos3][0], LEDPos[pos3][1], LEDPos[pos3][2], MemoryColors[pos3][0], MemoryColors[pos3][1],MemoryColors[pos3][2]);
       setPixelZXY(LEDPos[pos4][0], LEDPos[pos4][1], LEDPos[pos4][2], MemoryColors[pos4][0], MemoryColors[pos4][1],MemoryColors[pos4][2]);
       matrix.show();
       RefreshCube();
       delayms(500);
       setPixelZXY(LEDPos[pos1][0], LEDPos[pos1][1], LEDPos[pos1][2], 0, 0, 0);
       setPixelZXY(LEDPos[pos2][0], LEDPos[pos2][1], LEDPos[pos2][2], 0, 0, 0);
       setPixelZXY(LEDPos[pos3][0], LEDPos[pos3][1], LEDPos[pos3][2], 0, 0, 0);
       setPixelZXY(LEDPos[pos4][0], LEDPos[pos4][1], LEDPos[pos4][2], 0, 0, 0); 
       matrix.show();     
       delayms(500);        
       }
    delayms(2000);
}
//***********************************************************************************
void FlashError(unsigned char pos1)
{
int a;

    for(a = 0; a < 5; a++)
      {
      setPixelZXY(TempPos[pos1][0], TempPos[pos1][1], TempPos[pos1][2], 255, 255, 0);
      matrix.show();  
      delayms(500);
      setPixelZXY(LEDPos[pos1][0], LEDPos[pos1][1], LEDPos[pos1][2], MemoryColors[pos1][0], MemoryColors[pos1][1], MemoryColors[pos1][2]);
      matrix.show();  
      delayms(500);      
      }   
}
//************************************************************************************************************************************* 
void RefreshCube()
{
  int CubePos;
  
    for(CubePos = 1; CubePos < LED_COUNT + 1; CubePos++)
      {  
      setPixelZXY(LEDPos[CubePos][0], LEDPos[CubePos][1], LEDPos[CubePos][2], MemoryColors[CubePos][0], MemoryColors[CubePos][1],MemoryColors[CubePos][2]); 
      }
    matrix.show();       
} 
//***********************************************************************************
void ClearCube()
{
int a, b, c;

   for(a = 0; a < 5; a++)
     { 
     for(b = 0; b < 5; b++)
       {
       for(c = 0; c < 5; c++)
         {
         setPixelZXY(a, b, c, 0, 0, 0); //uses 24bit RGB color Code
         }    
       }      
     }      
   matrix.show();    
   for(a = 0; a < 65; a++)
     { 
     LEDPos[a][3] = 0;
     }
     
   for(a = 0; a < 65; a++)
     {
     for(b = 0; b < 4; b++)
        {       
        MemoryColors[a][b] = 0;
        }
     }   
   countLast = 1;
   next_player = 0;
   buttoncount = 0;     
}
//***********************************************************************************
void ClearPixel(unsigned char x, unsigned char y, unsigned char z)
{
      setPixelZXY(z, x, y, 0, 0, 0);
}
//***********************************************************************************
void SelectColor(unsigned char r, unsigned char g, unsigned char b, unsigned pos)
{
      setPixelZXY(TempPos[pos][0], TempPos[pos][1], TempPos[pos][2], r, g, b); 
      matrix.show();       
}
//***********************************************************************************
void SelectPixel(unsigned char r, unsigned char g, unsigned char b, unsigned pos)
{
      setPixelZXY(TempPos[pos][0], TempPos[pos][1], TempPos[pos][2], 0, 0, 0);
      matrix.show();
      delayms(50);
      setPixelZXY(TempPos[pos][0], TempPos[pos][1], TempPos[pos][2], r, g, b); 
      matrix.show();
      delayms(50);  
          
}
//***********************************************************************************
//set the pixel (Z,X,Y) of RGB Cube with colours R,G,B 
void setPixelZXY(unsigned char zaxis, unsigned char x, unsigned char y, unsigned char r, unsigned char g, unsigned char b) 
{     
     matrix.drawPixel(ZX[zaxis][x] + x, y, matrix.Color(r, g, b));
     
}
//*****************************************************************************************************
void EvaluateCube()
{
   //*****Read first position
   while (win[3] == 0)
     {
      current_pos = Compare[pos][next_slot];       //Check to see if start of a row 
      //*************************************** 
      switch(LEDPos[current_pos][3])
        {
        case 0:

           ++pos;

        break;
        //*************************************
        case 1:

        last_pos = current_pos;
        while(last_pos = current_pos)
           {
            
           win[flag] = Compare[pos][next_slot];
           ++next_slot;
           current_pos = Compare[pos][next_slot];
           if(LEDPos[current_pos][3] == 1)
              {
              ++flag;
              win[flag] = Compare[pos][next_slot];
              last_pos = current_pos;
              if(win[3] != 0) break;
              }
           if(LEDPos[current_pos][3] != 1)
              {
              win[0] = 0;
              win[1] = 0;
              win[2] = 0;
              win[3] = 0;
              win[4] = 0; 
              ++pos;
              flag = 0;
              next_slot = 0; 
              break; 
              }
           }
        break;
        //*************************************
        case 2:

        last_pos = current_pos;
        while(last_pos = current_pos)
           {
            
           win[flag] = Compare[pos][next_slot];
           ++next_slot;
           current_pos = Compare[pos][next_slot];
           if(LEDPos[current_pos][3] == 2)
              {
              ++flag;
              win[flag] = Compare[pos][next_slot];
              last_pos = current_pos;
              if(win[3] != 0) break;
              }
           if(LEDPos[current_pos][3] != 2)
              {
              win[0] = 0;
              win[1] = 0;
              win[2] = 0;
              win[3] = 0;
              win[4] = 0; 
              ++pos;
              flag = 0;
              next_slot = 0;  
              break;
              }
           }

        break;         
        }
       //***************************************      
       if(win[3] != 0)                          //if we have a WIN!!!!
          {
          FlashRow(win[0], win[1], win[2], win[3]);
          win[0] = 0;
          win[1] = 0;
          win[2] = 0;
          win[3] = 0;
          win[4] = 0;
          pos = 1;
          flag = 0;
          next_slot = 0;
          InitVariables(); 
 
          delayms(2000);
          ClearCube(); 
          next_player = 1;          
          return;
          }
       if(pos > 77)                             //if not a full row
          {
          pos = 1;
          next_slot = 0;  
          return;  
          }                                                
     } 
}
//***********************************************************************************
void SetMove(unsigned char r, unsigned char g, unsigned char b, unsigned posn)
{
   //**********************************
   //Error Check Routine
   if(LEDPos[posn][3] >= 1)
      {
      FlashError(posn);        
      if(player == 0) next_player = 1;
      if(player == 1) next_player = 3;
      countLast = 1;    
      return;
      } 
   //***********************************     
      LEDPos[posn][0] = TempPos[posn][0];
      LEDPos[posn][1] = TempPos[posn][1];
      LEDPos[posn][2] = TempPos[posn][2];
      MemoryColors[posn][0] = r;
      MemoryColors[posn][1] = g;
      MemoryColors[posn][2] = b;

      if(r == PlayerColor[1][0] && g == PlayerColor[1][1] && b == PlayerColor[1][2]) LEDPos[posn][3] = 1;   // Lock in Color to LEDPos
      if(r == PlayerColor[2][0] && g == PlayerColor[2][1] && b == PlayerColor[2][2]) LEDPos[posn][3] = 2;   // Lock in Color to LEDPos

      pos = 1;
      next_player = next_player + 1;
      setPixelZXY(LEDPos[posn][0], LEDPos[posn][1], LEDPos[posn][2], MemoryColors[posn][0], MemoryColors[posn][1],MemoryColors[posn][2]);
      
}
//***********************************************************************************
void SetPlayerColor()
{
    EncVal = 0;
    while(1 == 1)
       {       
       ColorChoice = countLast;
       if (countLast > 9) countLast = 0;   // if Colorchoice is max'd out, start over
       ++counter;                    // increment delayms count
       delayms(1);
       if(counter >= 10000)
          {
          counter = 10000;     // don't move from here. gets cleared when RandomColor and button is pressed.
          RandomColor();
          countLast = 0;         
          }// End If
       SelectColor(colors[ColorChoice][0], colors[ColorChoice][1], colors[ColorChoice][2], 49); 
       delayms(1);             
       if(digitalRead(BUTTON) == 0)  // when button is pressed, select Player one and move to player 2
          {
          while(digitalRead(BUTTON) == 0)   // Hold Here
            {}
          PlayerColor[1][0] = colors[ColorChoice][0];
          PlayerColor[1][1] = colors[ColorChoice][1];
          PlayerColor[1][2] = colors[ColorChoice][2]; 
          countLast = 1;       
          goto repeet;         
          }// End If
       }// End While
repeet: 
//************************************       
    while(1 == 1)
       {
       
       ColorChoice = countLast;
       if (countLast > 9)
          {
          countLast = 0;
          }
       SelectColor(colors[ColorChoice][0], colors[ColorChoice][1], colors[ColorChoice][2], 49);
       SelectColor(colors[ColorChoice][0], colors[ColorChoice][1], colors[ColorChoice][2], 53);  
       delayms(1);      
       if(digitalRead(BUTTON) == 0)
          {
          while(digitalRead(BUTTON) == 0)   // Hold Here
            {}  
          PlayerColor[2][0] = colors[ColorChoice][0];
          PlayerColor[2][1] = colors[ColorChoice][1];
          PlayerColor[2][2] = colors[ColorChoice][2]; 
          if(PlayerColor[2][0] == PlayerColor[1][0] && PlayerColor[2][1] == PlayerColor[1][1] &&
            PlayerColor[2][2] == PlayerColor[1][2])
            {
            FlashError(49);
            goto repeet;
            }// End If          
          countLast = 1;
          ++next_player;            
          return;        
          }// End If
       }// End While
}
//**************************************************************************************
void setup()
{
  Serial.begin(115200);
  pinMode(BUTTON, INPUT_PULLUP);

  matrix.begin();
  matrix.setBrightness(30);
  matrix.clear();
  strip.setBrightness(25);  
  strip.begin();  
  strip.show();                 
  r.setChangedHandler(rotate);

  InitVariables();
  
}
//***************************************************************************************
void loop()
{
  r.loop();
  //*** Increment Next Axis ***
  if(digitalRead(BUTTON) == 0)
    {
    while(digitalRead(BUTTON) == 0)   // Hold Here
        {        
        ++buttoncount;
        delayms(100);
        if(buttoncount >= 20) 
           {
           ClearCube();
           InitVariables();
           RefreshCube();  
           buttoncount = 0;
           next_player = -1;
           while(digitalRead(BUTTON) == 0)   // Hold Here
           {}           
           break;
           }        
        }//End While          
                                                 
    ++next_player;    
    if(next_player >= 5) next_player = 1;     
    RefreshCube();         
    }// End if 
  // *************************  
  if(buttoncount >= 5) buttoncount = 0; 
  if(next_player >= 5) next_player = 1;
  RefreshCube();

//***********************************************************       
        switch(next_player)
           {
           case 0:   // Wait for First Move *****************

                SetPlayerColor();
                RefreshCube();

           break;
           
           case 1:   // Player One Move **********************

                SelectPixel(PlayerColor[1][0], PlayerColor[1][1], PlayerColor[1][2], countLast); 
                player = 0;                              
 
           break;

           case 2:   // Set Red Pixel and Go To Player 2
                
                EncVal = 1; 
                SetMove(PlayerColor[1][0], PlayerColor[1][1], PlayerColor[1][2], countLast);
                ++buttoncount;                    
                
                r.setPosition(countLast);      
                EvaluateCube();
               
           break;
           
           case 3:   // Player Two Move ********************** 

                SelectPixel(PlayerColor[2][0], PlayerColor[2][1], PlayerColor[2][2], countLast);
                player = 1;
 
           break;
           
           case 4:   // Set Blue Pixel and Go Back *************** 

                EncVal = 1;
                SetMove(PlayerColor[2][0], PlayerColor[2][1], PlayerColor[2][2], countLast);
                ++buttoncount;                      
                
                r.setPosition(countLast);
                EvaluateCube();

           break;                                                                 
           }//End Switch
//***********************************************************    
           
}//End Loop
//**************************************************************************************************************
