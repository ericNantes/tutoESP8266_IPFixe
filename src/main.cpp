#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "InfoConnexion.h"
#include "ArduinoOTA.h"

// Définition des entrées/sorties
#define PIN_LED_ROUGE 5
#define PIN_LED_VERTE 4

// Informations de connexion : cachées dans fichier InfoConnexion.h
// Vous pouvez décommenter ici ou créer comme moi un fichier InfoConnexion.h
//const char * SSID = "A définir";
//const char * PASSWORD = "A définir";

// Gestion des événements du WiFi
// Lorsque la connexion vient d'aboutir
void onConnected(const WiFiEventStationModeConnected& event);
// Lorsque l'adresse IP est attribuée
void onGotIP(const WiFiEventStationModeGotIP& event);

void setup() {
  IPAddress ip(192,168,0,201);
  IPAddress gateway(192,168,0,1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress dns(192, 168, 0, 1);

  // Mise en place d'une liaison série
  Serial.begin(9600L);
  delay(100);

  // Configuration des entrées/sorties
  pinMode(PIN_LED_ROUGE, OUTPUT);
  pinMode(PIN_LED_VERTE, OUTPUT);

  // Nom de l'objet OTA
  ArduinoOTA.setHostname("Objet LED");
  ArduinoOTA.setPassword("testtoto");

  // Mode de connexion
  WiFi.mode(WIFI_STA);

  // Mode point d'accès
  WiFi.softAP("Objet LED");
 
  // Configuration pour l'IP Fixe
  WiFi.config(ip, gateway, subnet, dns);

  // Démarrage de la connexion
  WiFi.begin(SSID, PASSWORD);

  static WiFiEventHandler onConnectedHandler = WiFi.onStationModeConnected(onConnected);
  static WiFiEventHandler onGotIPHandler = WiFi.onStationModeGotIP(onGotIP);
}

void loop() {
  // Si l'objet est connecté au réseau, on effectue les tâches qui doivent l'être dans ce cas
  if (WiFi.isConnected()) {
    digitalWrite(PIN_LED_ROUGE, HIGH);
    ArduinoOTA.begin();
  }
  else {
    digitalWrite(PIN_LED_ROUGE, LOW);
  }

  // ArduinoOTA...
  ArduinoOTA.handle();
}

void onConnected(const WiFiEventStationModeConnected& event) {
  Serial.println("WiFi connecté");
  Serial.println("Adresse IP : " + WiFi.localIP().toString());
}

void onGotIP(const WiFiEventStationModeGotIP& event) {
  Serial.println("Adresse IP : " + WiFi.localIP().toString());
  Serial.println("Passerelle IP : " + WiFi.gatewayIP().toString());
  Serial.println("DNS IP : " + WiFi.dnsIP().toString());
  Serial.print("Puissance de réception : ");
  Serial.println(WiFi.RSSI());
}
