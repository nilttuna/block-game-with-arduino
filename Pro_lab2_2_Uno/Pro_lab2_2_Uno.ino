#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LDR_PIN A1


//dikdortgen
int rectHeight = 1; // yükseklik
int rectWidth = 15; // genişlik
int rectY = SCREEN_HEIGHT - rectHeight; // Ekranın en altındaki yükseklik
int rectX = (SCREEN_WIDTH - rectWidth) / 2; // Dikdörtgeni ekranın ortasına hizala

// yuvarlak
int circleX = SCREEN_WIDTH / 2; 
int circleY = rectY - 8; // dikdörtgenin üstünde
int radius = 4; // Yuvarlağın yarıçapı

int topSpeedX = 2;
int topSpeedY = 2;

const byte potPin = A0;
int potDeger;
int led_sayaci = 0;
int skor=0;
int begin_game=1;
int kirilan=0;

int A = 0; 
int B = 13;
int C = 5;
int D = 6;
int E = 7;
int F = 8;
int G = 9;

  int brickData1[][2] = {
    {40, 0},
    {60, 0},
    {80, 0},};
  int brickData2[][2] = {
    {0, 0},
    {20, 0},
    {40, 0},
    {60, 0},
    {80, 0},
    {100, 0},};
    int brickData3[][2] = {
    {0, 0},
    {20, 0},
    {40, 0},
    {60, 0},
    {80, 0},
    {100, 0},
    {40, 10},
    {60, 10},
    {80, 10},};

    int brickData4[][2] = {
    {0, 0},
    {20, 0},
    {40, 0},
    {60, 0},
    {80, 0},
    {100, 0},
    {0, 10},
    {20, 10},
    {40, 10},
    {60, 10},
    {80, 10},
    {100, 10},};

    int brickData5[][2] = {
    {0, 0},
    {20, 0},
    {40, 0},
    {60, 0},
    {80, 0},
    {100, 0},
    {0, 10},
    {20, 10},
    {40, 10},
    {60, 10},
    {80, 10},
    {100, 10},
    {40, 20},
    {60, 20},
    {80, 20},};

  int ldrDegeri;
  uint16_t screen_colour=BLACK;
  int object_colour=WHITE;


const unsigned char brick[] PROGMEM =
{
    B11111111, B11111111,
    B11111111, B11111111,
    B11111111, B11111111,
    B11111111, B11111111,
    B11111111, B11111111,
    B11111111, B11111111,
    B11111111, B11111111,
    B11111111, B11111111
};

const int upButton = 12;
const int downButton = 11;
const int selectButton = 10;
int menu = 1;

int led1=2;
int led2=3;
int led3=4;


Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup() {
  Serial.begin(9600);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 başlatılamadı"));
    while (true);
  }
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);

  oled.clearDisplay();
  beginMenu();
  }


void beginMenu()
{
  oled.clearDisplay();
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);

  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(1, 0);
  oled.println("--------");

  oled.setCursor(0, 10);
  oled.println(" BASLAT");

  oled.setCursor(1, 20);
  oled.println("--------");

  oled.setCursor(0, 30);
  oled.println(" CIKIS");

  oled.setCursor(0, 40);
  oled.println("--------");
  oled.display();
}



int sayac=1;

void loop() {
  if(sayac){
  if (!digitalRead(downButton)) {
    menu++;
    if (menu > 2) {
      menu = 1;
    }
    updateMenu();
    delay(100);
    while (!digitalRead(downButton));
  }
  else if (!digitalRead(upButton)) {
    menu--;
    if (menu < 1) {
      menu = 2;
    }
    updateMenu();
    delay(100);
    while (!digitalRead(upButton));
  }
  else if (!digitalRead(selectButton)) {
    executeAction();
    sayac=0;
    delay(100);
    while (!digitalRead(selectButton));
  }
}

}

