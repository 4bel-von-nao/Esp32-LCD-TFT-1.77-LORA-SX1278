
#include <Adafruit_GFX.h>   
#include <Adafruit_ST7735.h> 
#include <SPI.h>
#include <LoRaHSPI.h>
//#include "esp_task_wdt.h"

#define TFT_CS 2
#define TFT_RST 21
#define TFT_DC 19
#define TFT_SCLK 18   
#define TFT_MOSI 23
#define BLANCO ST7735_WHITE
#define NEGRO ST7735_BLACK
#define AMARILLO ST7735_YELLOW
#define AZUL ST7735_BLUE
#define VERDE ST7735_GREEN

#define SS   15    // NSS (CS) conectado a GPIO15
#define RST  27     // RST conectado a GPIO27
#define DIO0 33    // DIO0 conectado a GPIO33

String idReceptor   = "TX1";  
String idTransmisor = "TX2";
String datos = "";
int voltaje;
unsigned long tiempoInicio = 0; 
unsigned long tiempoInicio2 = 0; 
int x = 100; int k = 100;
int i = 0;
float frecuencia = 0.3;
int amplitud = 10;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup(void) {
  Serial.begin(9600);
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) {  // Cambia la frecuencia según tu región
    Serial.println("Error al inicializar LoRa");
    while (1);
  }
  LoRa.setSpreadingFactor(7);         // SF igual al transmisor
  LoRa.setSignalBandwidth(250E3);    // Ancho de banda igual
  LoRa.setCodingRate4(5);            // Tasa de codificación igual
  LoRa.setPreambleLength(6);         // Preámbulo igual
  delay(1000);
  Serial.println("LoRa inicializado");
 
  xTaskCreate(Task1, "Tarea 1", 1000, NULL, 1, NULL);

  tft.initR(INITR_BLACKTAB);  
  tft.fillScreen(ST7735_BLACK);   
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(0);
  tft.setCursor(30,80);
  tft.println("Hello World!");  
  delay(1000);
  tft.fillScreen(ST7735_BLACK); 
  tft.drawRect(0, 0, 128, 160, BLANCO);
  tft.drawLine(0, 53, 128, 53, BLANCO);
  tft.drawLine(0, 106, 128, 106, BLANCO);
}

void loop() { 
  if(millis()-tiempoInicio >= 100){
    rayo();
    seno1();
    seno2();
    tiempoInicio = millis();
  }
  if(millis()-tiempoInicio2 >= 1000){
    Voltaje();
    corriente(datos);
    potencia(datos,voltaje);
    tiempoInicio2 = millis();
  }
}

void Task1(void *pvParameters) {
  while(1) {
    vTaskDelay(10);                        // DELAY DE 10 TICS... O DESACTIVO EL PERRO O LE PONGO EL DELAY
     // Verificar si hay paquetes disponibles
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      String recibido = "";
      while (LoRa.available()) {
        recibido += (char)LoRa.read();
      }

      // Verificar si el mensaje tiene el identificador correcto
      if (recibido.startsWith(idTransmisor + ":")) {
        datos = recibido.substring(idTransmisor.length() + 1); // Extraer datos después de ":"
        Serial.println(datos);
        int pos1 = datos.indexOf(',');
        int pos2 = datos.indexOf(',');
        datos = datos.substring(0,pos1);
      } else {
        Serial.println("Paquete ignorado: " + recibido);
      }
    }
  }
}

void Voltaje(){
  tft.fillRect(2,16,87,25,NEGRO);
  voltaje = 220;
  voltaje = voltaje + random(-1,1);
  String voltaje2 = String(voltaje);
  tft.setTextColor(BLANCO);
  tft.setTextSize(1);
  tft.setCursor(2,2);
  tft.print("Voltaje rms:");
  tft.setTextColor(AMARILLO);
  tft.setCursor(2,18);
  tft.setTextSize(3);
  tft.print("220"); tft.print("V");
}

void corriente(String datos){
  tft.fillRect(2,71,87,21,NEGRO);
  tft.setTextColor(BLANCO);
  tft.setTextSize(1);
  tft.setCursor(2,55);
  tft.print("Corriente rms:");
  tft.setTextColor(AZUL);
  tft.setCursor(2,71);
  tft.setTextSize(3);
  tft.print(datos); tft.print("A"); 
}

