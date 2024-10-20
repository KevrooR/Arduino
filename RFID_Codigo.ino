#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definição de pinos para o MFRC522
#define RST_PIN         9  // Pino reset do MFRC522
#define SS_PIN          10 // Pino slave select do MFRC522

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Instância do MFRC522

// Configuração do LCD I2C. O endereço é 0x27 para o seu módulo.
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Endereço I2C, LCD 16x2

void setup() {
  Serial.begin(9600);        // Inicializa comunicação serial
  SPI.begin();               // Inicializa barramento SPI
  mfrc522.PCD_Init();        // Inicializa o MFRC522
  lcd.begin(16, 2);          // Inicializa o LCD 16x2 via I2C
  lcd.backlight();           // Ativa a luz de fundo do LCD
  lcd.print("Aproxime o");   // Mensagem inicial no LCD
  lcd.setCursor(0, 1);
  lcd.print("cartao...");
  delay(2000);
  lcd.clear();               // Limpa o LCD
}

void loop() {

  // Prepara a chave - Todas as chaves são definidas como FFFFFFFFFFFFh na entrega de fábrica
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  // Reseta o loop se não houver um novo cartão
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Seleciona um dos cartões
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Armazena a mensagem a ser exibida no LCD
  String mensagem = "";

  // Verifica se o UID corresponde ao primeiro cartão (0D 33 F9 30)
  if (mfrc522.uid.size == 4 && 
      mfrc522.uid.uidByte[0] == 0x0D &&
      mfrc522.uid.uidByte[1] == 0x33 &&
      mfrc522.uid.uidByte[2] == 0xF9 &&
      mfrc522.uid.uidByte[3] == 0x30) {
    mensagem = "Chave secreta";
  }

  // Verifica se o UID corresponde ao segundo cartão (95 83 8C AC)
  else if (mfrc522.uid.size == 4 && 
           mfrc522.uid.uidByte[0] == 0x95 &&
           mfrc522.uid.uidByte[1] == 0x83 &&
           mfrc522.uid.uidByte[2] == 0x8C &&
           mfrc522.uid.uidByte[3] == 0xAC) {
    mensagem = "Chave secreta 2";
  }

  // Caso o cartão lido não corresponda a nenhum dos IDs conhecidos
  else {
    mensagem = "Cartao desconhecido!";
  }

  // Exibe a mensagem no LCD e realiza o scroll se necessário
  exibirComScroll(mensagem);

  // Pausa antes de permitir uma nova leitura
  delay(2000);

  // Interrompe o cartão e a criptografia
  mfrc522.PICC_HaltA(); // Interrompe o cartão
  mfrc522.PCD_StopCrypto1();  // Interrompe a criptografia
}

void exibirComScroll(String mensagem) {
  lcd.clear();
  if (mensagem.length() <= 16) {
    // Se a mensagem tiver 16 caracteres ou menos, exibe diretamente
    lcd.print(mensagem);
  } else {
    // Se a mensagem for maior que 16 caracteres, exibe com scroll
    for (int i = 0; i < mensagem.length() - 15; i++) {
      lcd.setCursor(0, 0);
      lcd.print(mensagem.substring(i, i + 16));  // Exibe os próximos 16 caracteres
      delay(500);  // Ajuste o delay para a velocidade do scroll
    }
  }
}