void updateMenu() {
  oled.clearDisplay();
  switch (menu) {
    case 1:
      oled.setCursor(0, 0);
      oled.print(">");
      oled.setCursor(1, 0);
      oled.print(" BASLAT");
      oled.setCursor(0, 30);
      oled.print(" CIKIS");
      break;
    case 2:
      oled.setCursor(0, 0);
      oled.print(" BASLAT");
      oled.setCursor(0, 30);
      oled.print(">");
      oled.setCursor(1, 30);
      oled.print(" CIKIS");
      break;
  }
  oled.display();
}

void executeAction() {
  if(begin_game)
      {
        display_score(skor);
        begin_game=0;
      }
  switch (menu) {
    case 1:
      baslat();
      
      break;
    case 2:
      oled.clearDisplay();
      oled.setTextSize(1);
      oled.setCursor(0, 0);
      oled.println("Oyunumuza");
      oled.println("gosterdiginiz ilgi"); 
      oled.println("icin tesekkurler :)");
      oled.display();
      delay(1500);
      break;
  }
}


void baslat() {
      oled.clearDisplay();
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);

      oled.drawRect(rectX, rectY, rectWidth, rectHeight, object_colour);
      oled.fillCircle(circleX, circleY, radius, object_colour);

      while(true){
      potDeger = analogRead(potPin);
      // Dikdörtgenin X konumunu potansiyometre değerine göre güncelle
      int hareketMiktari = map(potDeger, 0, 1023, 0, SCREEN_WIDTH - rectWidth); // Potansiyometre değerini ekran genişliğine göre dönüştür
      rectX = hareketMiktari;

      // Dikdörtgenin X konumunu sınırlar içinde tut
      rectX = constrain(rectX, 0, SCREEN_WIDTH - rectWidth);
      updateScreen();
      
        }
}

int kontrol=5;
void updateScreen()
{
   oled.clearDisplay();
  ldrDegeri = analogRead(LDR_PIN);

  if (ldrDegeri < 250) {
    object_colour = BLACK;
    screen_colour=WHITE;
  } else {
    object_colour = WHITE;
    screen_colour=BLACK;

  }

    oled.fillScreen(screen_colour);

    oled.drawRect(rectX, rectY, rectWidth, rectHeight, object_colour);
    oled.fillCircle(circleX, circleY, radius, object_colour);

    circleX += topSpeedX;
    circleY += topSpeedY;

    bool dikdortgen_carpisma= circleY >= rectY - radius && circleY <= rectY +  rectHeight && circleX >= rectX - radius && circleX <= rectX + rectWidth;

    if (dikdortgen_carpisma) {
        topSpeedY = -topSpeedY; 
    }

    bool ekran_carpismax=circleX + radius >= SCREEN_WIDTH || circleX - radius <= 0;
    bool ekran_carpismay=circleY - radius <= 0;
    if (ekran_carpismax) {
    topSpeedX = -topSpeedX; 
    }
    if (ekran_carpismay) {
    topSpeedY = -topSpeedY; 
}

//seviye geçişi
  if(kirilan>=0 && kirilan<3)
      {
        level1();
        if(kirilan==3 && kontrol==5)
        {
          oled.clearDisplay();
          oled.setTextSize(1);
          oled.setCursor(0, 0);
          oled.println("Level2");
          oled.display();
          circleX = rectX + rectWidth / 2; // Dikdörtgenin üstünde konumlandır
          circleY = rectY - radius;
          topSpeedX = 2.4; 
          topSpeedY = -2.4; 
          kontrol--;
          delay(1000);
        }
      }
       
    if(kirilan>=3 && kirilan<9)
      {
        level2();
        if(kirilan==9 && kontrol==4)
        {
          oled.clearDisplay();
          oled.setTextSize(1);
          oled.setCursor(0, 0);
          oled.println("Level3");
          oled.display();
          circleX = rectX + rectWidth / 2; 
          circleY = rectY - radius;
          topSpeedX = 2.8; 
          topSpeedY = -2.8; 
          kontrol--;
          delay(1000);
        }
      }

      if(kirilan>=9 && kirilan<18)
      {
        level3();
        if(kirilan==18 && kontrol==3)
        {
          oled.clearDisplay();
          oled.setTextSize(1);
          oled.setCursor(0, 0);
          oled.println("Level4");
          oled.display();
          circleX = rectX + rectWidth / 2; 
          circleY = rectY - radius;
          topSpeedX = 3.2; 
          topSpeedY = -3.2; 
          kontrol--;
          delay(1000);
        }
      }

      if(kirilan>=18 && kirilan<30)
      {
        level4();
        if(kirilan==30 && kontrol==2)
        {
          oled.clearDisplay();
          oled.setTextSize(1);
          oled.setCursor(0, 0);
          oled.println("Level5");
          oled.display();
          circleX = rectX + rectWidth / 2; 
          circleY = rectY - radius;
          topSpeedX = 3.4; 
          topSpeedY = -3.4; 
          kontrol--;
          delay(1000);
        }
      }

      if(kirilan>=30 && kirilan<45)
      {
        level5();
        if(kirilan==45 && kontrol==1)
        {
          oled.clearDisplay();
          oled.setTextSize(1);
          oled.setCursor(0, 0);
          oled.print("Skor: ");
          oled.print(skor);
          oled.display();
          kontrol--;
          delay(1000);
          menu=1;
          beginMenu();
          exit(EXIT_SUCCESS);

        }
      }

        
}
    
 
 