void potencia(String datos, int voltaje){
  tft.fillRect(2,124,87,21,NEGRO);
  float potencia;
  float corriente = datos.toFloat();
  potencia = corriente * voltaje;
  tft.setTextColor(BLANCO);
  tft.setTextSize(1);
  tft.setCursor(2,108);
  tft.print("Potencia:");
  tft.setTextColor(VERDE);
  tft.setCursor(2,124);
  tft.setTextSize(3);
  tft.print(potencia); tft.print("W");
}

void seno1(){
    float seno = amplitud*sin(frecuencia*(x-100));
    int y = 18 - seno;
    tft.drawPixel(x, y, BLANCO);
    x++;

    if (x >= 122) {
      x = 100;  
      //tft.fillScreen(ST77XX_BLACK);  
      tft.fillRect(100,8,22,20,NEGRO);
    }
}

void seno2(){
    float seno = amplitud*sin(frecuencia*(k-100));
    int y = 75 - seno;
    tft.drawPixel(k, y, BLANCO);
    k++;
    if (k >= 122) {
      k = 100;  
      //tft.fillScreen(ST77XX_BLACK);  
      tft.fillRect(100,65,22,20,NEGRO);
    }
}

void rayo(){

  if( i == 0){tft.drawLine(100, 120, 120, 120, BLANCO);}
  if( i == 1){tft.drawLine(99, 121, 119, 121, BLANCO);}
  if( i == 2){tft.drawLine(98, 122, 118, 122, BLANCO);}
  if( i == 3){tft.drawLine(97, 123, 117, 123, BLANCO);}
  if( i == 4){tft.drawLine(96, 124, 116, 124, BLANCO);}
  if( i == 5){tft.drawLine(95, 125, 115, 125, BLANCO);}
  if( i == 6){tft.drawLine(94, 126, 114, 126, BLANCO);}
  if( i == 7){tft.drawLine(93, 127, 113, 127, BLANCO);}
  if( i == 8){tft.drawLine(92, 128, 112, 128, BLANCO);}
  if( i == 9){tft.drawLine(91, 129, 111, 129, BLANCO);}
  if( i == 10){tft.drawLine(90, 130, 110, 130, BLANCO);}
  if( i == 11){tft.drawLine(89, 131, 109, 131, BLANCO);}

  if( i == 12){tft.drawLine(96, 132, 114, 132, BLANCO);}
  if( i == 13){tft.drawLine(96, 133, 113, 133, BLANCO);}
  if( i == 14){tft.drawLine(95, 134, 111, 134, BLANCO);}
  if( i == 15){tft.drawLine(95, 135, 110, 135, BLANCO);}
  if( i == 16){tft.drawLine(94, 136, 108, 136, BLANCO);}
  if( i == 17){tft.drawLine(94, 137, 107, 137, BLANCO);}
  if( i == 18){tft.drawLine(93, 138, 105, 138, BLANCO);}
  if( i == 19){tft.drawLine(93, 139, 104, 139, BLANCO);}
  if( i == 20){tft.drawLine(92, 140, 102, 140, BLANCO);}
  if( i == 21){tft.drawLine(92, 141, 101, 141, BLANCO);}
  if( i == 22){tft.drawLine(91, 142, 99, 142, BLANCO);}
  if( i == 23){tft.drawLine(91, 143, 98, 143, BLANCO);}
  if( i == 24){tft.drawLine(90, 144, 96, 144, BLANCO);}
  if( i == 25){tft.drawLine(90, 145, 95, 145, BLANCO);}
  if( i == 26){tft.drawLine(89, 146, 93, 146, BLANCO);}
  if( i == 27){tft.drawLine(89, 147, 92, 147, BLANCO);}
  if( i == 28){tft.drawLine(88, 148, 90, 148, BLANCO);}
  if( i == 29){tft.drawLine(88, 149, 89, 149, BLANCO);}
  if( i == 30){tft.drawLine(87, 150, 88, 150, BLANCO);}
  if( i == 31){tft.drawLine(87, 151, 87, 151, BLANCO);}
  if( i == 32){tft.drawLine(86, 152, 86, 152, BLANCO);}  
  i++;
  if(i>=31){
    tft.fillRect(86,120,34,32,NEGRO);
    i=0;
  }
}
