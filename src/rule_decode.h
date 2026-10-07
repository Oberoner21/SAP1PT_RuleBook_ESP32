/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_decode.h

    Copyright(C) 2026 by Oberoner
*/

uint16_t GenerateRuleDECODE(uint16_t rule){

    // Entrypoint:  NP pointer position: I3 of Instruction Registers IR
    // Exitpoint :  On exit to every instruction 
    //              NP pointer position: I0 of Instruction Register IR

    uint8_t i;

    // ----- IR instruction demuxer 4 to 16 --------

    // 0
    SetRule(rule, '0', '0', RIGHT, rule+4);     // IR3 = 0
    SetRule(rule, '1', '1', RIGHT, rule+1);     // IR3 = 1
    rule++;
    // 1
    SetRule(rule, '0', '0', RIGHT, rule+6);     // IR2 = 0
    SetRule(rule, '1', '1', RIGHT, rule+1);     // IR2 = 1
    rule++;
    // 2
    SetRule(rule, '0', '0', RIGHT, rule+8);     // IR1 = 0
    SetRule(rule, '1', '1', RIGHT, rule+1);     // IR1 = 1
    rule++;
    // 3
    SetRule(rule, '0', '0', RIGHT, rule+26);    // IR0 = 0
    SetRule(rule, '1', '1', RIGHT, rule+27);    // IR0 = 1
    rule++;
    // 4
    SetRule(rule, '0', '0', RIGHT, rule+1);     // IR3 = 0 & IR2 = 0
    SetRule(rule, '1', '1', RIGHT, rule+4);     // IR3 = 0 & IR2 = 1
    rule++;
    // 5
    SetRule(rule, '0', '0', RIGHT, rule+1);     // IR3 = 0 & IR2 = 0 & IR1 = 0
    SetRule(rule, '1', '1', RIGHT, rule+4);     // IR3 = 0 & IR2 = 0 & IR1 = 1
    rule++;
    // 6
    SetRule(rule, '0', '0', RIGHT, rule+9);     // IR3 = 0 & IR2 = 0 & IR1 = 0 & IR0 = 0
    SetRule(rule, '1', '1', RIGHT, rule+10);    // IR3 = 0 & IR2 = 0 & IR1 = 0 & IR0 = 1
    rule++;
    // 7
    SetRule(rule, '0', '0', RIGHT, rule+5);     // IR3 = 1 & IR2 = 0 & IR1 = 0
    SetRule(rule, '1', '1', RIGHT, rule+4);     // IR3 = 1 & IR2 = 0 & IR1 = 1
    rule++;
    // 8
    SetRule(rule, '0', '0', RIGHT, rule+6);     // IR3 = 0 & IR1 = 0 & IR1 = 0
    SetRule(rule, '1', '1', RIGHT, rule+5);     // IR3 = 0 & IR1 = 0 & IR1 = 1
    rule++;
    // 9
    SetRule(rule, '0', '0', RIGHT, rule+8);     // IR3 = 0 & IR2 = 0 & IR1 = 1 & IR0 = 0;
    SetRule(rule, '1', '1', RIGHT, rule+9);     // IR3 = 0 & IR2 = 0 & IR1 = 1 & IR0 = 1;
    rule++;
    // 10
    SetRule(rule, '0', '0', RIGHT, rule+17);    // IR3 = 1 & IR2 = 1 & IR1 = 1 & IR0 = 0;
    SetRule(rule, '1', '1', RIGHT, rule+18);    // IR3 = 1 & IR2 = 1 & IR1 = 1 & IR0 = 1;
    rule++;
    // 11
    SetRule(rule, '0', '0', RIGHT, rule+14);    // IR3 = 1 & IR2 = 0 & IR1 = 1 & IR0 = 0;
    SetRule(rule, '1', '1', RIGHT, rule+15);    // IR3 = 1 & IR2 = 0 & IR1 = 1 & IR0 = 1;
    rule++;
    // 12
    SetRule(rule, '0', '0', RIGHT, rule+11);    // IR3 = 1 & IR2 = 0 & IR1 = 0 & IR0 = 0;
    SetRule(rule, '1', '1', RIGHT, rule+12);    // IR3 = 1 & IR2 = 0 & IR1 = 1 & IR0 = 1;
    rule++;
    // 13
    SetRule(rule, '0', '0', RIGHT, rule+8);     // IR3 = 0 & IR2 = 1 & IR1 = 1 & IR0 = 0;
    SetRule(rule, '1', '1', RIGHT, rule+9);     // IR3 = 0 & IR2 = 1 & IR1 = 1 & IR0 = 1;
    rule++;
    // 14
    SetRule(rule, '0', '0', RIGHT, rule+5);     // IR3 = 0 & IR2 = 1 & IR1 = 0 & IR0 = 0;
    SetRule(rule, '1', '1', RIGHT, rule+6);     // IR3 = 0 & IR2 = 1 & IR1 = 0 & IR0 = 1;
    rule++;

    // NP pointer position: $ before register PC (35)

    SetRule(rule, '$', '$', LEFT, RULE_NOP); // 0x0 NOP
    rule++;

    SetRule(rule, '$', '$', LEFT, RULE_LDA); // 0x1 LDA
    rule++;

    SetRule(rule, '$', '$', LEFT, RULE_ADD); // 0x2 ADD
    rule++;

    SetRule(rule, '$', '$', LEFT, RULE_SUB); // 0x3 SUB
    rule++;

    SetRule(rule, '$', '$', LEFT, RULE_STA); // 0x4 STA
    rule++;

    SetRule(rule, '$', '$', LEFT, RULE_LDI); // 0x5 LDI
    rule++;

    SetRule(rule, '$', '$', LEFT, RULE_JMP); // 0x6 JMP
    rule++;

    SetRule(rule, '$', '$', LEFT, RULE_JC); // 0x7 JC
    rule++;

    SetRule(rule, '$', '$', LEFT, RULE_JZ); // 0x8 JZ
    rule++;

    SetRule(rule, '$', '$', RIGHT, RULE_ERROR);
    rule++;

    SetRule(rule, '$', '$', RIGHT, RULE_ERROR);
    rule++;

    SetRule(rule, '$', '$', RIGHT, RULE_ERROR);
    rule++;

    SetRule(rule, '$', '$', RIGHT, RULE_ERROR);
    rule++;

    SetRule(rule, '$', '$', RIGHT, RULE_ERROR);
    rule++;

    SetRule(rule, '$', '$', LEFT, RULE_OUT); // 0xE OUT
    rule++;

    SetRule(rule, '$', '$', LEFT, RULE_HLT); // 0xF HLT
    rule++;

    return rule;
}