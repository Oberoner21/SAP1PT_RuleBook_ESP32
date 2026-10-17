/*
    SAP1PT - SAP1 Pure Turing
    Modul:  cpyMMBArgToPC.h

    Copyright(C) 2026 by Oberoner

    Copies the argument (low nibble) from a marked memory byte (MMB) into 
    the interleaved program counter.
    Entry conditions: PC and MMB is marked
    Entry point     : NP pointer position bit7 of the MMB
    Exit conditions : Nothing is marked
    Exit point      : NP pointer position M3
    Rules           : 17
*/
uint16_t CopyMMBArgToPC(uint16_t rule) {

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
    SetRule(rule, '0', '_', LEFT, rule+5);
    SetRule(rule, '1', '_', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+9);  // End point
    rule++;

    // 7 Seek left to the marked bit of PC and write 1
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '1', RIGHT, rule);

    // 8 Skip cx
    rule = nextRIGHT(rule);

    // 9 Mark the next bit right of PC
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++;

    // 10 Seek right to the marked bit of MMB, write 1 and repeat with the next bit
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '1', RIGHT, rule-5);

    // 11 Seek left to the marked bit of PC and write 0
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '0', RIGHT, rule);

    // 12 Skip cx
    rule = nextRIGHT(rule);

    // 13 Mark the next bit right of PC
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++;

    // 14 Seek right to the marked bit of MMB, write 0 and repeat with the next bit
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '0', RIGHT, rule-9);   // The rule are just incrementet !!!!

    // ----- Done copy -----

    // 15 Remark the last marked byte of PC
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);

    // 16 Seek to M3
    rule = next$RIGHT(rule);

    return rule;
}