#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SSD1306TUR.h"

#include "SevSeg.h"
#include "pitches.h"

#define SCREEN_WIDTH        128 // OLED display width, in pixels
#define SCREEN_HEIGHT        64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET            -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // Datasheete bakınız; 0x3D -> 128x64, 0x3C -> 128x32
#define LDR_PIN 1
#define SPEAKER_PIN 23



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define MAX_FALLING_OBJECTS 5

struct FallingObject {
  int x, y;
  bool isActive;
};

FallingObject fallingObjects[MAX_FALLING_OBJECTS];

void initializeFallingObjects() {
  for (int i = 0; i < MAX_FALLING_OBJECTS; i++) {
    fallingObjects[i].isActive = false;
  }
}

SevSeg sevseg; //Instantiate a seven segment controller object

int menuSelection = 0; // Menüdeki seçilen öğe
// Oyunun global değişkenleri
int score = 0;
int tempScore = score;
int lives = 3;
bool gameRunning = false;
int ballX = SCREEN_WIDTH / 2;
int ballY = SCREEN_HEIGHT / 2;
int ballDirectionX = 1;
int ballDirectionY = 1;
int paddleX = SCREEN_WIDTH / 2;
int paddleWidth = 20;
const int paddleHeight = 5;


int buttonPins[] = {2, 3, 4};
int ledPins[] = {11, 12, 13};


int segment1[] = {30, 31, 52, 53, 51, 50, 48, 49};


void display0(void) 
{ 
  digitalWrite(segment1[0],LOW); //ALT
  digitalWrite(segment1[1],LOW); //SOL ALT
  digitalWrite(segment1[2],LOW); //NOKTA
  digitalWrite(segment1[3],LOW);  //SAĞ ALT
  digitalWrite(segment1[4],HIGH);  //orta
  digitalWrite(segment1[5],LOW);  //SOL UST
  digitalWrite(segment1[6],LOW);  //SAĞ UST
  digitalWrite(segment1[7],LOW);  //UST
}
void display1(void) 
{ 
  digitalWrite(segment1[0],HIGH); //ALT
  digitalWrite(segment1[1],HIGH); //SOL ALT
  digitalWrite(segment1[2],HIGH); //NOKTA
  digitalWrite(segment1[3],LOW);  //SAĞ ALT
  digitalWrite(segment1[4],HIGH);  //orta
  digitalWrite(segment1[5],HIGH);  //SOL UST
  digitalWrite(segment1[6],LOW);  //SAĞ UST
  digitalWrite(segment1[7],HIGH);  //UST
}
void display2(void) 
{ 
  digitalWrite(segment1[0],LOW); //ALT
  digitalWrite(segment1[1],LOW); //SOL ALT
  digitalWrite(segment1[2],LOW); //NOKTA
  digitalWrite(segment1[3],HIGH);  //SAĞ ALT
  digitalWrite(segment1[4],LOW);  //orta
  digitalWrite(segment1[5],HIGH);  //SOL UST
  digitalWrite(segment1[6],LOW);  //SAĞ UST
  digitalWrite(segment1[7],LOW);  //UST
}
void display3(void) 
{ 
  digitalWrite(segment1[0],LOW); //ALT
  digitalWrite(segment1[1],HIGH); 
  digitalWrite(segment1[2],LOW); //NOKTA
  digitalWrite(segment1[3],LOW);  //SAĞ ALT
  digitalWrite(segment1[4],LOW);  //orta
  digitalWrite(segment1[5],HIGH); 
  digitalWrite(segment1[6],LOW);  //SAĞ UST
  digitalWrite(segment1[7],LOW);  //UST
}
void display4(void) 
{  digitalWrite(segment1[0],HIGH); 
  digitalWrite(segment1[1],HIGH); 
  digitalWrite(segment1[2],HIGH); 
  digitalWrite(segment1[3],LOW);  //SAĞ ALT
  digitalWrite(segment1[4],LOW);  //orta
  digitalWrite(segment1[5],LOW);  //SOL UST
  digitalWrite(segment1[6],LOW);  //SAĞ UST
  digitalWrite(segment1[7],HIGH); 
}
void display5(void) 
{ 
  digitalWrite(segment1[0],LOW); //ALT
  digitalWrite(segment1[1],HIGH); 
  digitalWrite(segment1[2],LOW); //NOKTA
  digitalWrite(segment1[3],LOW);  //SAĞ ALT
  digitalWrite(segment1[4],LOW);  //orta
  digitalWrite(segment1[5],LOW);  //SOL UST
  digitalWrite(segment1[6],HIGH); 
  digitalWrite(segment1[7],LOW);  //UST
}
void display6(void) 
{ 
  digitalWrite(segment1[0],LOW); //ALT
  digitalWrite(segment1[1],LOW); //SOL ALT
  digitalWrite(segment1[2],LOW); //NOKTA
  digitalWrite(segment1[3],LOW);  //SAĞ ALT
  digitalWrite(segment1[4],LOW);  //orta
  digitalWrite(segment1[5],LOW);  //SOL UST
  digitalWrite(segment1[6],HIGH); 
  digitalWrite(segment1[7],HIGH); 
}

