const int ledPin = 8; // Define o pino do LED vermelho
const int ledPin2 = 9;
const int ledPin3 = 7;

void setup() {
  pinMode(ledPin, OUTPUT); // Configura o pino do LED como saída
  pinMode(ledPin2, OUTPUT); // Configura o segundo pino do LED como saída
  pinMode(ledPin3, OUTPUT); 
}

void loop() {
  digitalWrite(ledPin, HIGH); // Liga o LED vermelho 2
  delay(1000); // Espera 2 segundos
  digitalWrite(ledPin, LOW); // Desliga o LED vermelho 2
  digitalWrite(ledPin2, HIGH);
  delay(10000);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, HIGH);
  delay(2000);
  digitalWrite(ledPin3, LOW);
}
