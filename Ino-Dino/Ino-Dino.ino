#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//int ScrX = 0, ScrY = 0, flagX = 0, flagY = 0;

char rdletter;
int ButtonPin = 11, ButtonState, ButtonLastState; /*按钮状态*/
int IfStart, BlockX, mark, JumpCounter, MaxMark;  /*是否开始游戏, 障碍物X坐标, 得分, 跳起时用于实现空中停留的计数器, 最高分*/
int x1 = 20, x2 = 16, x3 = 25, y1 = 50, y2 = 45, y3 = 38; /*小恐龙身体坐标*/
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

void StartUp(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(25,12);
  display.print("Press To Start\n      Mark:");
  display.println(mark);
  display.print("      High:");
  display.println(MaxMark);
  display.display();
}

int ButtonDown(){
  ButtonState = digitalRead(ButtonPin);
  if (ButtonState != HIGH) {
    return 1;
  } else {
    return 0;
  }
}

char RandomLetter(){
   int num = random(0, 26);
   char l = (char)(num + 65);
   return l;
}


void setup() {
  // put your setup code here, to run once:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  pinMode(ButtonPin,INPUT_PULLUP);
  mark = 0;
  IfStart = 0;
  MaxMark = 0;
  StartUp();
}

void loop() {
  if(IfStart == 0){
    if (ButtonDown() == 1) {
      display.clearDisplay();
      IfStart = 1;
      mark = 0;
      JumpCounter = 0;
    }
  } else {
    BlockX = 110;
    rdletter = RandomLetter();
    display.setTextSize(2);
    
    
    while(BlockX >= 1){
      if(mark == 0 && BlockX >= 70){
        ButtonLastState = 0;
      }
      
      display.clearDisplay();
      
      if(y1 == 50 && x1 >= BlockX && x2 <= BlockX + 10){
        if(mark > MaxMark) MaxMark = mark;
        StartUp();
        IfStart = 0;
        break;
      }
      
      display.setCursor(BlockX,36);
      display.print(rdletter);
      
      display.drawLine(0, 51, 128, 51, SSD1306_WHITE);
      
      if((ButtonDown() == 1 || ButtonLastState == 1) && JumpCounter <= 10){
        /*y1 = 15;
        y2 = 10;
        y3 = 3;*/

        y1 = 50;
        y2 = 45;
        y3 = 38;

        if(JumpCounter <= 4){
          y1 -= JumpCounter * 6;
          y2 -= JumpCounter * 6;
          y3 -= JumpCounter * 6;
        }

        if(JumpCounter >= 5){
          y1 = 15;
          y2 = 10;
          y3 = 3;
          y1 += JumpCounter * 4;
          y2 += JumpCounter * 4;
          y3 += JumpCounter * 4;
        }
        
        display.drawTriangle(x1, y1, x2, y2, x3, y3, SSD1306_WHITE);
        JumpCounter += 1;
        ButtonLastState = 1;
        
        if(JumpCounter >= 10){
          JumpCounter = 0;
          ButtonLastState = 0;
        }
        
      } else {
        y1 = 50;
        y2 = 45;
        y3 = 38;
        display.drawTriangle(x1, y1, x2, y2, x3, y3, SSD1306_WHITE);
      }
      display.display();
      BlockX -= 4;
      delay(50);
    }
    mark += 100;
  }
}
