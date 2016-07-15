#include <Thread.h>
#include <ThreadController.h>

#define ledVermelhoDianteiro 9
#define ledAmareloDianteiro 8
#define ledVerdeDianteiro 7

#define ledVermelhoTraseiro 6
#define ledAmareloTraseiro 5
#define ledVerdeTraseiro 4

#define sensorTraseiro 11
#define sensorDianteiro 10

#define buzzerGeral 2

Thread leituraDeSensorDianteiro;
Thread leituraDeSensorTraseiro;
Thread acendimentoDeLeds;
Thread emissorSomBuzzer;
Thread transmisorBluetooth;
Thread receptorBluetooth;
ThreadController mainThread;

boolean sistemaLigado = false;

float distanciaBaixoRisco = 100.00;
float distanciaAltoRisco = 30.00;
float distanciaMedioRisco = 50.00;

float distanciaDianteira = 999.00;
float distanciaTraseira = 999.00;

unsigned long millisAnterior = 0;


void lerSensorDeDistanciaTraseiro() {
  float duracaoSensorTraseiro;
  pinMode(sensorTraseiro, OUTPUT);
  digitalWrite(sensorTraseiro, LOW);
  delayMicroseconds(2);
  digitalWrite(sensorTraseiro, HIGH);
  delayMicroseconds(5);
  digitalWrite(sensorTraseiro, LOW);
  pinMode(sensorTraseiro, INPUT);
  duracaoSensorTraseiro = pulseIn(sensorTraseiro, HIGH);
  distanciaTraseira = duracaoSensorTraseiro / 29 / 2;


}

void lerSensorDeDistanciaDianteiro() {
  float duracaoSensorDianteiro;
  pinMode(sensorDianteiro, OUTPUT);
  digitalWrite(sensorDianteiro, LOW);
  delayMicroseconds(2);
  digitalWrite(sensorDianteiro, HIGH);
  delayMicroseconds(5);
  digitalWrite(sensorDianteiro, LOW);
  pinMode(sensorDianteiro, INPUT);
  duracaoSensorDianteiro = pulseIn(sensorDianteiro, HIGH);
  distanciaDianteira = duracaoSensorDianteiro / 29 / 2;

}

void configurarSistema() {
  String config = " ";
  if (Serial.available() > 0) {
    config = Serial.readString();
  }

  if (config == "l" && sistemaLigado) {
    toogleLeds();

  } else if (config == "s" && sistemaLigado) {
    toogleSom();

  } else if (config == "d" && sistemaLigado) {
    desligarSistema();
    sistemaLigado=false;
    
  } else if (config == "i") {
    ligarSistema();
    sistemaLigado=true;
  }
}

void ligarSistema() {
  
  distanciaDianteira = 999.00;
  distanciaTraseira = 999.00;
  
  acendimentoDeLeds.enabled = true;

  emissorSomBuzzer.enabled = true;

  leituraDeSensorDianteiro.enabled = true;

  leituraDeSensorTraseiro.enabled = true;
  transmisorBluetooth.enabled = true;
}

void toogleLeds() {
  if (acendimentoDeLeds.enabled) {
    acendimentoDeLeds.enabled = false;
    digitalWrite(ledVermelhoDianteiro, LOW);
    digitalWrite(ledAmareloDianteiro, LOW);
    digitalWrite(ledVerdeDianteiro, LOW);
    digitalWrite(ledVermelhoTraseiro, LOW);
    digitalWrite(ledAmareloTraseiro, LOW);
    digitalWrite(ledVerdeTraseiro, LOW);

  } else {
    acendimentoDeLeds.enabled = true;
  }

}

void toogleSom() {
  if (emissorSomBuzzer.enabled) {
    emissorSomBuzzer.enabled = false;
    digitalWrite(buzzerGeral, LOW);
    noTone(buzzerGeral);

    millisAnterior = 0;
  } else {
    emissorSomBuzzer.enabled = true;
  }
}

void desligarSistema() {

  acendimentoDeLeds.enabled = false;

  emissorSomBuzzer.enabled = false;

  leituraDeSensorDianteiro.enabled = false;

  leituraDeSensorTraseiro.enabled = false;

  transmisorBluetooth.enabled = false;

  digitalWrite(ledVermelhoDianteiro, LOW);
  digitalWrite(ledAmareloDianteiro, LOW);
  digitalWrite(ledVerdeDianteiro, LOW);
  digitalWrite(ledVermelhoTraseiro, LOW);
  digitalWrite(ledAmareloTraseiro, LOW);
  digitalWrite(ledVerdeTraseiro, LOW);

  distanciaDianteira = 999.00;
  distanciaTraseira = 999.00;

  digitalWrite(buzzerGeral, LOW);
  noTone(buzzerGeral);
  millisAnterior = 0;

  digitalWrite(sensorDianteiro, LOW);
  digitalWrite(sensorTraseiro, LOW);


}