void led_count()
{
   if(led_sayaci==0)
   {
      digitalWrite(led1, LOW);
   }
   else if(led_sayaci==1)
   {
      digitalWrite(led2, LOW);
   }
   else if(led_sayaci==2)
   {
      digitalWrite(led3, LOW);
   }
    led_sayaci = (led_sayaci + 1) % 3;
      
    if(led_sayaci == 0 )
   {
      oled.clearDisplay();
      oled.setTextSize(1);
      oled.setCursor(0, 0);
      oled.print("Skor: ");
      oled.print(skor);
      oled.display();
      delay(1500);
      menu=1;
      beginMenu();
      exit(EXIT_SUCCESS);
   }

}


void increase_led()
{
  if(led_sayaci==0 )
  {

  }
  else if(led_sayaci==1 )
  {
    digitalWrite(led1, HIGH);
    led_sayaci=0;
  }
  else if(led_sayaci==2 )
  {
    digitalWrite(led2, HIGH);
    led_sayaci=1;
  }
  

}

void breaking_bricks(int brickData[][2], int bricks) {
    int brickWidth = 16;
    int brickHeight = 8;

    for (int i = 0; i < bricks; i++) {
        int brickX = brickData[i][0];
        int brickY = brickData[i][1];

        if (brickX != -1 && brickY != -1) {

            oled.drawBitmap(brickX, brickY, brick, brickWidth, brickHeight, object_colour);

            // Çarpışma tespiti
            if (circleY + radius >= brickY && circleY - radius <= brickY + brickHeight && circleX + radius >= brickX && circleX - radius <= brickX + brickWidth) {
               
                brickData[i][0] = -1;
                brickData[i][1] = -1;
                skor++;
                display_score(skor);
                kirilan++;
                if(kirilan==2 || kirilan==6 || kirilan==8 || kirilan==12 ||kirilan==16 || kirilan==24 || kirilan==28 || kirilan==34){
                increase_led();
                }
                topSpeedY = -topSpeedY;
            }
        }
    }
}

