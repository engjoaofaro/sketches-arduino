/*  Projeto - Sistema de Irrigação inteligente
 *  Eng Joao Faro
 */
// INCLUSÃO DE BIBLIOTECAS
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// DEFINIÇÕES
#define endereco  0x27
#define colunas   16
#define linhas    2

// INSTANCIANDO OBJETOS
LiquidCrystal_I2C lcd(endereco, colunas, linhas);

// variáveis do programa
const int pinoSensor = A0;
const int pinoSensor2 = A1;
const int pinoSensor3 = A2;
const int pinoValvula = 13;
const int pinoRele = 2;
const int pinoBotaoLcd = 5;
const int limiarSeco = 40;
const int tempoRega = 5; // Tempo de rega em segundos
int umidadeSolo = 0;
int umidadeSolo2 = 0;
int umidadeSolo3 = 0;
int mediaUmidade = 0;

void setup() {
  pinMode(pinoRele, OUTPUT);
  pinMode(pinoValvula, OUTPUT);

  digitalWrite(pinoRele, LOW);
  // Desliga a válvula
  digitalWrite(pinoValvula, LOW);
  // Define o botao como entrada
  pinMode(pinoBotaoLcd, INPUT);

  lcd.init();
  // Inicia o backlight do lcd
  lcd.backlight();
  lcd.clear();
  // Exibe a mensagem no Display LCD.
  lcd.print("     SII JF    ");

  Serial.begin(9600);

}

void loop() {
  
  delay(2000); // Aguarda 2 segundos no inicio do ciclo de leitura dos sensor

  if (digitalRead(pinoBotaoLcd) == HIGH) {
    lcd.backlight();
  } else {
    lcd.noBacklight();
  }
  lcd.clear();
  lcd.print("     SII JF    ");
  lcd.setCursor(0, 0);
  // Posiciona o cursor do LCD na coluna 0 linha 1
  lcd.setCursor(0, 1);
  // Exibe a mensagem no Display LCD:
  lcd.print("  Umidade: ");
  // Faz a leitura do sensor de umidade do solo
  umidadeSolo = analogRead(pinoSensor);
  umidadeSolo2 = analogRead(pinoSensor2);
  umidadeSolo3 = analogRead(pinoSensor3);
  // Converte a variação do sensor de 0 a 1023 para 0 a 100
  umidadeSolo = map(umidadeSolo, 1023, 0, 0, 100);
  umidadeSolo2 = map(umidadeSolo2, 1023, 0, 0, 100);
  umidadeSolo3 = map(umidadeSolo3, 1023, 0, 0, 100);

  // média dos valores extraidos
  mediaUmidade = (umidadeSolo+umidadeSolo2+umidadeSolo3)/3;
  // Exibe a mensagem no Display LCD:
  lcd.print(mediaUmidade);
  lcd.print("%");
  
  // Mantem resultado na tela por alguns segundos
  delay(3000);

  // Verifica se precisa regar as plantas
  if(mediaUmidade < limiarSeco) {
    // Posiciona o cursor do LCD na coluna 0 linha 1
    lcd.setCursor(0, 1);

    // envia um sinal para o raspberry - Regador Acionado
    Serial.println("RA");
    
    // Exibe a mensagem no Display LCD:
    lcd.print("    Regando     ");
    // Liga a válvula
    digitalWrite(pinoRele, HIGH);
    digitalWrite(pinoValvula, HIGH);
    // Espera o tempo estipulado de 10s regando
    delay(tempoRega*1000);
    // Desliga a valvula
    digitalWrite(pinoRele, LOW);
    digitalWrite(pinoValvula, LOW);
    
    lcd.setCursor(0, 1);
    // Exibe a mensagem no Display LCD:
    lcd.print("    Aguarde     ");
  }
  // Senão precisar, segue em frente
  else {
    // Exibe a mensagem no Display LCD:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Solo Umido");

    Serial.println(mediaUmidade);
  }
}
