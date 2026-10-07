#pragma once
#include <Arduino.h>
#include "symbols.h"

#define     LEFT            -1
#define     RIGHT           1
#define     CLKOUT          1
#define     RULES           0x1000
#define     SYMBOLS         4

#define     RULE_RESET      0
#define     RULE_FETCH      20
#define     RULE_DECODE     200
#define     RULE_NOP        250
#define     RULE_LDA        300
#define     RULE_ADD        800
#define     RULE_SUB        1200
#define     RULE_STA        2000
#define     RULE_LDI        2300
#define     RULE_JMP        2800
#define     RULE_JC         3000
#define     RULE_JZ         3200
#define     RULE_OUT        3400
#define     RULE_HLT        3800

#define     RULE_END        4094
#define     RULE_ERROR      4095

uint16_t rule_ALU;

struct rule {
    uint16_t next;
    int8_t direction;
    uint8_t writeSymbol;
    uint8_t clkOut;
} ruleBook[RULES][SYMBOLS];


void SetRule(uint16_t rule, unsigned char readSymbol, 
    unsigned char writeSymbol, int8_t direction, uint16_t nextRule, uint8_t clkOut = 0){

    uint8_t numReadSymbol = symbol2num(readSymbol);

    ruleBook[rule][numReadSymbol].writeSymbol = symbol2num(writeSymbol);
    ruleBook[rule][numReadSymbol].direction = direction;
    ruleBook[rule][numReadSymbol].next = nextRule;
    ruleBook[rule][numReadSymbol].clkOut = clkOut;
}

uint16_t next_LEFT(uint16_t rule, int8_t endDirection = RIGHT){

    SetRule(rule, '0', '0', LEFT, rule);        
    SetRule(rule, '1', '1', LEFT, rule);
    SetRule(rule, '$', '$', LEFT, rule);
    SetRule(rule, '_', '_', endDirection, rule+1);

    return rule+1;
}

uint16_t next_RIGHT(uint16_t rule, int8_t endDirection = RIGHT){

    SetRule(rule, '0', '0', RIGHT, rule);        
    SetRule(rule, '1', '1', RIGHT, rule);
    SetRule(rule, '$', '$', RIGHT, rule);
    SetRule(rule, '_', '_', endDirection, rule+1);

    return rule+1;
}

uint16_t next$RIGHT(uint16_t rule, int8_t endDirection = RIGHT) {

    SetRule(rule, '0', '0', RIGHT, rule);
    SetRule(rule, '1', '1', RIGHT, rule);
    SetRule(rule, '_', '_', RIGHT, rule);
    SetRule(rule, '$', '$', endDirection, rule+1);
    
    return rule+1;
}

uint16_t next$LEFT(uint16_t rule, int8_t endDirection = LEFT) {

    SetRule(rule, '0', '0', LEFT, rule);
    SetRule(rule, '1', '1', LEFT, rule);
    SetRule(rule, '_', '_', LEFT, rule);
    SetRule(rule, '$', '$', endDirection, rule+1);
    
    return rule+1;
}

uint16_t nextLEFT(uint16_t rule){

    SetRule(rule, '0', '0', LEFT, rule+1);
    SetRule(rule, '1', '1', LEFT, rule+1);
    SetRule(rule, '_', '_', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+1);

    return rule+1;
}

uint16_t nextRIGHT(uint16_t rule){

    SetRule(rule, '0', '0', RIGHT, rule+1);
    SetRule(rule, '1', '1', RIGHT, rule+1);
    SetRule(rule, '_', '_', RIGHT, rule+1);
    SetRule(rule, '$', '$', RIGHT, rule+1);

    return rule+1;
}

#include "rule_markMemByte.h"
#include "rule_cpyMMBArgToPC.h"
#include "rule_cpyMMBArgToMAR.h"
#include "rule_cpyMMBToAB.h"
#include "rule_cpyAtoMMB.h"

#include "rule_reset.h"
#include "rule_fetch.h"
#include "rule_decode.h"
#include "rule_ldi.h"
#include "rule_sta.h"
#include "rule_add.h"
#include "rule_jc.h"
#include "rule_out.h"
#include "rule_lda.h"
#include "rule_jmp.h"

