/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_cpyPCtoMAR.h

    Copyright(C) 2026 by Oberoner

    Copies the value from interleaved PC into MAR
    Entry conditions: Nothing is marked
    Entry point     : NP pointer position M0 of the register MAR
    Exit point      : NP pointer position I0 of the Instruction register IR
    Rules           : 12 
*/

uint16_t CopyPCtoMAR(uint16_t rule){

    // 0 Mark M0
    SetRule(rule, '0', '_', LEFT, rule+1);
    SetRule(rule, '1', '_', LEFT, rule+1);
    rule++;

    // 1 Skip LEFT MAR
    rule = next$LEFT(rule);

    // 2 Skip LEFT IR
    rule = next$LEFT(rule);

    // 3 Skip Cx of PC
    rule = nextLEFT(rule);
    SetRule(rule-1, '$', '$', RIGHT, rule+7);     // Exit point

    // 4 Read Px and mark it
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+4);
    rule++; 

    // 5 Seek right to marked bit of MAR and write 0
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '0', LEFT, rule);

    // 6 Skip left next byte of MAR and mark it
    SetRule(rule, '0', '_', LEFT, rule+1);
    SetRule(rule, '1', '_', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+1);
    rule++; 

    // 7 Seek left to marked bit of PC and write 0
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '0', LEFT, rule-5);

    // 8 Seek right to marked bit of MAR and write 1
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '1', LEFT, rule);

    // 9 Skip left next byte of MAR and mark it
    SetRule(rule, '0', '_', LEFT, rule+1);
    SetRule(rule, '1', '_', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+1);
    rule++; 

    // 10 Seek left to marked bit of PC and write 1
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '1', LEFT, rule-8);

    // -------- EXIT ---------

    // 11 Skip right PC
    rule = next$RIGHT(rule);

    // 12 Skip right IR and go to position I0
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', LEFT, rule);

    return rule;
}