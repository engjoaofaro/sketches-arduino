/*  Projeto - Sistema de Irrigação inteligente
 *  Eng Joao Faro
 */
#include "DHT.h"

#define pinSensorA A0

const int pino_dht = 9;
const int pino_led_solenoide = 12;
float temperatura;
float umidade;
DHT dht(pino_dht, DHT11);

void setup() {
  pinMode(pino_led_solenoide, OUTPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {

  // Aguarda alguns segundos entre uma leitura e outra
  delay(10000);

  temperatura = dht.readTemperature();
  umidade = dht.readHumidity();

  // Se ocorreu alguma falha durante a leitura
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Falha na leitura do Sensor DHT!");
  } else {
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print(" *C ");
    
    Serial.print("\t");
  
    Serial.print("Umidade externa: ");
    Serial.print(umidade);
    Serial.print(" %\t"); 
    
    Serial.println();
  }

  Serial.print("Umidade do solo: ");
  Serial.print(analogRead(pinSensorA)); 
  Serial.print("  ");

  Serial.print("  Atuador: ");
  if (analogRead(pinSensorA) > 720) {

    while(millis() < 6000 || analogRead(pinSensorA) > 350) {
      
      if (isnan(umidade) || isnan(temperatura)) {
        Serial.println("Falha na leitura do Sensor DHT!");
        } else {
          Serial.print("Temperatura: ");
          Serial.print(temperatura);
          Serial.print(" *C ");
          
          Serial.print("\t");
        
          Serial.print("Umidade: ");
          Serial.print(umidade);
          Serial.print(" %\t"); 
          
          Serial.println();
        }

      Serial.println("SOLO SECO - SOLENOIDE LIGADO");
      digitalWrite(pino_led_solenoide, HIGH); // Ativa Solenoide

      delay(6000);
      
    }
  } else {
    
    Serial.println("SOLO UMIDO - SOLENOIDE DESLIGADO");
    digitalWrite(pino_led_solenoide, LOW); // Desativa Solenoide
  }
}