void GenerateRuleBook(uint8_t USE_FSM) {

    uint8_t i;
    uint16_t rule = 0;
    uint8_t rule_ADD;

    // Initialize all rules as Error State
    while(rule < RULES){

        SetRule(rule, '_', '_', RIGHT, RULE_ERROR);
        SetRule(rule, '0', '0', RIGHT, RULE_ERROR);  
        SetRule(rule, '1', '1', RIGHT, RULE_ERROR);
        SetRule(rule, '$', '$', RIGHT, RULE_ERROR);
        rule++;
    }


    GenerateRuleRESET(RULE_RESET, USE_FSM);
    GenerateRuleLDI(RULE_LDI);
    GenerateRuleSTA(RULE_STA);
    GenerateRuleADD(RULE_ADD);      // Call before GenerateRuleSUB(RULE_SUB)
    GenerateRuleJC(RULE_JC);
    GenerateRuleOUT(RULE_OUT);
    GenerateRuleLDA(RULE_LDA);
    GenerateRuleJMP(RULE_JMP);
    GenerateRuleDECODE(RULE_DECODE);
    GenerateRuleFETCH(RULE_FETCH);
    


    // // Seek to B0
    // rule = nextRIGHT(rule);
    // rule = nextRIGHT(rule);
    // rule = next$RIGHT(rule);
    // rule = next$RIGHT(rule);
    // SetRule(rule-1, '$', '$', LEFT, rule);

    // // Set B0 to 1 as constant to increment A
    // SetRule(rule, '_', '_', RIGHT, rule+1);
    // SetRule(rule, '0', '1', RIGHT, rule+1);  
    // SetRule(rule, '1', '1', RIGHT, rule+1);
    // SetRule(rule, '$', '$', RIGHT, rule+1);
    // rule++;

    // // Seek back to B0 - Next rule is NoCarry
    // SetRule(rule, '0', '0', LEFT, rule+2);
    // SetRule(rule, '1', '1', LEFT, rule+2);
    // SetRule(rule, '_', '_', LEFT, rule+2);
    // SetRule(rule, '$', '$', LEFT, rule+2);
    // rule++;

    // //----------------- A + B ----------------------

    // // 0
    // SetRule(rule, '0', '0', LEFT, rule+1);      // SUM = 0
    // SetRule(rule, '1', '1', LEFT, rule+1);
    // rule++;
    // // 1
    // rule_ADD = rule;
    // SetRule(rule, '0', '0', LEFT, rule-1);      // No Carry
    // SetRule(rule, '1', '1', LEFT, rule+1);
    // SetRule(rule, '$', '$', LEFT, rule+4);      // End of Add with no carry
    // rule++;
    // // 2
    // SetRule(rule, '0', '1', LEFT, rule-1);      // SUM = 1
    // SetRule(rule, '1', '0', LEFT, rule+1);
    // rule++;
    // // 3
    // SetRule(rule, '0', '0', LEFT, rule-1);      // Carry
    // SetRule(rule, '1', '1', LEFT, rule+1);
    // SetRule(rule, '$', '$', LEFT, rule+2);      // End of Add with carry
    // rule++;
    // // 4
    // SetRule(rule, '0', '0', LEFT, rule-1);      // SUM = 2
    // SetRule(rule, '1', '1', LEFT, rule-1);
    // rule++;

    // // Mark A/B
    // rule = nextRIGHT(rule);
    // rule = nextRIGHT(rule);
    // SetRule(rule-1, '$', '_', RIGHT, rule);

    // // Mark Output register
    // rule = next$RIGHT(rule);
    // SetRule(rule-1, '$', '_', LEFT, rule);
    
    // // Seek to A7
    // rule = next_LEFT(rule);
    // SetRule(rule-1, '_', '_', RIGHT, rule); 
    
    // // Copy A to Output register
    // rule = CopyAtoMMB(rule);

    // // Remark A/B
    // rule = nextLEFT(rule);
    // SetRule(rule, '_', '$', RIGHT, rule+1);
    // rule++;

    // // Remark Output
    // rule = next_RIGHT(rule);
    // SetRule(rule-1, '_', '$', RIGHT, rule); 

    // // Out A
    // for(i=0; i<7; i++) {

    //     // Output of bit7 to bit1
    //     SetRule(rule, '0', '0', RIGHT, rule+1, CLKOUT);
    //     SetRule(rule, '1', '1', RIGHT, rule+1, CLKOUT);
    //     rule++;

    //     // This is only to clear CLKOUT
    //     rule = nextLEFT(rule);
    //     rule = nextRIGHT(rule);
    // }

    // // Output bit0. 
    // /* The direction is now to the left because the next character to the right is 
    // the dollar sign, representing the transfer of data from the shift register to 
    // the output register of the 74HC595. At this point, however, the CLKOUT line is 
    // still HIGH. When the Notepad pointer subsequently moves to the left onto the dollar 
    // sign of the output register CLKOUT is first set to LOW, and the data transfer 
    // within the shift register takes place upon reaching the dollar sign. This ensures 
    // everything proceeds smoothly.
    // */
    // SetRule(rule, '0', '0', LEFT, rule+1, CLKOUT);
    // SetRule(rule, '1', '1', LEFT, rule+1, CLKOUT);
    // rule++;
    // // The Notepadpointer is now on position 28
    // // First move to right clears the CLKOUT.
    // // Second move to right, here is a '$', clocks se Outputregister of the 74HC595
    // rule = next$RIGHT(rule, LEFT);

    // // Skip back to BO and jump to A + B
    // rule = next$LEFT(rule);
    // SetRule(rule-1, '$', '$', LEFT, rule_ADD);

    // Serial.println(rule);
}

