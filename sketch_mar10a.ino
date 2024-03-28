#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>
#include "ValveController.h"

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

#define CAN0_INT 21 // Set INT to pin 21
MCP_CAN CAN0(5); // Set CS to pin 5
ValveController valve(25, 33); // Initialize valve controller

#define BUTTON_PIN 32  // Definicja pinu przycisku
#define LED_PIN 22     // Definicja pinu LED

volatile bool auto_mode = true; // Teraz tylko sygnalizuje tryb auto
int modeStep = 0; // 0: auto, 1: otwarte, 2: zamknięte
volatile unsigned long lastDebounceTime = 0; // Czas ostatniego drgania
const unsigned long debounceDelay = 50; // Opóźnienie debounce w ms


bool coldStartDetected = false;
unsigned long coldStartTime = 0;
const unsigned long coldStartDuration = 10000 ; // Duration of "cold start" in milliseconds (300 ms for testing)

volatile unsigned int sharedRpmValue = 0; // Shared variable for RPM value
volatile bool isRpmValueUpdated = false; // Flag to indicate new RPM value is available
volatile bool valveIsOpen = false; // Flag to indicate valve state
bool coldStartTimerActive = false;
unsigned long coldStartTimerStart;
// Task prototypes
void TaskCANListener(void *pvParameters);
void TaskValveController(void *pvParameters);
void TaskSerialHandler(void *pvParameters);

enum ButtonPressType {
    SHORT_PRESS,
    LONG_PRESS,
    NO_PRESS
};

