#define BLYNK_TEMPLATE_ID "TMPL6Ie709TA-"
#define BLYNK_TEMPLATE_NAME "Hostel Living Room"
#define BLYNK_AUTH_TOKEN "Q3miNq258KlOO5fYmxjoRuNKgwcBLgxu"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <EEPROM.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>

// WiFi credentials
char ssid[] = "moye moye";
char pass[] = "77777777";

// Relay and Switch pin definitions
#define RelayPin1 D5
#define RelayPin2 D6
#define RelayPin3 D7
#define RelayPin4 D2
#define RelayPin5 D1

#define SwitchPin1 D3
#define SwitchPin2 D4
#define SwitchPin3 10  // Changed from D4 to D8
#define SwitchPin4 D0
#define SwitchPin5 D9

// Initial toggle states
int toggleState_1;
int toggleState_2;
int toggleState_3;
int toggleState_4;
int toggleState_5;

// Sinric Pro credentials
#define APP_KEY "ee2639d5-b554-431b-842d-59d4b6edcff9"
#define APP_SECRET "4c474a22-2110-452b-a8f7-54aca19232a4-e49786ab-903c-4c92-aa00-56d2faffc307"

#define SWITCH_ID_1 "66abe47f6e1af3593521707c"
#define SWITCH_ID_2 "66abe58d5d818a66fad58871"
#define SWITCH_ID_3 "66abe5cc888aa7f7a2547c1d"

SinricProSwitch &mySwitch1 = SinricPro[SWITCH_ID_1];
SinricProSwitch &mySwitch2 = SinricPro[SWITCH_ID_2];
SinricProSwitch &mySwitch3 = SinricPro[SWITCH_ID_3];

// Function to read saved states from EEPROM
void readEEPROM() {
  toggleState_1 = EEPROM.read(0);
  toggleState_2 = EEPROM.read(1);
  toggleState_3 = EEPROM.read(2);
  toggleState_4 = EEPROM.read(3);
  toggleState_5 = EEPROM.read(4);
}

// Function to save states to EEPROM
void saveToEEPROM() {
  EEPROM.write(0, toggleState_1);
  EEPROM.write(1, toggleState_2);
  EEPROM.write(2, toggleState_3);
  EEPROM.write(3, toggleState_4);
  EEPROM.write(4, toggleState_5);
  EEPROM.commit();
}

// Blynk virtual pin control for relays
BLYNK_WRITE(V1) {
  int value = param.asInt();
  toggleState_1 = value == 1 ? 0 : 1;
  digitalWrite(RelayPin1, value == 1 ? LOW : HIGH);
  mySwitch1.sendPowerStateEvent(value == 1);  // Notify SinricPro
  saveToEEPROM();  // Save state to EEPROM
}

BLYNK_WRITE(V2) {
  int value = param.asInt();
  toggleState_2 = value == 1 ? 0 : 1;
  digitalWrite(RelayPin2, value == 1 ? LOW : HIGH);
  mySwitch1.sendPowerStateEvent(value == 1);  // Notify SinricPro
  saveToEEPROM();  // Save state to EEPROM
}

BLYNK_WRITE(V3) {
  int value = param.asInt();
  toggleState_3 = value == 1 ? 0 : 1;
  digitalWrite(RelayPin3, value == 1 ? LOW : HIGH);
  mySwitch1.sendPowerStateEvent(value == 1);  // Notify SinricPro
  saveToEEPROM();  // Save state to EEPROM
}

BLYNK_WRITE(V4) {
  int value = param.asInt();
  toggleState_4 = value == 1 ? 0 : 1;
  digitalWrite(RelayPin4, value == 1 ? LOW : HIGH);
  mySwitch2.sendPowerStateEvent(value == 1);  // Notify SinricPro
  saveToEEPROM();  // Save state to EEPROM
}

BLYNK_WRITE(V5) {
  int value = param.asInt();
  toggleState_5 = value == 1 ? 0 : 1;
  digitalWrite(RelayPin5, value == 1 ? LOW : HIGH);
  mySwitch3.sendPowerStateEvent(value == 1);  // Notify SinricPro
  saveToEEPROM();  // Save state to EEPROM
}

// Sinric Pro event handlers for relays
bool onPowerState1(const String &deviceId, bool &state) {
  toggleState_1 = state ? 0 : 1;
  digitalWrite(RelayPin1, state ? LOW : HIGH);
  Blynk.virtualWrite(V1, state ? 1 : 0);  // Notify Blynk
  digitalWrite(RelayPin2, state ? LOW : HIGH);
  Blynk.virtualWrite(V2, state ? 1 : 0);  // Notify Blynk
  digitalWrite(RelayPin3, state ? LOW : HIGH);
  Blynk.virtualWrite(V3, state ? 1 : 0);  // Notify Blynk
  saveToEEPROM();  // Save state to EEPROM
  return true;
}