void display_score(int skor)
{
  if(skor==0 && begin_game==1)
  {
    // "0" Sayısını yazar.
    digitalWrite(A,HIGH);
    digitalWrite(B,HIGH);
    digitalWrite(C,HIGH);
    digitalWrite(D,HIGH);
    digitalWrite(E,HIGH);
    digitalWrite(F,HIGH);
    digitalWrite(G,LOW);
  }
  else if(skor%9==1)
  {
    digitalWrite(A,LOW);
    digitalWrite(B,HIGH);
    digitalWrite(C,HIGH);
    digitalWrite(D,LOW);
    digitalWrite(E,LOW);
    digitalWrite(F,LOW);
    digitalWrite(G,LOW);
  }
  else if(skor%9==2)
  {
    digitalWrite(A,HIGH);
    digitalWrite(B,HIGH);
    digitalWrite(C,LOW);
    digitalWrite(D,HIGH);
    digitalWrite(E,HIGH);
    digitalWrite(F,LOW);
    digitalWrite(G,HIGH);
  }
  else if(skor%9==3)
  {
    digitalWrite(A,HIGH);
    digitalWrite(B,HIGH);
    digitalWrite(C,HIGH);
    digitalWrite(D,HIGH);
    digitalWrite(E,LOW);
    digitalWrite(F,LOW);
    digitalWrite(G,HIGH);
  }
  else if(skor%9==4)
  {
    digitalWrite(A,LOW);
    digitalWrite(B,HIGH);
    digitalWrite(C,HIGH);
    digitalWrite(D,LOW);
    digitalWrite(E,LOW);
    digitalWrite(F,HIGH);
    digitalWrite(G,HIGH);
  }
  else if(skor%9==5)
  {
    digitalWrite(A,HIGH);
    digitalWrite(B,LOW);
    digitalWrite(C,HIGH);
    digitalWrite(D,HIGH);
    digitalWrite(E,LOW);
    digitalWrite(F,HIGH);
    digitalWrite(G,HIGH);
  }
  else if(skor%9==6)
  {
    digitalWrite(A,HIGH);
    digitalWrite(B,LOW);
    digitalWrite(C,HIGH);
    digitalWrite(D,HIGH);
    digitalWrite(E,HIGH);
    digitalWrite(F,HIGH);
    digitalWrite(G,HIGH);
  }
  else if(skor%9==7)
  {
    digitalWrite(A,HIGH);
    digitalWrite(B,HIGH);
    digitalWrite(C,HIGH);
    digitalWrite(D,LOW);
    digitalWrite(E,LOW);
    digitalWrite(F,LOW);
    digitalWrite(G,LOW);
  }
  else if(skor%9==8)
  {
    digitalWrite(A,HIGH);
    digitalWrite(B,HIGH);
    digitalWrite(C,HIGH);
    digitalWrite(D,HIGH);
    digitalWrite(E,HIGH);
    digitalWrite(F,HIGH);
    digitalWrite(G,HIGH);
  }
  else if(skor%9==0)
  {
    digitalWrite(A,HIGH);
    digitalWrite(B,HIGH);
    digitalWrite(C,HIGH);
    digitalWrite(D,HIGH);
    digitalWrite(E,LOW);
    digitalWrite(F,HIGH);
    digitalWrite(G,HIGH);
  }
}

void level1()
{
    if (circleY > SCREEN_HEIGHT - radius) {
        // Ekranın altına düşerse, topun konumunu sıfırla
        circleX = rectX + rectWidth / 2; // Dikdörtgenin üstünde konumlandır
        circleY = rectY - radius;
        topSpeedX = 2.0; 
        topSpeedY = -2.0; 
        led_count();
    }
   
    breaking_bricks(brickData1,3);
    oled.display();

}

  void level2()
  {
    
    if (circleY > SCREEN_HEIGHT - radius) {
        circleX = rectX + rectWidth / 2; 
        circleY = rectY - radius;
        topSpeedX = 2.4; 
        topSpeedY = -2.4; 
        led_count();
    }

    breaking_bricks(brickData2,6);
    oled.display();
  

  }

  void level3()
  {
     if (circleY > SCREEN_HEIGHT - radius) {
        circleX = rectX + rectWidth / 2; 
        circleY = rectY - radius;
        topSpeedX = 2.8; 
        topSpeedY = -2.8; 
        led_count();
    }
    breaking_bricks(brickData3,9);
    oled.display();

  }
   void level4()
  {
     if (circleY > SCREEN_HEIGHT - radius) {
        circleX = rectX + rectWidth / 2; 
        circleY = rectY - radius;
        topSpeedX = 3.2; 
        topSpeedY = -3.2; 
        led_count();
    }
    breaking_bricks(brickData4,12);
    oled.display();

  }

   void level5()
  {
     if (circleY > SCREEN_HEIGHT - radius) {
        circleX = rectX + rectWidth / 2; 
        circleY = rectY - radius;
        topSpeedX = 3.4; 
        topSpeedY = -3.4; 
        led_count();
    }
    breaking_bricks(brickData5,15);
    oled.display();

  }







