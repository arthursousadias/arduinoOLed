/* Desenvolvido por Arthur de Sousa Dias */

#include <Wire.h>
#include <SPI.h>
#include <../Adafruit_BMP280_Library-2.0.1/Adafruit_BMP280.h>
#include <../Adafruit_SSD1306-master/Adafruit_SSD1306.h>
#include <../DHT/dht.h>
#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)
#define OLED_RESET 4
#define dht_dpin 9

const float fatorCorrecaoTemperatura = 1.3; //Adiciona para corrigir a distorção da temperatura;

Adafruit_SSD1306 display(OLED_RESET);
Adafruit_BMP280 bmp; //OBJETO DO TIPO Adafruit_BMP280 (I2C)
dht sensorDht;
void writeText(String text,int fontSize, int posX, int posY);
void writeBlankText(String text,int fontSize, int posX, int posY);


void setup() {
  //Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  Serial.println(F("BMP280 test"));

  if(!bmp.begin(0x76)){ //SE O SENSOR NÃO FOR INICIALIZADO NO ENDEREÇO I2C 0x76, FAZ
    Serial.println(F("Sensor BMP280 não foi identificado! Verifique as conexões.")); //IMPRIME O TEXTO NO MONITOR SERIAL
    while(1); //SEMPRE ENTRE NO LOOP
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X16,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
 display.clearDisplay();

}

void loop() {
    double umidade = 0;
    double temperatura = 0;
    display.clearDisplay();
    sensorDht.read11(dht_dpin);
    umidade = sensorDht.humidity;
    temperatura = sensorDht.temperature;
    writeText( "T: " + (String(bmp.readTemperature()-fatorCorrecaoTemperatura) + "C "),1,0,0);
    writeText( "P: " + (String(int(bmp.readPressure()/100)) + "hPa"),1,65,0);
    writeText("20:48",2,35,8); //Será incluído o RTC
    writeText("A: "+(String(int(bmp.readAltitude(1013.25))))+"M",1,0,25);
    writeText(" U: "+(String(int(umidade-10)))+" %",1,65,25);
    delay(4000);
}

void writeText(String text,int fontSize, int posX, int posY) {
  display.setTextSize(fontSize);
  display.setTextColor(WHITE);
  display.setCursor(posX,posY);
  display.println(text);
  display.display();  
}

void writeBlankText(String text,int fontSize, int posX, int posY) {
  display.setTextSize(fontSize);
  display.setTextColor(WHITE);
  display.setCursor(posX,posY);
  display.println(text);
  display.display();  
}

