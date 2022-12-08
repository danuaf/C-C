#include <FS.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <led.h>

WiFiManager wifiManager;

#define BLYNK_TEMPLATE_ID "TMPLdTBpAnq0"
#define BLYNK_DEVICE_NAME "CNC"

#define TRIGGER_PIN 16
#define RELAY_PIN_1 5
#define RELAY_PIN_2 4
#define RELAY_PIN_3 12
#define RELAY_PIN_4 13
#define VPIN_BUTTON_1 V1
#define VPIN_BUTTON_2 V2
#define VPIN_BUTTON_3 V3
#define VPIN_BUTTON_4 V4

led LED;

// int LED[3] = {15, 14, 0}; // blue, green, red
int relay1State = LOW,
    relay2State = LOW,
    relay3State = LOW,
    relay4State = LOW;

char blynk_token[34] = "";
bool shouldSaveConfig = false;

void saveConfigCallback () {
    Serial.println(F("Should save config"));
    shouldSaveConfig = true;
}

void initSPIFFS() {
    Serial.println(F("mounting FS..."));
    if (SPIFFS.begin()) {
        Serial.println(F("mounted file system"));
        if (SPIFFS.exists("/token.txt")) {
            Serial.println(F("reading config file"));
            File configFile = SPIFFS.open("/token.txt", "r");
            if (configFile) {
                Serial.print(F("opened config file"));
                while (configFile.available()){
                    String line = configFile.readStringUntil('\n');
                    strcpy(blynk_token, line.c_str());
                    Serial.println(F(" success"));
                }
            } else {
                Serial.println(F("failed to load token"));
            }
            configFile.close();
        }
    } else {
        Serial.println(F("failed to mount FS"));
    }
}

void saveToken() {
    Serial.println("saving config");
    File configFile = SPIFFS.open("/token.txt", "w");
    if (!configFile){
        Serial.println("file creation failed");
    } else {
        for (int i = 1; i <= 8; i++) {
            LED.led_biru(0);
            // digitalWrite(LED[0], LOW);
            // digitalWrite(LED[1], HIGH);
            // digitalWrite(LED[2], HIGH);
            delay(80);
            LED.led_biru(1);
            // digitalWrite(LED[0], HIGH);
            // digitalWrite(LED[1], HIGH);
            // digitalWrite(LED[2], HIGH);
            delay(80);
            LED.led_biru(0);
            // digitalWrite(LED[0], LOW);
            // digitalWrite(LED[1], HIGH);
            // digitalWrite(LED[2], HIGH);
            delay(80);
            LED.led_biru(1);
            // digitalWrite(LED[0], HIGH);
            // digitalWrite(LED[1], HIGH);
            // digitalWrite(LED[2], HIGH);
            delay(80);
        }
        Serial.println("File Created!");
        configFile.println(blynk_token);
    }
    configFile.close();
}

void initialize() {
    digitalWrite(LED[0], LOW);
    digitalWrite(LED[1], HIGH);
    digitalWrite(LED[2], HIGH);

    initSPIFFS();

    WiFiManagerParameter custom_blynk_token("blynk", "Token Blynk", blynk_token, 34);
    wifiManager.setSaveConfigCallback(saveConfigCallback);
    wifiManager.addParameter(&custom_blynk_token);

    if (!wifiManager.autoConnect("C&C", "12345678")) {
        Serial.println("failed to connect and hit timeout");
        delay(3000);
        ESP.reset();
        delay(5000);
    }

    Serial.println("connected");
    strcpy(blynk_token, custom_blynk_token.getValue());

    if (shouldSaveConfig) {
        saveToken();
    }
}


void checkButton() {
    if (digitalRead(TRIGGER_PIN) == 1 ) {
        delay(50);
        if ( digitalRead(TRIGGER_PIN) == 1 ) {
            Serial.println("Button Pressed");
            delay(1000);
            if ( digitalRead(TRIGGER_PIN) == 1 ) {
                for (int i = 1; i <= 4; i++) {
                    LED.led_biru(0);
                    // digitalWrite(LED[0], LOW);
                    // digitalWrite(LED[1], HIGH);
                    // digitalWrite(LED[2], HIGH);
                    delay(70);
                    LED.led_biru(1);
                    // digitalWrite(LED[0], HIGH);
                    // digitalWrite(LED[1], HIGH);
                    // digitalWrite(LED[2], HIGH);
                    delay(70);
                    LED.led_biru(0);
                    // digitalWrite(LED[0], LOW);
                    // digitalWrite(LED[1], HIGH);
                    // digitalWrite(LED[2], HIGH);
                    delay(70);
                    LED.led_biru(1);
                    // digitalWrite(LED[0], HIGH);
                    // digitalWrite(LED[1], HIGH);
                    // digitalWrite(LED[2], HIGH);
                    delay(500);
                }   
                // digitalWrite(LED[0], LOW);
                // digitalWrite(LED[1], HIGH);
                // digitalWrite(LED[2], HIGH);
                Serial.println("Button Held");
                Serial.println("Erasing Config, restarting");
                wifiManager.resetSettings();
            }
        }
    }
    else {
        // digitalWrite(LED[0], HIGH);
        // digitalWrite(LED[1], LOW);
        // digitalWrite(LED[2], HIGH);
        LED.led_merah(0);
        Serial.println("Connecting");
        Blynk.begin(blynk_token, WiFi.SSID().c_str(), WiFi.psk().c_str(), "blynk.cloud", 80);
        Serial.println("Connected");
        LED.led_hijau(0);
        // digitalWrite(LED[0], HIGH);
        // digitalWrite(LED[1], HIGH);
        // digitalWrite(LED[2], LOW);
    }
}


BLYNK_CONNECTED() {
    Blynk.syncVirtual(VPIN_BUTTON_1);
    Blynk.syncVirtual(VPIN_BUTTON_2);
    Blynk.syncVirtual(VPIN_BUTTON_3);
    Blynk.syncVirtual(VPIN_BUTTON_4);
}

BLYNK_WRITE(VPIN_BUTTON_1) {
    relay1State = param.asInt();
    digitalWrite(RELAY_PIN_1, relay1State);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
    relay2State = param.asInt();
    digitalWrite(RELAY_PIN_2, relay2State);
}
BLYNK_WRITE(VPIN_BUTTON_3) {
    relay3State = param.asInt();
    digitalWrite(RELAY_PIN_3, relay3State);
}
BLYNK_WRITE(VPIN_BUTTON_4) {
    relay4State = param.asInt();
    digitalWrite(RELAY_PIN_4, relay4State);
}


void setup() {
    Serial.begin(115200);
    Serial.println("Booting");

    // for (int i = 0; i <= 2; i++) {
    //     pinMode(LED[i], OUTPUT);
    // }

    pinMode(TRIGGER_PIN, INPUT);
    pinMode(RELAY_PIN_1, OUTPUT);
    pinMode(RELAY_PIN_2, OUTPUT);
    pinMode(RELAY_PIN_3, OUTPUT);
    pinMode(RELAY_PIN_4, OUTPUT);

    digitalWrite(RELAY_PIN_1, relay1State);
    digitalWrite(RELAY_PIN_2, relay2State);
    digitalWrite(RELAY_PIN_3, relay3State);
    digitalWrite(RELAY_PIN_4, relay4State);

    initialize();
    checkButton();
}

void loop() {
    Blynk.run();
}