void display7(void) 
{ 
  digitalWrite(segment1[0],HIGH); 
  digitalWrite(segment1[1],HIGH); 
  digitalWrite(segment1[2],HIGH); 
  digitalWrite(segment1[3],LOW);  //SAĞ ALT
  digitalWrite(segment1[4],HIGH); 
  digitalWrite(segment1[5],HIGH); 
  digitalWrite(segment1[6],LOW);  //SAĞ UST
  digitalWrite(segment1[7],LOW);  //UST
}
void display8(void) 
{ 
  digitalWrite(segment1[0],LOW); //ALT
  digitalWrite(segment1[1],LOW); //SOL ALT
  digitalWrite(segment1[2],LOW); //NOKTA
  digitalWrite(segment1[3],LOW);  //SAĞ ALT
  digitalWrite(segment1[4],LOW);  //orta
  digitalWrite(segment1[5],LOW);  //SOL UST
  digitalWrite(segment1[6],LOW);  //SAĞ UST
  digitalWrite(segment1[7],LOW);  //UST
}
void display9(void) 
{ 
  digitalWrite(segment1[0],LOW); //ALT
  digitalWrite(segment1[1],HIGH); 
  digitalWrite(segment1[2],LOW); //NOKTA
  digitalWrite(segment1[3],LOW);  //SAĞ ALT
  digitalWrite(segment1[4],LOW);  //orta
  digitalWrite(segment1[5],LOW);  //SOL UST
  digitalWrite(segment1[6],LOW);  //SAĞ UST
  digitalWrite(segment1[7],LOW);  //UST
}

