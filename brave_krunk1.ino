// --- TINKERCAD KÖNYVTÁRAK ---
// CSAK EZT A KETTŐT ADD HOZZÁ A FIÓK IKONNAL:
// 1. Adafruit SSD1306
// 2. Adafruit GFX Library
// (A DHT-t NEM KELL hozzáadni!)

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

// --- BEKÖTÉSEK ---
#define TMP_PIN A2   // !!! A FEKETE SZENZOR KÖZÉPSŐ LÁBA AZ A2-re MENJEN!
#define SOIL_PIN A0  // Talajnedvesség
#define LDR_PIN A1   // Fényérzékelő

#define BUTTON_PIN 2 // Gomb

#define R_PIN 3      // Piros LED
#define G_PIN 5      // Zöld LED
#define B_PIN 6      // Kék LED

#define SERVO_PIN 9  // Szervó motor

// --- KIJELZŐ BEÁLLÍTÁS ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Servo soilGauge;

// --- VÁLTOZÓK ---
int DRY_THRESHOLD = 600; 
int DARK_THRESHOLD = 800; 

int wateringCount = 0;
bool wasDry = false;
long lastCheck = 0;

void setup() {
  Serial.begin(9600);

  // Bemenetek
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // A TMP36-nak nem kell pinMode, mert analóg
  
  // Kimenetek
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  // Szervó indítása
  soilGauge.attach(SERVO_PIN);
  
  // OLED indítása
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("HIBA: OLED Konyvtar hianyzik! Add hozza a menuben!"));
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println(F("Rendszer Indul..."));
    display.display();
    delay(1000);
  }
}

void loop() {
  // Gomb figyelése
  if (digitalRead(BUTTON_PIN) == LOW) {
    wateringCount = 0;
    Serial.println("Szamlalo nullazva.");
    delay(200); 
  }

  // Időzítés
  if (millis() - lastCheck > 1000) {
    lastCheck = millis();
    
    // --- 1. ADATOK MÉRÉSE ---
    
    // TMP36 Hőmérséklet számítása
    int tmpReading = analogRead(TMP_PIN);
    // Átváltás feszültségre (5V rendszer)
    float voltage = tmpReading * 5.0;
    voltage /= 1024.0; 
    // Átváltás Celsiusra ((Feszültség - 0.5) * 100)
    float temperature = (voltage - 0.5) * 100;
    
    int soil = analogRead(SOIL_PIN); 
    int light = analogRead(LDR_PIN);

    // --- 2. KIÉRTÉKELÉS ---
    bool isDry = (soil < DRY_THRESHOLD); 
    bool isDark = (light < DARK_THRESHOLD);

    // Számláló logika
    if (isDry && !wasDry) {
      wateringCount++;
    }
    wasDry = isDry;

    // --- 3. LED VEZÉRLÉS ---
    if (isDry) {
      // PIROS
      analogWrite(R_PIN, 255); analogWrite(G_PIN, 0); analogWrite(B_PIN, 0);
    } else if (isDark) {
      // KÉK
      analogWrite(R_PIN, 0); analogWrite(G_PIN, 0); analogWrite(B_PIN, 255);
    } else {
      // ZÖLD
      analogWrite(R_PIN, 0); analogWrite(G_PIN, 255); analogWrite(B_PIN, 0);
    }

    // --- 4. SZERVÓ ---
    // Talajnedvesség (kb 0-876) átváltása 0-180 fokra
    int angle = map(soil, 0, 876, 0, 180); 
    soilGauge.write(angle);

    // --- 5. OLED KIÍRÁS ---
    display.clearDisplay();
    
    // Felső sor: Hőmérséklet (Most már a TMP36-ból!)
    display.setCursor(0,0);
    display.print("Homerseklet: "); 
    display.print((int)temperature); 
    display.print(" C");

    // Középső sor: Állapot
    display.setCursor(0, 16);
    if(isDry) display.print("ALLAPOT: SZARAZ!");
    else if(isDark) display.print("ALLAPOT: SOTET!");
    else display.print("ALLAPOT: OK");

    // Alsó sor: Számláló és Talaj érték
    display.setCursor(0, 32);
    display.print("Ontozesek: "); display.print(wateringCount);
    display.setCursor(0, 45);
    display.print("Talaj adat: "); display.print(soil);
    
    display.display();
  }
}