/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rulebook.h

    Copyright(C) 2026 by Oberoner
*/
#pragma once
#include "symbols.h"

#define     LEFT            -1
#define     RIGHT           1
#define     CLKOUT          1
#define     RULES           0x400
#define     SYMBOLS         4

#define     RULE_RESET      0
#define     RULE_FETCH      20
#define     RULE_DECODE     150
#define     RULE_NOP        200
#define     RULE_LDA        250
#define     RULE_ALU        350
#define     RULE_ADD        400
#define     RULE_SUB        500
#define     RULE_STA        600
#define     RULE_LDI        700
#define     RULE_JMP        750
#define     RULE_JC         800
#define     RULE_JZ         850
#define     RULE_OUT        900
#define     RULE_HLT        950

#define     RULE_END        1022
#define     RULE_ERROR      1023


struct rule {
    uint16_t next;
    int8_t direction;
    uint8_t writeSymbol;
    uint8_t clkOut;
} ruleBook[RULES][SYMBOLS];


void SetRule(uint16_t rule, uint8_t readSymbol, uint8_t writeSymbol, int8_t direction, 
    uint16_t nextRule, uint8_t clkOut = 0){

    uint8_t numReadSymbol = symbol2num(readSymbol);

    ruleBook[rule][numReadSymbol].writeSymbol = symbol2num(writeSymbol);
    ruleBook[rule][numReadSymbol].direction = direction;
    ruleBook[rule][numReadSymbol].next = nextRule;
    ruleBook[rule][numReadSymbol].clkOut = clkOut;
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


#include "rule_markMemByte.h"
#include "rule_cpyMMBArgToPC.h"
#include "rule_cpyMMBArgToMAR.h"
#include "rule_cpyMMBToA.h"
#include "rule_cpyMMBToB.h"
#include "rule_cpyAtoMMB.h"
#include "rule_cpyPCtoMAR.h"
#include "rule_incPC.h"
#include "rule_cpyINSTtoIR.h"

#include "rule_reset.h"
#include "rule_alu.h"
#include "rule_fetch.h"
#include "rule_decode.h"
#include "rule_nop.h"
#include "rule_ldi.h"
#include "rule_sta.h"
#include "rule_add.h"
#include "rule_jc.h"
#include "rule_out.h"
#include "rule_lda.h"
#include "rule_jmp.h"
#include "rule_hlt.h"
#include "rule_sub.h"
#include "rule_jz.h"


void GenerateRuleBook(uint8_t USE_FSM)
{
    uint16_t rule = 0;

    // Initialize all rules as Error State
    while(rule < RULES){

        SetRule(rule, '_', '_', RIGHT, RULE_ERROR);
        SetRule(rule, '0', '0', RIGHT, RULE_ERROR);  
        SetRule(rule, '1', '1', RIGHT, RULE_ERROR);
        SetRule(rule, '$', '$', RIGHT, RULE_ERROR);
        rule++;
    }

    GenerateRuleRESET(RULE_RESET, USE_FSM);
    GenerateRuleFETCH(RULE_FETCH);
    GenerateRuleDECODE(RULE_DECODE);
    GenerateRuleNOP(RULE_NOP);
    GenerateRuleLDI(RULE_LDI);
    GenerateRuleSTA(RULE_STA);
    GenerateRuleALU(RULE_ALU);
    GenerateRuleADD(RULE_ADD);
    GenerateRuleJC(RULE_JC);
    GenerateRuleOUT(RULE_OUT);
    GenerateRuleLDA(RULE_LDA);
    GenerateRuleJMP(RULE_JMP);
    GenerateRuleHLT(RULE_HLT);
    GenerateRuleSUB(RULE_SUB);
    GenerateRuleJZ(RULE_JZ);
}