void setup() {

  
  Serial.begin(9600);
  Serial.begin(115200);
  pinMode(A7, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(8, INPUT);


  for(int i = 0; i < 8; i++){
    pinMode(segment1[i], OUTPUT); //segment 1
  } 
  for(int i = 0; i < 3; i++){
    pinMode(buttonPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
  }
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  byte numDigits = 1;
  byte digitPins[] = {};
  byte segmentPins[] = {30, 31, 52, 53, 51, 50, 48, 49 };
  bool resistorsOnSegments = true; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_ANODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);

  digitalWrite(ledPins[0], HIGH); //1. can
  digitalWrite(ledPins[1], HIGH); //2. can
  digitalWrite(ledPins[2], HIGH); //3. can



  // Buton pinleri giriş olarak ayarlanır
  pinMode(2, INPUT_PULLUP); // Yukarı buton
  pinMode(3, INPUT_PULLUP); // Aşağı buton
  pinMode(4, INPUT_PULLUP); // Seçim/Onay buton

  // Menü ekranını ayarla
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Menu:"));
  display.setCursor(0, 10);
  display.println(F("> Baslat"));
  display.setCursor(0, 20);
  display.println(F("Cikis"));
  display.display(); // Ekranı güncelle ve menüyü göster

  
}


bool showEndScreen = false;
bool showingExitMessage = false; // Çıkış mesajının gösterilip gösterilmediğini takip etmek için bir değişken

unsigned long endScreenStartTime = 0;
const unsigned long endScreenDuration = 3000; // 3 saniye


void loop() {
  if (digitalRead(LDR_PIN) == LOW) {
    display.invertDisplay(true);
  } else {
    display.invertDisplay(false);
  }

  if (!gameRunning && !showEndScreen && !showingExitMessage) {
        // Eğer oyun, bitiş ekranı veya çıkış mesajı gösterilmiyorsa, menü işlemlerini yönet
        handleMenu();
    } else if (gameRunning) {
        // Oyun çalışıyorsa, oyunun ana döngüsünü çalıştır
        playGame();
    } else if (showingExitMessage) {
        // Çıkış mesajı gösteriliyorsa, belirli bir süre sonra ana menüye dön
        delay(3000); // Mesajı 3 saniye göster
        showingExitMessage = false; // Mesaj gösterimini kapat
        updateMenuDisplay(); // Ana menüyü güncelle ve göster
    }
    
    
}

void endGame() {
    // Oyun sonlandığında çalışacak kodlar
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print("Game Over");
    display.setTextSize(1);
    display.setCursor(0, 25);
    display.print("Score: ");
    display.print(score);
    display.display();
    delay(3000); // 3 saniye beklet
    
    score = 0;
    tempScore = 0;
    lives = 3;
    gameRunning = false; // Oyun döngüsünü durdur
    showEndScreen = false;
    
    // Burada direkt menüye dönüş yapılıyor, ekstra bir durum kullanılmıyor
    updateMenuDisplay();
}

// Çıkış seçeneği için fonksiyon ekleyin
void selectExitOption() {
    showingExitMessage = true; // Çıkış mesajını göster
    handleMenu(); // Menüyü güncelle
}

void handleMenu() {
  // Menüdeki seçeneklerin gösterimi
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("Ana Menu:"));
  display.setCursor(0,10);
  if (menuSelection == 0) {
    display.println(F("> Baslat"));
    display.println(F("  Cikis"));
  } else {
    display.println(F("  Baslat"));
    display.println(F("> Cikis"));
  }
  display.display();

  if (digitalRead(2) == LOW) { // Yukarı butonu
    menuSelection = 0;
    delay(200); // Debounce için basit bir gecikme
  } else if (digitalRead(3) == LOW) { // Aşağı butonu
    menuSelection = 1;
    delay(200); // Debounce için basit bir gecikme
  }

  if (digitalRead(4) == LOW) { // Seçim/Onay butonu
    if (menuSelection == 0) {
      // Oyunu başlat
      gameRunning = true;
      setupGame(); // Oyunu başlatmadan önce gerekli ayarları yapar
    } else {
      // Çıkış yap (Bu örnekte ekran temizlenir ve ana menüde kalınır)
       /* 
      display.clearDisplay();
      display.display(); */
      display.clearDisplay();
        display.setCursor(0, 0);
        display.println(F("Oyunumuza gosterdiginiz ilgi icin tesekkurler"));
        display.display();
        delay(4000); // Mesajı bir süre göster
    }
    delay(200); // Debounce için basit bir gecikme
  }
}

void setupGame() {
  // Oyunun başlangıç ayarlarını burada yap
  score = 0;
  lives = 3;
  startGame();
  // Diğer gerekli ayarlar...
}


void updateMenuDisplay() {
  // Menü ekranını güncelle
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Menü:"));
  display.setCursor(0, 10);
  display.println(menuSelection == 0 ? F("> Baslat") : F("Baslat"));
  display.setCursor(0, 20);
  display.println(menuSelection == 1 ? F("> Cikis") : F("Cikis"));
  display.display();
}

void startGame() {
  // Oyun başlatma işlemleri
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Oyun basliyor..."));
  display.display();
  delay(2000);
  
  // Oyunun asıl içeriğini burada başlatın
  // Örnek:
   while (gameRunning) {
     playGame();
   }
 }


// Global değişkenler
#define BRICK_ROWS 3
#define BRICK_COLUMNS 6
#define BRICK_WIDTH  (SCREEN_WIDTH / BRICK_COLUMNS)
#define BRICK_HEIGHT 5
bool bricks[BRICK_ROWS][BRICK_COLUMNS]; // Tuğlaların durumu
int ballSpeed = 2;



void updateScoreDisplay() {
    // Skorun değerine bağlı olarak ilgili display fonksiyonunu çağır
    switch(tempScore) {
        case 0:
            display0();
            break;
        case 1:
            display1();
            break;
        case 2:
            display2();
            break;
        case 3:
            display3();
            break;
        case 4:
            display4();
            break;
        case 5:
            display5();
            break;
        case 6:
            display6();
            break;
        case 7:
            display7();
            break;
        case 8:
            display8();
            break;
        case 9:
            display9();
            break;
        
    }
}


bool nextLevel = false; // Yeni bölüme geçilip geçilmediğini kontrol etmek için
int level = 1; // Oyunun seviyesi



