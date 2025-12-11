#include <Arduino.h>
#include "JoystickLib.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include "Internet.h"
#include "certificados.h"
#include <WiFiClientSecure.h>
#include <Bounce2.h>

#define pinA 5
#define pinB 6
#define pinC 7
#define pinD 15
#define pinE 16
#define pinF 17
#define pinK 18
#define pinX 8
#define pinY 3
#define PinM0Dir 18
#define PinM0Esq 3
#define PinM1Dir 10
#define PinM1Esq 46
#define PinM2Dir 13
#define PinM2Esq 14
#define PinM3Dir 12
#define PinM3Esq 11

#define FrequenciaPWM 20000
#define resolucaoPWM 8

const int NumBotoes = 7;

Joystick stick(pinX, pinY);
WiFiClientSecure espClient;
PubSubClient mqtt(espClient);
Bounce btn[NumBotoes];

const int mqtt_port = 8883;
const char *mqtt_id = "lizandra";
const char *mqtt_SUB = "NovaFuture/esp32/Enviar";
const char *mqtt_pub = "NovaFuture/esp32/Receber";

const int Botoes[] = {pinA, pinB, pinC, pinD, pinE, pinF, pinK};

bool estadoAnt[NumBotoes];
bool valorAtual[NumBotoes] = {false, false, false, false, false, false, false};

void conectaMQTT();

void setup()
{
  Serial.begin(9600);
  conectaWiFi();

  while (!Serial)
  {
  }

  for (int i = 0; i < NumBotoes; i++)
  {
    btn[i].attach(Botoes[i], INPUT_PULLUP);
  }

  stick.calibrate(); // calibra o joystick(as portas x e y)

  espClient.setCACert(AWS_ROOT_CA);
  espClient.setCertificate(AWS_CERT);
  espClient.setPrivateKey(AWS_KEY);

  mqtt.setServer(AWS_BROKER, mqtt_port);

  conectaMQTT();
}

void loop()
{

  stick.loop();
  //  btn2.update();
  //  btn3.update();
  //  btn4.update();
  //  btn5.update();
  //  btn6.update();
  //  btn7.update();

  JsonDocument doc;
  ReconectarWiFi();

  static unsigned long tempo_pub = 0;

  if (!mqtt.connected())
  {
    conectaMQTT();
  }

  mqtt.loop();

  for (int i = 0; i < NumBotoes; i++)
  {
    btn[i].update();

    if (btn[i].fell())
    {
      valorAtual[i] = !valorAtual[i];
      String chave = "Botao" + String(i);
      doc[chave] = valorAtual[i] ? 1 : 0;

      String mensagem;
      serializeJson(doc, mensagem);
      mqtt.publish(mqtt_pub, mensagem.c_str());
      Serial.println(mensagem);
    }
  }

  if (millis() - tempo_pub > 170)
  {
    if (stick.isUp())
    {
      Serial.println(F("Frente"));
      doc["Direcao"] = 1;

      String mensagem;
      serializeJson(doc, mensagem);

      mqtt.publish(mqtt_pub, mensagem.c_str());
    }

    if (stick.isRightUp())
    {
      Serial.println("Direita Frente");
      doc["Direcao"] = 2;

      String mensagem;
      serializeJson(doc, mensagem);

      mqtt.publish(mqtt_pub, mensagem.c_str());
    }

    if (stick.isRight())
    {
      Serial.println("Direita");
      doc["Direcao"] = 3;

      String mensagem;
      serializeJson(doc, mensagem);

      mqtt.publish(mqtt_pub, mensagem.c_str());
    }

    if (stick.isRightDown())
    {
      Serial.println("Direita aTras");
      doc["Direcao"] = 4;

      String mensagem;
      serializeJson(doc, mensagem);

      mqtt.publish(mqtt_pub, mensagem.c_str());
    }

    if (stick.isDown())
    {
      Serial.println("atras");
      doc["Direcao"] = 5;

      String mensagem;
      serializeJson(doc, mensagem);

      mqtt.publish(mqtt_pub, mensagem.c_str());
    }

    if (stick.isLeftDown())
    {
      Serial.println("esquerda Atras");
      doc["Direcao"] = 6;

      String mensagem;
      serializeJson(doc, mensagem);

      mqtt.publish(mqtt_pub, mensagem.c_str());
    }

    if (stick.isLeft())
    {
      Serial.println("Esquerda");
      doc["Direcao"] = 7;

      String mensagem;
      serializeJson(doc, mensagem);

      mqtt.publish(mqtt_pub, mensagem.c_str());
    }

    if (stick.isLeftUp())
    {
      Serial.println("Esquerda Frente");
      doc["Direcao"] = 8;

      String mensagem;
      serializeJson(doc, mensagem);

      mqtt.publish(mqtt_pub, mensagem.c_str());
    }

    if (stick.isCenter())
    {

      Serial.println("centro");
      doc["Direcao"] = 9;

      String mensagem;
      serializeJson(doc, mensagem);

      mqtt.publish(mqtt_pub, mensagem.c_str());
    }
    tempo_pub = millis();
  }
}

void conectaMQTT()
{
  while (!mqtt.connected())
  {
    Serial.print("conectando ao aws IoT core ...");
    if (mqtt.connect(mqtt_id))
    {
      Serial.println("Conectado");
      mqtt.subscribe(mqtt_SUB);
    }
    else
    {
      Serial.printf("falhou (%d). tentando novamente em 5s \n\r", mqtt.state());
      delay(3000);
    }
  }
}
