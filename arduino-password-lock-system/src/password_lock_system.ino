#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <MPU6050.h>

#define SERVO_PIN 8
#define RED_LED_PIN 12
#define GREEN_LED_PIN 13
#define BUZZER_PIN 11

Servo myservo;
LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x27 I2C adresi; ekran 16x2 boyutunda

const byte rows = 4, cols = 3;
char keys[rows][cols] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[rows] = {A0, A1, A2, A3};
byte colPins[cols] = {6, 7, 9};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

MPU6050 mpu;

char savedPassword[5] = "0000";
char enteredPassword[5]; //Kullanıcının girdiği şifreyi tutacak dizi.
int currentPosition = 0; //Kullanıcının şu anda şifre girişinde hangi karakter pozisyonunda olduğunu tutar.
int invalidCount = 0; //Yanlış şifre girme sayacı.
bool alarmActive = false;

void setup() {
  Wire.begin();
  mpu.initialize();

  lcd.init();
  lcd.backlight();
  
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  myservo.attach(SERVO_PIN);
  myservo.write(0); // Başlangıçta kapalı

  lcd.setCursor(0,0);
  lcd.print("ENTER THE CODE");
  delay(100);
}


void loop() {
  if (alarmActive) {
    alarmLoop();
    return;
  }

  if (detectMotion()) {
    triggerAlarm();
    return;
  }

  char key = keypad.getKey(); //kullanıcı tuş takımındaki bir tuşa bastığında o tuşun karakterini döner.
  if (key != NO_KEY) {
    if (key == '*') {
      if (currentPosition > 0) {
        currentPosition--;
        enteredPassword[currentPosition] = '\0';
        lcd.setCursor(currentPosition, 1);
        lcd.print(" ");
        lcd.setCursor(currentPosition, 1);
      }
    } 
    else if (key >= '0' && key <= '9' && currentPosition < 4) {
      enteredPassword[currentPosition] = key;
      lcd.setCursor(currentPosition, 1);
      lcd.print("*");
      currentPosition++;
    } 
    else if (key == '#') {
      if (currentPosition == 4) {
        checkPassword();
      } else {
        lcd.clear();
        lcd.print("Incomplete Pass"); // şifre tamamlanmamışsa uyar.
        delay(1500);
        lcd.clear();
        lcd.print("ENTER THE CODE");
        currentPosition = 0;
      }
    }
  }
}

bool detectMotion() {
  // MPU6050 ivme ve gyro verilerini oku
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Basit hareket algılama: ivme değerlerinden biri eşik değerini aşarsa hareket var
  const int16_t motionThreshold = 30000; 
  delay(100);

  if (abs(ax) > motionThreshold || abs(ay) > motionThreshold || abs(az) > motionThreshold) {
    return true;
  }
  return false;
}

void triggerAlarm() {
  alarmActive = true;
  lcd.clear();
  lcd.print("MOTION DETECTED!");
  digitalWrite(RED_LED_PIN, HIGH);

  // Alarm sesi (buzzer)
  for (int i = 0; i < 5; i++) {
    tone(BUZZER_PIN, 1000, 200);
    delay(300);

    noTone(BUZZER_PIN);
    delay(300);
  }
}

void alarmLoop() {
  digitalWrite(RED_LED_PIN, HIGH);
  tone(BUZZER_PIN, 1000);
  delay(500);
  noTone(BUZZER_PIN);
  delay(500);
}
  
void checkPassword() {
  enteredPassword[currentPosition] = '\0';
  if (strcmp(enteredPassword, savedPassword) == 0) {
    invalidCount = 0;
    alarmActive = false;  // Alarmı kapat şifre doğruysa
    digitalWrite(RED_LED_PIN, LOW);
    noTone(BUZZER_PIN);
    unlock();
  } else {
    invalidCount++;
    incorrectPassword();
    if (invalidCount >= 3) {
      lcd.clear();
      lcd.print("SYSTEM LOCKED");
      tone(BUZZER_PIN,1000);
      while (1);
    }
  }
  currentPosition = 0;
}

void incorrectPassword() {
  lcd.clear();
  lcd.print("CODE INCORRECT");

  // LED ve buzzer birlikte 3 saniye açık kalsın
  digitalWrite(RED_LED_PIN, HIGH);
  tone(BUZZER_PIN, 1000);
  delay(900); // 0.9 saniye

  // Kapat
  noTone(BUZZER_PIN);
  digitalWrite(RED_LED_PIN, LOW);

  // Ekranı sıfırla
  lcd.clear();
  lcd.print("ENTER THE CODE");
}

void unlock() {
  lcd.clear();
  lcd.print("Access Granted");
  digitalWrite(GREEN_LED_PIN, HIGH);
  delay(900);
  noTone(BUZZER_PIN);
  digitalWrite(GREEN_LED_PIN, LOW);
  delay(500);

  myservo.write(90);

  for (int secondsLeft = 10; secondsLeft > 0; secondsLeft--) {
    lcd.clear();
    lcd.print("Locks in:");
    lcd.setCursor(6, 1);
    lcd.print(secondsLeft);
    lcd.print(" sec");
    delay(1000);
  }

  lcd.clear();
  lcd.print("Locking...");
  myservo.write(0);
  delay(100);

  lcd.clear();
  lcd.print("ENTER THE CODE");
}