void initializeBricks() {
    // Seviyeye bağlı olarak farklı düzenler oluştur
    switch(level) {
        case 1:
            // Seviye 1 için daha karmaşık bir düzen
            for (int i = 0; i < BRICK_ROWS; i++) {
                for (int j = 0; j < BRICK_COLUMNS; j++) {
                    // Her sütunun çift ve tek satırlarında tuğlalar olacak
                    bricks[i][j] = (i + j) % 2 == 0;
                }
            }
            
            break;
        case 2:
            // Seviye 2 için daha da karmaşık bir düzen
            for (int i = 0; i < BRICK_ROWS; i++) {
                for (int j = 0; j < BRICK_COLUMNS; j++) {
                    // Sadece ortadaki satırlarda tuğlalar olacak
                    bricks[i][j] = i == 1 || i == 2;
                }
            }
            
            break;
        case 3:
            // Seviye 3 için basit bir düzen
            for (int i = 0; i < BRICK_ROWS; i++) {
                for (int j = 0; j < BRICK_COLUMNS; j++) {
                    bricks[i][j] = true;
                }
            }
            break;
        // Daha fazla seviye için benzer şekilde düzenler ekleyebilirsiniz.
        default:
            // Varsayılan bir düzen
            for (int i = 0; i < BRICK_ROWS; i++) {
                for (int j = 0; j < BRICK_COLUMNS; j++) {
                    bricks[i][j] = true;
                }
            }
    }
}



bool areAllBricksDestroyed() {
    for (int i = 0; i < BRICK_ROWS; i++) {
        for (int j = 0; j < BRICK_COLUMNS; j++) {
            if (bricks[i][j]) {
                return false; // Aktif tuğla varsa, false döndür
            }
        }
    }
    return true; // Tüm tuğlalar yok edilmişse, true döndür
}



void playGame() {
  initializeBricks(); // Tuğlaları başlat
  display.clearDisplay();
  display.display();

  resetBallAndPaddle(); // Oyun başladığında top ve paletin konumunu resetle
  updateLivesDisplay(); // Oyun başladığında can göstergesini güncelle

  
  // Oyun başladığında düşen objeler için dizi pasif olarak ayarlanır
  for (int i = 0; i < MAX_FALLING_OBJECTS; i++) {
    fallingObjects[i].isActive = false;
  }

  while (gameRunning && lives > 0) {
    int potValue = analogRead(A7); // Potansiyometreden değer oku
    paddleX = map(potValue, 0, 1023, 0, SCREEN_WIDTH - paddleWidth); // Paletin X konumunu ayarla

    ballX += ballDirectionX * ballSpeed;
    ballY += ballDirectionY * ballSpeed;

    if (ballX <= 0 || ballX >= SCREEN_WIDTH) ballDirectionX = -ballDirectionX;
    if (ballY <= 0) ballDirectionY = -ballDirectionY;

    if (ballY >= SCREEN_HEIGHT - paddleHeight && ballX >= paddleX && ballX <= paddleX + paddleWidth) {
      ballDirectionY = -ballDirectionY;
      ballY = SCREEN_HEIGHT - paddleHeight - 1; // Topu paletin üstünde tut
    }

    for (int i = 0; i < BRICK_ROWS; i++) {
      for (int j = 0; j < BRICK_COLUMNS; j++) {
        if (bricks[i][j]) {
          int brickX = j * BRICK_WIDTH;
          int brickY = i * BRICK_HEIGHT;
          int brickRightEdge = brickX + BRICK_WIDTH;
          int brickBottomEdge = brickY + BRICK_HEIGHT;
          if (ballX > brickX && ballX < brickRightEdge && ballY > brickY && ballY < brickBottomEdge) {
             bricks[i][j] = false; // Tuğlayı kaldır
            // Topun yönünü değiştirme
            if (ballDirectionY < 0) { // Eğer top yukarı yönde gidiyorsa
              ballDirectionY = 1; // Topun yönünü aşağı çevir
            } else {
              ballDirectionY = -1; // Eğer top zaten aşağı yönde ise, yukarı çevir
            }
            score++;
            tempScore = score;
            if(tempScore >= 10)
              tempScore = tempScore % 10;
            updateScoreDisplay();

            // %10 şansla düşen bir obje oluştur
            if (random(10) < 1) { // %10 şans
              for (int k = 0; k < MAX_FALLING_OBJECTS; k++) {
                if (!fallingObjects[k].isActive) {
                  fallingObjects[k].x = brickX + BRICK_WIDTH / 2;
                  fallingObjects[k].y = brickY;
                  fallingObjects[k].isActive = true;
                  break;
                }
              }
            }
            
          }
        }
      }
    }

    updateFallingObjects(); // Düşen objeleri güncelle

    if (ballY >= SCREEN_HEIGHT) {
      lives--;
       resetBallAndPaddle(); // Top ve paleti resetle

      updateLivesDisplay(); // LED'leri can sayısına göre güncelle

      if (lives <= 0) {
        gameRunning = false; // Can kalmadıysa oyunu bitir
        endGame();
      } else {
        // Can hala varsa, bir sonraki "can" için oyunu hazırla
        // Burada kısa bir bekleme süresi veya oyunun duraklatılmasını sağlayabilirsiniz.
        delay(1000); // Oyuncuya hazırlanma süresi ver
      }
    }

    if (areAllBricksDestroyed()) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0,0);
        display.println("Level Complete!");
        display.display();
        delay(5000); // 5 saniye bekle

        level++; // Seviyeyi artır
        ballSpeed *= 1.20; // Top hızını %20 artır
        initializeBricks(); // Yeni seviye için tuğlaları başlat
        resetBallAndPaddle(); // Top ve paleti resetle
        nextLevel = true; // Yeni seviye bayrağını ayarla
    }


    drawGame(); // Oyun durumunu çiz

    delay(50); // Oyun hızını ayarla
  }
}
void updateFallingObjects() {
  for (int i = 0; i < MAX_FALLING_OBJECTS; i++) {
    if (fallingObjects[i].isActive) {
      fallingObjects[i].y += 1; // Objeyi aşağı doğru hareket ettir
      
      // Oyuncunun paletiyle çarpışma kontrolü
      if (fallingObjects[i].y >= SCREEN_HEIGHT - paddleHeight && fallingObjects[i].x >= paddleX && fallingObjects[i].x <= paddleX + paddleWidth) {
        fallingObjects[i].isActive = false; // Objeyi pasif yap
        if (lives < 3) {
          lives++; // Can sayısını artır ama maksimum 3 olacak şekilde
          updateLivesDisplay(); // LED göstergesini güncelle
        }
      } else if (fallingObjects[i].y > SCREEN_HEIGHT) {
        fallingObjects[i].isActive = false; // Ekrandan çıkan objeyi pasif yap
      }
    }
  }
}



