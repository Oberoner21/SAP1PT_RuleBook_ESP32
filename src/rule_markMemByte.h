/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_markMemByte.h

    Copyright(C) 2026 by Oberoner
*/

uint16_t MarkMemoryByte(uint16_t rule){

    // Comment numbers are rule numbers from rule diagramm

    // Entrypoint: NP pointer position: Bit M3 of register MAR
    // Exitpoint : NP pointer position: Bit7 of the marked memory byte

    uint8_t i;

    // ----- MAR address demuxer 4 to 16 --------

    // 0
    SetRule(rule, '0', '0', RIGHT, rule+4);     // MAR3 = 0
    SetRule(rule, '1', '1', RIGHT, rule+1);     // MAR3 = 1
    rule++;
    // 1
    SetRule(rule, '0', '0', RIGHT, rule+6);     // MAR2 = 0
    SetRule(rule, '1', '1', RIGHT, rule+1);     // MAR2 = 1
    rule++;
    // 2
    SetRule(rule, '0', '0', RIGHT, rule+8);     // MAR1 = 0
    SetRule(rule, '1', '1', RIGHT, rule+1);     // MAR1 = 1
    rule++;
    // 3
    SetRule(rule, '0', '0', RIGHT, rule+26);    // MAR0 = 0
    SetRule(rule, '1', '1', RIGHT, rule+27);    // MAR0 = 1
    rule++;
    // 4
    SetRule(rule, '0', '0', RIGHT, rule+1);     // MAR3 = 0 & MAR2 = 0
    SetRule(rule, '1', '1', RIGHT, rule+4);     // MAR3 = 0 & MAR2 = 1
    rule++;
    // 5
    SetRule(rule, '0', '0', RIGHT, rule+1);     // MAR3 = 0 & MAR2 = 0 & MAR1 = 0
    SetRule(rule, '1', '1', RIGHT, rule+4);     // MAR3 = 0 & MAR2 = 0 & MAR1 = 1
    rule++;
    // 6
    SetRule(rule, '0', '0', RIGHT, rule+9);     // MAR3 = 0 & MAR2 = 0 & MAR1 = 0 & MAR0 = 0
    SetRule(rule, '1', '1', RIGHT, rule+10);    // MAR3 = 0 & MAR2 = 0 & MAR1 = 0 & MAR0 = 1
    rule++;
    // 7
    SetRule(rule, '0', '0', RIGHT, rule+5);     // MAR3 = 1 & MAR2 = 0 & MAR1 = 0
    SetRule(rule, '1', '1', RIGHT, rule+4);     // MAR3 = 1 & MAR2 = 0 & MAR1 = 1
    rule++;
    // 8
    SetRule(rule, '0', '0', RIGHT, rule+6);     // MAR3 = 0 & MAR1 = 0 & MAR1 = 0
    SetRule(rule, '1', '1', RIGHT, rule+5);     // MAR3 = 0 & MAR1 = 0 & MAR1 = 1
    rule++;
    // 9
    SetRule(rule, '0', '0', RIGHT, rule+8);     // MAR3 = 0 & MAR2 = 0 & MAR1 = 1 & MAR0 = 0;
    SetRule(rule, '1', '1', RIGHT, rule+9);     // MAR3 = 0 & MAR2 = 0 & MAR1 = 1 & MAR0 = 1;
    rule++;
    // 10
    SetRule(rule, '0', '0', RIGHT, rule+17);    // MAR3 = 1 & MAR2 = 1 & MAR1 = 1 & MAR0 = 0;
    SetRule(rule, '1', '1', RIGHT, rule+18);    // MAR3 = 1 & MAR2 = 1 & MAR1 = 1 & MAR0 = 1;
    rule++;
    // 11
    SetRule(rule, '0', '0', RIGHT, rule+14);    // MAR3 = 1 & MAR2 = 0 & MAR1 = 1 & MAR0 = 0;
    SetRule(rule, '1', '1', RIGHT, rule+15);    // MAR3 = 1 & MAR2 = 0 & MAR1 = 1 & MAR0 = 1;
    rule++;
    // 12
    SetRule(rule, '0', '0', RIGHT, rule+11);    // MAR3 = 1 & MAR2 = 0 & MAR1 = 0 & MAR0 = 0;
    SetRule(rule, '1', '1', RIGHT, rule+12);    // MAR3 = 1 & MAR2 = 0 & MAR1 = 1 & MAR0 = 1;
    rule++;
    // 13
    SetRule(rule, '0', '0', RIGHT, rule+8);     // MAR3 = 0 & MAR2 = 1 & MAR1 = 1 & MAR0 = 0;
    SetRule(rule, '1', '1', RIGHT, rule+9);     // MAR3 = 0 & MAR2 = 1 & MAR1 = 1 & MAR0 = 1;
    rule++;
    // 14
    SetRule(rule, '0', '0', RIGHT, rule+5);     // MAR3 = 0 & MAR2 = 1 & MAR1 = 0 & MAR0 = 0;
    SetRule(rule, '1', '1', RIGHT, rule+6);     // MAR3 = 0 & MAR2 = 1 & MAR1 = 0 & MAR0 = 1;
    rule++;

    // -------- Mark the MAR addressed memory byte --------

    // 15 - Mark Memory Byte0
    SetRule(rule, '$', '_', LEFT, rule+31);
    rule++;

    // 16 ... 30 - Skip $ right of MAR
    for(i=0; i<15; i++) {

        SetRule(rule, '$', '$', RIGHT, rule+15);
        rule++;    
    }

    // 31 - Mark Memory Byte
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '_', LEFT, rule+14); 

    // 32...45 - Skip Memory Byte
    for(i=0; i<14; i++){

        rule = next$RIGHT(rule);
        SetRule(rule-1, '$', '$', RIGHT, rule-2);   
    }

    // 46 - Remark MAR
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);

    // 47 - Skip to Bit7 of the marked byte 
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '_', RIGHT, rule);

    return rule;
}