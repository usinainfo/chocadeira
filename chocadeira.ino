#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Wire.h>
#include <dht.h>

dht DHT;
RTC_DS1307 rtc;

LiquidCrystal_I2C lcd(0x27, 20, 4);

#define DHT22_PIN   5
#define servo       3
#define mosfet      10

int pwmluz;

int Hor;
int Min;
int Sec;
int Data;
int falta;

int i;

uint8_t   date;

void setup() {
  rtc.begin();
  pinMode(mosfet, OUTPUT);
  analogWrite(servo, 80);
  lcd.init();
  lcd.backlight();
}

void loop() {
//rtc.adjust(DateTime(2020, 1, 1, 07, 25, 50));
  DateTime now = rtc.now();
  Hor = rtc.now().hour();
  Min = rtc.now().minute();
  Sec = rtc.now().second();
  Data = rtc.now().day();

  falta = (22 - Data);

// Se faltar mais de 4 dias para a choca move os motores as 6:00, 12:00, 17:56 e 23:59
  if (falta > 4) {
    if ( Hor == 6 &&  (Min == 00 || Min == 00) && Sec == 01) {
      //analogWrite(servo, 80);
      for (int i = 50; i <= 80; i++) {
    analogWrite(servo, i);
    delay(100);
  }
 }

    if ( Hor == 12 &&  (Min == 00 || Min == 00)&& Sec == 01) {
      //analogWrite(servo, 50);
      for (int i = 80; i >= 40; i--) {
    analogWrite(servo, i);
    delay(100);
  }
 }

    if ( Hor == 17 &&  (Min == 56 || Min == 00)&& Sec == 01) {
      //analogWrite(servo, 80);
      for (int i = 50; i <= 80; i++) {
    analogWrite(servo, i);
    delay(100);
  }
 }

    if ( Hor == 23 &&  (Min == 59 || Min == 00)&& Sec == 01) {
      //analogWrite(servo, 120);
      for (int i = 80; i <= 120; i++) {
    analogWrite(servo, i);
    delay(100);
  }
 }
}

// Se faltar 4 dias ou menos para chocar a chocadeira para na posição 80
  if (falta <= 4){
    analogWrite(servo, 80);
  }

  uint32_t start = micros();
  int chk = DHT.read22(DHT22_PIN);
  uint32_t stop = micros();

  if (isnan(DHT.humidity) || isnan(DHT.temperature))  {
    return;
  }

// Imprime as informações de temperatura e umidade no display
  lcd.setCursor(0, 0);
  lcd.print("U:");
  lcd.print(DHT.humidity);
  lcd.print("  T:");
  lcd.print(DHT.temperature);

// Imprime as informações de hora e tempo que falta para a eclosão
  lcd.setCursor(0, 1);
  lcd.print(Hor);
  lcd.print(":");
  lcd.print(Min);
  lcd.print(" F: ");
  lcd.print(falta);
  lcd.print(" dias  ");

// Realiza o cálculo de pwm da luz de acordo com a temperatura
  pwmluz = -(DHT.temperature - 37.9) * 255;

  if (pwmluz > 255)  {
    analogWrite(mosfet, 255);
  }

  if (pwmluz <= 255)  {
    analogWrite(mosfet, pwmluz);
  }

  if (pwmluz <= 1)  {
    digitalWrite(mosfet, LOW);
    delay(5000);
  }
}