void drawGame() {
    display.clearDisplay(); // Ekranı temizle
    display.setTextSize(1); // Metin boyutunu ayarla
    display.setTextColor(SSD1306_WHITE); // Metin rengini ayarla

    // Puanı ve can sayısını çiz
    display.setCursor(0, 0); // Metin konumunu ayarla
    display.print("Score: ");
    display.print(score);
    display.print(" Lives: ");
    display.print(lives);

    // Paleti çiz
    display.fillRect(paddleX, SCREEN_HEIGHT - paddleHeight - 1, paddleWidth, paddleHeight, SSD1306_WHITE);

    // Topu çiz
    display.fillCircle(ballX, ballY, 2, SSD1306_WHITE);

    // Tuğlaları çiz
    for (int i = 0; i < BRICK_ROWS; i++) {
        for (int j = 0; j < BRICK_COLUMNS; j++) {
            if (bricks[i][j]) { // Eğer tuğla aktifse çiz
                int brickX = j * BRICK_WIDTH;
                int brickY = i * BRICK_HEIGHT + 10; // Üstten biraz boşluk bırakarak başla
                display.fillRect(brickX, brickY, BRICK_WIDTH - 1, BRICK_HEIGHT - 1, SSD1306_WHITE);
            }
        }
    }

    // Düşen objeleri çiz
    for (int i = 0; i < MAX_FALLING_OBJECTS; i++) {
        if (fallingObjects[i].isActive) {
            // Düşen objeyi belirli bir şekilde çizin, örneğin bir daire olarak
            display.fillCircle(fallingObjects[i].x, fallingObjects[i].y, 3, SSD1306_WHITE);
        }
    }

    // Ekrana yansıt
    display.display();
}


void resetBallAndPaddle() {
  // Top ve paletin konumunu resetler
  ballX = SCREEN_WIDTH / 2;
  ballY = SCREEN_HEIGHT / 2;
  paddleX = SCREEN_WIDTH / 2 - paddleWidth / 2; // Paleti ekranın ortasına yerleştir
}


void updateLivesDisplay() {
  // Can sayısını LED'ler üzerinde gösterir
  for (int i = 0; i < 3; i++) {
    if (i < lives) {
      digitalWrite(ledPins[i], HIGH); // Can varsa LED'i yak
    } else {
      digitalWrite(ledPins[i], LOW); // Can yoksa LED'i söndür
    }
  }
}