volatile ButtonPressType buttonPressType = NO_PRESS;
void setup() {
    Serial.begin(115200);
    while (!Serial); // for boards that need to wait to ensure Serial is up
    // Initialize CAN
    if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
    else Serial.println("Error Initializing MCP2515...");
    CAN0.setMode(MCP_NORMAL); // Set operation mode to normal so the MCP2515 sends acks to received data.
    pinMode(CAN0_INT, INPUT); // Configuring pin for /INT input
    pinMode(LED_PIN, OUTPUT);
    // Create tasks for CAN listener, valve controller, and serial handler
    xTaskCreatePinnedToCore(TaskCANListener, "TaskCANListener", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(TaskValveController, "TaskValveController", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(TaskModeController, "TaskModeController", 2048, NULL, 1, NULL, 0); // Pinned to core 0
    xTaskCreatePinnedToCore(TaskButtonHandler, "TaskButtonHandler", 2048, NULL, 1, NULL, 1);
}

void loop() {
    vTaskDelay(portMAX_DELAY); // Infinite delay
}





void checkColdStartTimer() {
    if (coldStartTimerActive && (millis() - coldStartTimerStart > coldStartDuration)) {
        // Timer has expired
        coldStartTimerActive = false;
        // Here, you could close the valve or perform other actions indicating the end of the cold start
        if (valveIsOpen) {
            valve.close();
            valveIsOpen = false;
            Serial.println("Cold start period has ended. Valve closed.");
        }
        
    }


}

void manageColdStart() {
    if (isRpmValueUpdated) {
        isRpmValueUpdated = false;
        unsigned int rpm_value = sharedRpmValue;

        if (rpm_value == 0 && valveIsOpen) {
            valve.close();
            valveIsOpen = false;
            coldStartDetected = false; // Reset cold start
            Serial.println("RPM dropped to 0. Valve closed and cold start reset.");
        } else if (rpm_value > 0 && !coldStartDetected) {
            valve.open();
            valveIsOpen = true;
            coldStartDetected = true;
            startColdStartTimer(); // Start or reset the cold start timer
            Serial.println("Cold start detected. Valve opened.");
        }
    }

    checkColdStartTimer(); // Check the timer outside of the RPM update check to ensure it's always evaluated
}




void startColdStartTimer() {
    coldStartTimerStart = millis();
    coldStartTimerActive = true;
}



void TaskButtonHandler(void *pvParameters) {
    const unsigned long longPressTime = 1000; // Czas w ms uznawany za długie naciśnięcie
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    int lastButtonState = HIGH;
    unsigned long buttonPressedTime = 0;

    for (;;) {
        int currentButtonState = digitalRead(BUTTON_PIN);
        // Rozpoczęcie naciśnięcia przycisku
        if (currentButtonState == LOW && lastButtonState == HIGH) {
            buttonPressedTime = millis();
        }
        // Zwolnienie przycisku
        if (currentButtonState == HIGH && lastButtonState == LOW) {
            unsigned long pressDuration = millis() - buttonPressedTime;
            if (pressDuration >= longPressTime) {
                buttonPressType = LONG_PRESS;
            } else {
                buttonPressType = SHORT_PRESS;
            }
        }
        lastButtonState = currentButtonState;
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void TaskModeController(void *pvParameters) {
    for (;;) {
        switch (buttonPressType) {
            case LONG_PRESS:
                // Logika dla długiego naciśnięcia
                Serial.println("Switching to auto mode...");
               
                for (int i = 0; i < 3; i++) {
                    digitalWrite(LED_PIN, HIGH);
                    vTaskDelay(200 / portTICK_PERIOD_MS);
                    digitalWrite(LED_PIN, LOW);
                    vTaskDelay(200 / portTICK_PERIOD_MS);
                }
                auto_mode = true;
                buttonPressType = NO_PRESS; // Resetujemy stan po obsłudze
                break;
            case SHORT_PRESS:
                // Logika dla krótkiego naciśnięcia
                if (auto_mode) {
                    Serial.println("Manual mode - toggling valve state...");
                    // Tutaj logika przełączania stanu zaworu
                    if (valveIsOpen) {
                        valve.close();
                        valveIsOpen = false;
                        digitalWrite(LED_PIN, LOW); // LED wyłączony dla zamkniętego
                    } else {
                        valve.open();
                        valveIsOpen = true;
                        digitalWrite(LED_PIN, HIGH); // LED włączony dla otwartego
                    }
                } else {
                    // Jeśli nie jesteśmy w trybie auto, możemy chcieć zrobić coś innego lub nic nie robić
                }
                buttonPressType = NO_PRESS; // Resetujemy stan po obsłudze
                break;
            default:
                // Brak akcji
                break;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
void TaskValveController(void *pvParameters) {
    for (;;) {
        if (auto_mode) {
            if (!coldStartDetected) {
                manageColdStart(); // Handle the cold start logic
                vTaskDelay(3500 / portTICK_PERIOD_MS);
            } else {
                unsigned int rpm_value = sharedRpmValue;

                if (rpm_value < 3200 && valveIsOpen) {
                    valve.close();
                     digitalWrite(LED_PIN,LOW);
                    valveIsOpen = false;
                    Serial.println("RPM dropped below 3200. Valve closed.");
                } else if (rpm_value >= 3200 && !valveIsOpen) {
                    valve.open();
                    digitalWrite(LED_PIN, HIGH);
                    valveIsOpen = true;
                    Serial.println("RPM reached or exceeded 3200. Valve opened.");
                }
            }
        } 
        
        vTaskDelay(10 / portTICK_PERIOD_MS); // Reduce CPU load
    }
}



void TaskCANListener(void *pvParameters) {
    for (;;) {
        
        
            if (!digitalRead(CAN0_INT)) {
                CAN0.readMsgBuf(&rxId, &len, rxBuf);
                if (rxId == 0x110) {
                    unsigned int high_byte = rxBuf[1];
                    unsigned int low_byte = rxBuf[2];
                    unsigned int rpm_value = (high_byte << 8) | low_byte;
                    rpm_value /= 4; // Adjust RPM value
                    
                    sharedRpmValue = rpm_value;
                    isRpmValueUpdated = true;
                    
                    // Jeśli RPM równa się 0, resetuj coldStartDetected
                    if(rpm_value == 0) {
                        coldStartDetected = false;
                        Serial.println("RPM is 0. Cold start detection reset.");
                    }
                }
            }
        
        
        vTaskDelay(1 / portTICK_PERIOD_MS); // Odpoczynek dla CPU, redukcja obciążenia
    }}











