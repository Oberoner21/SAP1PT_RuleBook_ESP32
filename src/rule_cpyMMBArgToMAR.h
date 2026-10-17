/*
    SAP1PT - SAP1 Pure Turing
    Modul:  cpyMMBArgToMAR.h

    Copyright(C) 2026 by Oberoner

    Copies the argument (low nibble) from a marked memory byte (MMB) into 
    the Memory Address Register MAR.
    Entry conditions: M3 and MMB is marked
    Entry point     : NP pointer position bit7 of the MMB
    Exit conditions : M3 is marked
    Exit point      : NP pointer position M3 of marked MAR 
    Rules           : 66 
*/
uint16_t CopyMMBArgToMAR(uint16_t rule) {

    // 0 Skip left to the MMB marker
    rule = nextLEFT(rule);
    // 1 Remark the MMB
    SetRule(rule, '_', '$', RIGHT, rule+1);
    rule++;
    // 2 Skip right to bit6 of MMB
    rule = nextRIGHT(rule);
    // 3 Skip right to bit5 of MMB
    rule = nextRIGHT(rule);
    // 4 Skip right to bit4 of MMB
    rule = nextRIGHT(rule);
    // 5 Skip right to bit3 of MMB
    rule = nextRIGHT(rule);

    // 6 Read current bit of MMB mark it and branch
    SetRule(rule, '0', '_', LEFT, rule+4);
    SetRule(rule, '1', '_', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+7);  // End point
    rule++;

    // 7 Seek left to the marked bit of MAR and write 1
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '1', RIGHT, rule);

    // 8 Mark the next bit right of MAR
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++;

    // 9 Seek right to the marked bit of MMB, write 1 and repeat with the next bit
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '1', RIGHT, rule-4);

    // 10 Seek left to the marked bit of MAR and write 0
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '0', RIGHT, rule);

    // 11 Mark the next bit right of MAR
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++;

    // 12 Seek right to the marked bit of MMB, write 0 and repeat with the next bit
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '0', RIGHT, rule-7);   // The rule are just incrementet !!!!

    // ----- Done copy -----

    // 13 Remark the last marked byte of MAR
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '$', LEFT, rule);

    // 14 Mark MAR and skip right to M3
    rule = next$LEFT(rule);
    SetRule(rule-1, '$', '_', RIGHT, rule);

    return rule;
}