/*
 * MicroCoaster - Module Lift Hill ESP32
 *
 * Montée du train : entraînement par chaîne, contrôle de vitesse, détection
 * d'arrivée en crête et anti-retour.
 *
 * Auteurs: Cybertrist, Yamakajump
 * Version: 0.1.0
 */

#include <Arduino.h>          // Bibliothèque principale Arduino pour ESP32
#include <AyresWiFiManager.h> // Gestionnaire WiFi avec portail captif
#include <WebSocketsClient.h> // Client WebSocket pour communication serveur
#include <ArduinoJson.h>      // Manipulation des données JSON

// ========================================
// CONFIGURATION MATERIELLE
// ========================================

// Moteur d'entraînement de la chaîne, via pont en H.
#define MOTOR_PWM_PIN 25
#define MOTOR_DIR_PIN 26
#define MOTOR_ENABLE_PIN 27

// Capteur à effet Hall sur l'arbre : une impulsion par tour. Sert à la fois à
// mesurer la vitesse et à détecter un blocage de chaîne.
#define HALL_SENSOR_PIN 34

// Présence du train en pied de montée et en crête.
#define SENSOR_BOTTOM_PIN 35
#define SENSOR_CREST_PIN 32

// Signalisation d'état.
#define LED_RUNNING_PIN 2
#define LED_FAULT_PIN 4

// ========================================
// PARAMETRES DE MONTEE
// ========================================

// Consigne de vitesse par défaut, en pourcentage du rapport cyclique.
static const uint8_t LIFT_SPEED_PERCENT = 60;

// Rampe d'accélération et de décélération, en millisecondes. Un démarrage sec
// fait claquer la chaîne et secoue le train.
static const uint16_t RAMP_MS = 500;

// Délai sans impulsion Hall au-delà duquel on considère la chaîne bloquée.
static const uint16_t STALL_TIMEOUT_MS = 800;

// Durée maximale d'une montée. Dépassée, c'est qu'il y a un problème.
static const uint16_t LIFT_TIMEOUT_MS = 20000;

// ========================================
// ETATS
// ========================================

enum LiftState {
  STATE_IDLE,     // Chaîne à l'arrêt, aucun train
  STATE_ENGAGED,  // Train en pied de montée, prêt à monter
  STATE_LIFTING,  // Montée en cours
  STATE_CREST,    // Train arrivé en crête, chaîne relâchée
  STATE_FAULT     // Chaîne bloquée, survitesse ou délai dépassé
};

static LiftState state = STATE_IDLE;

// ========================================
// A IMPLEMENTER
// ========================================
//
// setup()  : sorties moteur en état sûr, interruption sur le capteur Hall,
//            portail WiFi, puis connexion WebSocket au serveur.
//
// loop()   : machine à états, asservissement de vitesse, surveillance du
//            blocage, et remontée de télémétrie périodique.
//
// Deux règles de sécurité. L'anti-retour est mécanique, pas logiciel : le
// firmware le surveille mais ne le remplace pas. Et une perte de liaison
// WebSocket pendant une montée ne coupe pas le moteur, elle la termine : un
// train arrêté en pleine pente redescend.

void setup() {
  Serial.begin(115200);
}

void loop() {
}
