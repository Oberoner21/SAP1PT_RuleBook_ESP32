/*
    SAP1PT - SAP1 Pure Turing
    Modul:  cpyMMBToAB.h

    Copyright(C) 2026 by Oberoner

    Copies the value from a marked memory byte (MMB) into register A
    Entry conditions: A7 and MMB is marked
    Entry point     : NP pointer position bit7 of the MMB
    Exit conditions : Nothing is marked
    Exit point      : NP pointer position P3 of the PC
    Rules           : Destination register A: 205
*/

uint16_t CopyMMBToA(uint16_t rule) {

    // 0 Read current bit of MMB, marke it and branch
    SetRule(rule, '0', '_', LEFT, rule+1);
    SetRule(rule, '1', '_', LEFT, rule+5);
    SetRule(rule, '$', '$', LEFT, rule+9);  // Exit point
    rule++;

    // 1 Seek left to marked bit of register A and write 0
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '0', RIGHT, rule);

    // 2 Skip right current bit of B
    rule = nextRIGHT(rule);

    // 3 Mark next bit in register A
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++;

    // 4 Seek back to marked bit of MMB, remark it, write 0 and repeat with next bit
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '0', RIGHT, rule-5);

    // 5 Seek left to marked bit of register A and write 1
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '1', RIGHT, rule);

    // 6 Skip right current bit of B
    rule = nextRIGHT(rule);

    // 7 Mark next bit in register A
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++;

    // 8 Seek back to marked bit of MMB, remark it, write 0 and repeat with next bit
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '1', RIGHT, rule-9);

    // ------- Exit ------
    
    // 9 Seek left to the last marker right from A/B, remark it and go to P3
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);

    return rule;

}
