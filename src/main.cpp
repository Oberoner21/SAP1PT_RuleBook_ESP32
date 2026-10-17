/*
    Simulation des SAP1PT-Rulebooks
    Notepad, EPROM mit SAP1PT-Tape und serielle Ausgabe sind
    als eigenständige Hardware an die GPIO-Pins des ESP32 über
    Pegelschifter 3,3V auf 5V angeschlossen
    
    Microcontroller: ESP32 Devkit V1

    Copyright (C) Oktober 2026 by Oberoner
*/

#include <Arduino.h>
#include "symbols.h"
#include "notepad.h"
#include "rulebook.h"

#define USE_FSM     1   // 1 = Use external hardware notepad, 0 = Emulate the notepad

// ESP32 pin definition
const uint8_t OUTCLK_PIN        = 32;       // D32
const uint8_t DATA0_OUT_PIN     = 33;       // D33
const uint8_t DATA1_OUT_PIN     = 25;       // D25
const uint8_t DATA0_IN_PIN      = 14;       // D14
const uint8_t DATA1_IN_PIN      = 13;       // D13
const uint8_t DIRECTION_PIN     = 26;       // D26
const uint8_t CLKRAW_PIN        = 27;       // D27
const uint8_t RESET_PIN         = 15;       // D15


// Variables
uint8_t notePadPointer = 0;
uint8_t symbol;
uint16_t rule = 0;
boolean halt = false;
uint8_t rSymbol;
uint8_t lastRule;
int8_t direction;
uint16_t lastNotePadPointer;
long cykles = 0;
long cyklesBefore = 0;

void setup() {
    Serial.begin(115200);
    Serial.println();

    BuildNotepad();
    GenerateRuleBook(USE_FSM);

    pinMode(OUTCLK_PIN, OUTPUT);
    pinMode(DATA0_OUT_PIN, OUTPUT);
    pinMode(DATA1_OUT_PIN, OUTPUT);
    pinMode(DATA0_IN_PIN, INPUT);
    pinMode(DATA1_IN_PIN, INPUT);
    pinMode(DIRECTION_PIN, OUTPUT);

    digitalWrite(RESET_PIN, HIGH);
    pinMode(RESET_PIN, OUTPUT);

    digitalWrite(CLKRAW_PIN, LOW);
    pinMode(CLKRAW_PIN, OUTPUT);   

    // Reset the FSM
    digitalWrite(RESET_PIN, LOW);
    delayMicroseconds(100);
    digitalWrite(RESET_PIN, HIGH);
}

void loop() {

    delayMicroseconds(100);  // 10uS = 100kHz, 100uS = 10kHz, 1000uS = 1kHz

    if(!halt) {

        if(USE_FSM) {

            // Read the symbol from SRAM Notepad
            symbol = 0;
            if(digitalRead(DATA0_IN_PIN) == HIGH) symbol = symbol | 0x01;
            if(digitalRead(DATA1_IN_PIN) == HIGH) symbol = symbol | 0x02;
        
            // Get the next rule
            uint16_t nextRule = ruleBook[rule][symbol].next;

            // Send out the Writesymbol
            digitalWrite(DATA0_OUT_PIN, ruleBook[rule][symbol].writeSymbol & 0x01 ? HIGH : LOW);
            digitalWrite(DATA1_OUT_PIN, ruleBook[rule][symbol].writeSymbol & 0x02 ? HIGH : LOW);

            // Output clock to Shift register
            digitalWrite(OUTCLK_PIN, ruleBook[rule][symbol].clkOut);

            // Set the direction for next cykle
            digitalWrite(DIRECTION_PIN, ruleBook[rule][symbol].direction == RIGHT ? 0 : 1);
            direction = ruleBook[rule][symbol].direction;
            // For error debugging compute NPP

            notePadPointer += ruleBook[rule][symbol].direction;
                        lastNotePadPointer = notePadPointer;
            //Serial.print(cSymbols[ruleBook[rule][symbol].writeSymbol]);
            if(cykles < 256) Serial.print(cSymbols[symbol]);
            if(cykles == 256) Serial.println();

            lastRule = rule;
            // Make nextRule to the current rule in next cykle
            rule = nextRule;

            // Clock the external mashine
            digitalWrite(CLKRAW_PIN, HIGH);
            delayMicroseconds(10);
            digitalWrite(CLKRAW_PIN, LOW);

            cykles++;

            if(rule == RULE_ERROR) {

                rSymbol = 0;
                if(digitalRead(DATA0_IN_PIN) == HIGH) rSymbol = rSymbol | 0x01;
                if(digitalRead(DATA1_IN_PIN) == HIGH) rSymbol = rSymbol | 0x02;
                Serial.println();
                Serial.print("ERROR state in Cykle: ");
                Serial.println(cykles);
                Serial.print("NPP: 0x");
                Serial.print(lastNotePadPointer, HEX);
                Serial.print(" ");
                Serial.print(lastNotePadPointer);
                Serial.print("  RSymbol: ");
                Serial.print(cSymbols[rSymbol]);
                Serial.print("  LastRule: ");
                Serial.println(lastRule);

                halt = true;

            }

            if(rule == RULE_END) {

                rSymbol = 0;
                if(digitalRead(DATA0_IN_PIN) == HIGH) rSymbol = rSymbol | 0x01;
                if(digitalRead(DATA1_IN_PIN) == HIGH) rSymbol = rSymbol | 0x02;
                Serial.println();
                Serial.println("END state");
                Serial.print("NPP: 0x");
                Serial.print(lastNotePadPointer, HEX);
                Serial.print(" ");
                Serial.print(lastNotePadPointer);
                Serial.print("  RSymbol: ");
                Serial.print(cSymbols[rSymbol]);
                Serial.print("  LastRule: ");
                Serial.println(lastRule);

                halt = true;

            }

            if(rule == RULE_OUT) {

                Serial.println(cykles - cyklesBefore);
                cyklesBefore = cykles;
            }

        } else {

            cykles++;

            // Use internal Notepad
            uint8_t symbol = notePad[notePadPointer];
            uint16_t nextRule = ruleBook[rule][symbol].next;
            uint8_t wSymbol = ruleBook[rule][symbol].writeSymbol;
            notePad[notePadPointer] = wSymbol;

            // Send out the Writesymbol
            digitalWrite(DATA0_OUT_PIN, ruleBook[rule][symbol].writeSymbol & 0x01 ? HIGH : LOW);
            digitalWrite(DATA1_OUT_PIN, ruleBook[rule][symbol].writeSymbol & 0x02 ? HIGH : LOW);
            digitalWrite(OUTCLK_PIN, ruleBook[rule][symbol].clkOut);

            direction = ruleBook[rule][symbol].direction;
            notePadPointer += direction;
            rule = nextRule;

            if(rule == RULE_ERROR) {

                Serial.println();
                Serial.println("ERROR state");
                Serial.print("NPP: ");
                Serial.print(notePadPointer);
                Serial.print("  RSymbol: ");
                Serial.print(cSymbols[notePad[notePadPointer]]);
                Serial.print("  Rule: ");
                Serial.println(rule);

                halt = true;
            }

            if(rule == RULE_END) {

                Serial.println();
                Serial.println("END state");
                Serial.print("NPP: ");
                Serial.print(notePadPointer);
                Serial.print("  RSymbol: ");
                Serial.print(cSymbols[notePad[notePadPointer]]);
                Serial.print("  Rule: ");
                Serial.println(rule);

                halt = true;
            }

            if(rule == RULE_OUT) {

                Serial.println(cykles - cyklesBefore);
                cyklesBefore = cykles;
            }

        }
    }

}