void acenderLeds() {
  if (distanciaDianteira <= distanciaAltoRisco) {
    digitalWrite(ledVermelhoDianteiro, HIGH);
    digitalWrite(ledAmareloDianteiro, LOW);
    digitalWrite(ledVerdeDianteiro, LOW);
  } else if (distanciaDianteira <= distanciaMedioRisco) {
    digitalWrite(ledAmareloDianteiro, HIGH);
    digitalWrite(ledVermelhoDianteiro, LOW);
    digitalWrite(ledVerdeDianteiro, LOW);
  } else {
    digitalWrite(ledVermelhoDianteiro, LOW);
    digitalWrite(ledVerdeDianteiro, HIGH);
    digitalWrite(ledAmareloDianteiro, LOW);
  }

  if (distanciaTraseira <= distanciaAltoRisco) {
    digitalWrite(ledVermelhoTraseiro, HIGH);
    digitalWrite(ledAmareloTraseiro, LOW);
    digitalWrite(ledVerdeTraseiro, LOW);
  } else if (distanciaTraseira <= distanciaMedioRisco) {
    digitalWrite(ledAmareloTraseiro, HIGH);
    digitalWrite(ledVermelhoTraseiro, LOW);
    digitalWrite(ledVerdeTraseiro, LOW);
  } else {
    digitalWrite(ledVermelhoTraseiro, LOW);
    digitalWrite(ledVerdeTraseiro, HIGH);
    digitalWrite(ledAmareloTraseiro, LOW);
  }

}

void tocarBuzzer() {


  digitalWrite(buzzerGeral, LOW);
  if (distanciaTraseira <= distanciaAltoRisco
      || distanciaDianteira <= distanciaAltoRisco) {
    digitalWrite(buzzerGeral, HIGH);
  } else {
    digitalWrite(buzzerGeral, LOW);
    unsigned long millisAtuais = millis();



    if (millisAtuais - millisAnterior > 1000) {

      millisAnterior = millisAtuais;

      digitalWrite(buzzerGeral, HIGH);
    }


  }
}

void transmitirViaBluetooth() {

  Serial.print(distanciaDianteira);
  Serial.print('-');
  Serial.print(distanciaTraseira);
  Serial.print('#');
  
  


}

void setup() {
  Serial.begin(9600);
  pinMode(buzzerGeral, OUTPUT);
  pinMode(ledVerdeTraseiro, OUTPUT);
  pinMode(ledAmareloTraseiro, OUTPUT);
  pinMode(ledVermelhoTraseiro, OUTPUT);

  pinMode(ledVermelhoDianteiro, OUTPUT);
  pinMode(ledVerdeDianteiro, OUTPUT);
  pinMode(ledAmareloDianteiro, OUTPUT);

  leituraDeSensorTraseiro.setInterval(200);
  leituraDeSensorTraseiro.onRun(lerSensorDeDistanciaTraseiro);
  leituraDeSensorTraseiro.enabled = false;

  leituraDeSensorDianteiro.setInterval(200);
  leituraDeSensorDianteiro.onRun(lerSensorDeDistanciaDianteiro);
  leituraDeSensorDianteiro.enabled = false;

  acendimentoDeLeds.setInterval(100);
  acendimentoDeLeds.onRun(acenderLeds);
  acendimentoDeLeds.enabled = false;

  emissorSomBuzzer.setInterval(100);
  emissorSomBuzzer.onRun(tocarBuzzer);
  emissorSomBuzzer.enabled = false;

  transmisorBluetooth.setInterval(200);
  transmisorBluetooth.onRun(transmitirViaBluetooth);
  transmisorBluetooth.enabled = false;

  receptorBluetooth.setInterval(300);
  receptorBluetooth.onRun(configurarSistema);

  mainThread.add(&emissorSomBuzzer);
  mainThread.add(&leituraDeSensorTraseiro);
  mainThread.add(&leituraDeSensorDianteiro);
  mainThread.add(&acendimentoDeLeds);
  mainThread.add(&transmisorBluetooth);
  mainThread.add(&receptorBluetooth);

}

void loop() {
  mainThread.run();
}


