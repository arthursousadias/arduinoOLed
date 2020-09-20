#include <Wire.h>
#include <SPI.h>
#include <../Adafruit_BMP280_Library-2.0.1/Adafruit_BMP280.h>
#include <../Adafruit_SSD1306-master/Adafruit_SSD1306.h>
#include <../DHT/dht.h>
#include <../RTClib-1.11.2/RTClib.h>
/*#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)*/
#define OLED_RESET 4
#define dht_dpin 9

const float fatorCorrecaoTemperatura = 0; //Adiciona para corrigir a distorção da temperatura;

char daysOfTheWeek[7][12] = {"DOM", "SEG", "TER","QUA", 
                            "QUI", "SEX", "SAB"};
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_BMP280 bmp; 
dht sensorDht;
RTC_DS1307 rtc;


void writeText(String text,int fontSize, int posX, int posY);
void ajustDateHour();
String getDateNow();
String getHourNow();
String getDayOfWeek();
String getDay();

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  Serial.println(F("BMP280 test"));
  if(!bmp.begin(0x76)){ //SE O SENSOR NÃO FOR INICIALIZADO NO ENDEREÇO I2C 0x76, FAZ
    Serial.println(F("Sensor BMP280 não foi identificado! Verifique as conexões.")); //IMPRIME O TEXTO NO MONITOR SERIAL
    while(1); //SEMPRE ENTRE NO LOOP
  }
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
 display.clearDisplay();
 //
 //Observação: Adicionar um  delay(200) se o ajuste do rtc não funcionar
 //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

}

void loop() {    
    double umidade = 0;
    display.clearDisplay();
    sensorDht.read11(dht_dpin);
    umidade = sensorDht.humidity;
    writeText( "T: " + (String(bmp.readTemperature()-fatorCorrecaoTemperatura) + "C "),1,0,0);
    writeText( "P: " + (String(int(bmp.readPressure()/100)) + "hPa"),1,65,0);
    writeText(getDayOfWeek(),1,1,8);
    writeText(getDay(),1,1,16);
    writeText(getHourNow(),2,30,8); 
    writeText("A: "+(String(int(bmp.readAltitude(1013.25))))+"M",1,0,25);
    writeText(" U: "+(String(int(umidade-0)))+" %",1,65,25);
    delay(1000);
}

void writeText(String text,int fontSize, int posX, int posY) {
  display.setTextSize(fontSize);
  display.setTextColor(WHITE);
  display.setCursor(posX,posY);
  display.println(text);
  display.display();  
}

String getDateNow(){
  DateTime now = rtc.now();
  String dataAtual;
  dataAtual = String(now.day(),DEC) + '/' + String(now.month(),DEC) + '/' + String(now.year(),DEC);
  return dataAtual;
   
}
String getHourNow(){
  DateTime now = rtc.now();
  String horaAtual;
  horaAtual = String(now.hour(),DEC) + ':' + String(now.minute(),DEC) + ':'+ String(now.second(),DEC);
 return horaAtual;   
}
String getDayOfWeek(){
  DateTime now = rtc.now();
  return daysOfTheWeek[now.dayOfTheWeek()];
}
String getDay(){
  DateTime now = rtc.now();
  return String(now.day(),DEC);
}

void ajustDateHour(){
  DateTime now = rtc.now();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}
