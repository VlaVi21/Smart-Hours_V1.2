/*
****************************************************************************************************************************************************
 
                                                                   Smart-Hours_V1.2


                                                                 by : vla-vi21 (inst)
 
 Author of libraries :  ("EncButton.h") Alex Gyver
****************************************************************************************************************************************************
 */
#include <EncButton.h> //библиотека для работы с кнокпой и при желании с энкодером(будет отдельная прошивка)
EncButton<EB_TICK, 3> enc; // просто кнопка <KEY>


#include <Wire.h> //подключаем библиотеку для подсветки дисплея
#include <LiquidCrystal_I2C.h> //подключаем библиотеку для управления дисплеем
LiquidCrystal_I2C lcd(0x27, 16, 2); //указываем параметры дисплея


#include <iarduino_RTC.h> //подключаем библиотеку для модуля реального времени
iarduino_RTC time(RTC_DS1302, 8, 6, 7); // для модуля DS1302 - RST, CLK, DAT


#include <AHT10.h>  //подключаем библиотеку для датчика влажности и температуры
AHT10 myAHT20(AHT10_ADDRESS_0X38, AHT20_SENSOR); //настраиваем датчик


#include <SPI.h> //подключение барометра
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

Adafruit_BMP280 bme(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

void setup() {

  delay(300); //задержка для часов
  Serial.begin(9600);
  
  lcd.init();  //инициализация подсветки дисплея (wire)
  lcd.backlight(); //инициализация дисплея
  lcd.clear(); //очищаем дислей

  enc.setHoldTimeout(400);  // установка таймаута удержания кнопки

  time.begin(); //инициализация модуля реального времени
  //time.settime(0, 30, 18, 20, 9, 2, 1); // 0  сек, 30 мин, 18 часов, 20, сентябрь, 202, понедельник

  bme.begin();  //инициализация барометра

}
void loop() {
  static byte mode ; //переменная для счётчика нажатий
  enc.tick(); //функция отработки для кнопки(постоянно опрашивается)

  if (enc.isHolded())   lcd.backlight(); //при долгом нажатии на кнопку включаем подсветку
  if (enc.hasClicks(3))  lcd.noBacklight();//при третьем  нажатии на кнопку выключаем подсветку

  if (enc.isClick()) {        //нажатие кнопки
    if (++mode >= 9) mode = 0; //счётчик нажатий
  }
  switch (mode) { //переключение режимов 
    case 1 : task0(); break;
    case 2 : lcd.clear(); break;
    case 3 : task1(); break;
    case 4 : lcd.clear(); break;
    case 5 : task2();  break;
    case 6 : lcd.clear(); break;
    case 7 : task3(); break;
    case 8 : lcd.clear(); break;
 // case 9 : task4(); break; *вывод высоты*
 // case 0 : lcd.clear(); break;
  }

}

void task0() { //функция для вывода время
  static uint32_t tmr;
  if (millis() - tmr >= 1000) { //таймер (мс)
    tmr = millis();
    lcd.setCursor(0, 0);
    lcd.print(time.gettime("d M Y, D"));
    lcd.setCursor(4, 1);
    lcd.print(time.gettime("H:i:s"));
  }
}
void task1() { //функция для вывода температуры
  float t = myAHT20.readTemperature();
  static uint32_t tmr;
  if (millis() - tmr >= 5000) { //таймер (мс)
    tmr = millis();
    lcd.setCursor(2, 0);
    lcd.print("Temperature:");
    lcd.setCursor(5, 1);
    lcd.print(t);
    lcd.print(" C");
  }
}
void task2() { //функция для вывода влажности воздуха
  float H =  myAHT20.readHumidity();
  static uint32_t tmr;
  if (millis() - tmr >= 5000) { //таймер (мс)
    tmr = millis();
    lcd.setCursor(3, 0);
    lcd.print("Humidity:");
    lcd.setCursor(5, 1);
    lcd.print(H);
    lcd.print("%");
  }
}
void task3()  { //функция для вывода атмосферного давления
//float p = bme.readPressure();
//p = p / 100 ; //преобразуем Паскали в  hPa (для удобства)
  float p = bme.readPressure();
  p = p * 0.00750062; //преобразуем Паскали в мм.рт.ст
  static uint32_t tmr;
  if (millis() - tmr >= 5000) { //таймер (мс)
    tmr = millis();
    lcd.setCursor(3, 0);
    lcd.print("Pressure:");
    lcd.setCursor(2, 1);
    lcd.print(p);
    lcd.print(" mmGh");
  }
}
/*void task4() {  //Функция для вывода высоты (использовать по желанию)
 float h = bme.readAltitude(1013.25);
 static uint32_t tmr;
  if (millis() - tmr >= 5000) { //таймер (мс)
    lcd.setCursor(5, 0);
    lcd.print("Height");
    lcd.setCursor(5, 1);
    lcd.print(h);
    lcd.print(" m");
    }
  }
*/
