#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

#define TMP_PIN A2
#define SOIL_PIN A0
#define LDR_PIN A1
#define BUTTON_PIN 2
#define R_PIN 3
#define G_PIN 5
#define B_PIN 6
#define SERVO_PIN 9
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Servo soilGauge;

int DRY_THRESHOLD = 600;
int DARK_THRESHOLD = 800;
int wateringCount = 0;
bool wasDry = false;
long lastCheck = 0;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  soilGauge.attach(SERVO_PIN);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("HIBA: OLED hiba"));
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
  if (digitalRead(BUTTON_PIN) == LOW) {
    wateringCount = 0;
    Serial.println("Szamlalo nullazva.");
    delay(200);
  }

  if (millis() - lastCheck > 1000) {
    lastCheck = millis();

    int tmpReading = analogRead(TMP_PIN);
    float voltage = tmpReading * 5.0;
    voltage /= 1024.0;
    float temperature = (voltage - 0.5) * 100;

    int soil = analogRead(SOIL_PIN);
    int light = analogRead(LDR_PIN);

    bool isDry = (soil < DRY_THRESHOLD);
    bool isDark = (light < DARK_THRESHOLD);

    if (isDry && !wasDry) {
      wateringCount++;
    }
    wasDry = isDry;

    if (isDry) {
      analogWrite(R_PIN, 255); analogWrite(G_PIN, 0); analogWrite(B_PIN, 0);
    } else if (isDark) {
      analogWrite(R_PIN, 0); analogWrite(G_PIN, 0); analogWrite(B_PIN, 255);
    } else {
      analogWrite(R_PIN, 0); analogWrite(G_PIN, 255); analogWrite(B_PIN, 0);
    }

    int angle = map(soil, 0, 876, 0, 180);
    soilGauge.write(angle);

    display.clearDisplay();
    display.setCursor(0,0);
    display.print("Homerseklet: ");
    display.print((int)temperature);
    display.print(" C");

    display.setCursor(0, 16);
    if(isDry) display.print("ALLAPOT: SZARAZ!");
    else if(isDark) display.print("ALLAPOT: SOTET!");
    else display.print("ALLAPOT: OK");

    display.setCursor(0, 32);
    display.print("Ontozesek: "); display.print(wateringCount);
    display.setCursor(0, 45);
    display.print("Talaj adat: "); display.print(soil);

    display.display();
  }
}