bool onPowerState2(const String &deviceId, bool &state) {
  toggleState_4 = state ? 0 : 1;
  digitalWrite(RelayPin4, state ? LOW : HIGH);
  Blynk.virtualWrite(V4, state ? 1 : 0);  // Notify Blynk
  saveToEEPROM();  // Save state to EEPROM
  return true;
}

bool onPowerState3(const String &deviceId, bool &state) {
  toggleState_5 = state ? 0 : 1;
  digitalWrite(RelayPin5, state ? LOW : HIGH);
  Blynk.virtualWrite(V5, state ? 1 : 0);  // Notify Blynk
  saveToEEPROM();  // Save state to EEPROM
  return true;
}

void setup() {
  // Serial and Blynk initialization
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Initialize EEPROM
  EEPROM.begin(512);

  // Read the previous states from EEPROM
  readEEPROM();

  // Initialize relay pins and set their initial states
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(RelayPin5, OUTPUT);
  
  digitalWrite(RelayPin1, toggleState_1 ? HIGH : LOW);
  digitalWrite(RelayPin2, toggleState_2 ? HIGH : LOW);
  digitalWrite(RelayPin3, toggleState_3 ? HIGH : LOW);
  digitalWrite(RelayPin4, toggleState_4 ? HIGH : LOW);
  digitalWrite(RelayPin5, toggleState_5 ? HIGH : LOW);

  // Initialize switch pins with internal pull-ups
  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);
  pinMode(SwitchPin5, INPUT_PULLUP);

  // Attach SinricPro event handlers
  mySwitch1.onPowerState(onPowerState1);
  mySwitch2.onPowerState(onPowerState2);
  mySwitch3.onPowerState(onPowerState3);

  // Initialize SinricPro
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void loop() {
  Blynk.run();
  SinricPro.handle();
  checkSwitches();
}

// Function to check and debounce switches
void checkSwitches() {
  if (digitalRead(SwitchPin1) == LOW) {
    toggleRelay(1);
    while(digitalRead(SwitchPin1) == LOW); // Wait for button release
  }
  if (digitalRead(SwitchPin2) == LOW) {
    toggleRelay(2);
    while(digitalRead(SwitchPin2) == LOW); // Wait for button release
  }
  if (digitalRead(SwitchPin3) == LOW) {
    toggleRelay(3);
    while(digitalRead(SwitchPin3) == LOW); // Wait for button release
  }
  if (digitalRead(SwitchPin4) == LOW) {
    toggleRelay(4);
    while(digitalRead(SwitchPin4) == LOW); // Wait for button release
  }
  if (digitalRead(SwitchPin5) == LOW) {
    toggleRelay(5);
    while(digitalRead(SwitchPin5) == LOW); // Wait for button release
  }
}

// Function to toggle relays based on switch input
void toggleRelay(int relay) {
  switch (relay) {
    case 1:
      toggleState_1 = !toggleState_1;
      digitalWrite(RelayPin1, toggleState_1 ? HIGH : LOW);
      Blynk.virtualWrite(V1, toggleState_1 ? 0 : 1);
      mySwitch1.sendPowerStateEvent(toggleState_1 ? false : true);
      break;
    case 2:
      toggleState_2 = !toggleState_2;
      digitalWrite(RelayPin2, toggleState_2 ? HIGH : LOW);
      Blynk.virtualWrite(V2, toggleState_2 ? 0 : 1);
      mySwitch1.sendPowerStateEvent(toggleState_2 ? false : true);
      break;
    case 3:
      toggleState_3 = !toggleState_3;
      digitalWrite(RelayPin3, toggleState_3 ? HIGH : LOW);
      Blynk.virtualWrite(V3, toggleState_3 ? 0 : 1);
      mySwitch1.sendPowerStateEvent(toggleState_3 ? false : true);
      break;
    case 4:
      toggleState_4 = !toggleState_4;
      digitalWrite(RelayPin4, toggleState_4 ? HIGH : LOW);
      Blynk.virtualWrite(V4, toggleState_4 ? 0 : 1);
      mySwitch2.sendPowerStateEvent(toggleState_4 ? false : true);
      break;
    case 5:
      toggleState_5 = !toggleState_5;
      digitalWrite(RelayPin5, toggleState_5 ? HIGH : LOW);
      Blynk.virtualWrite(V5, toggleState_5 ? 0 : 1);
      mySwitch3.sendPowerStateEvent(toggleState_5 ? false : true);
      break;
  }
  saveToEEPROM();  // Save state to EEPROM after each toggle
}
