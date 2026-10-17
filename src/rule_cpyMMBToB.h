/*
    SAP1PT - SAP1 Pure Turing
    Modul:  cpyMMBToAB.h

    Copyright(C) 2026 by Oberoner

    Copies the value from a marked memory byte (MMB) into register B
    Entry conditions: B0 is marked
    Entry point     : NP pointer position bit7 of the MMB
    Exit conditions : Nothing is marked
    Exit point      : NP pointer position A7 of the register A/B
    Rules           : 11
*/

uint16_t CopyMMBToB(uint16_t rule) {

    // 0 Seek to bit 0 of MMB
    rule = next$RIGHT(rule, LEFT);

    // 1 Read current bit of MMB, marke it and branch
    SetRule(rule, '0', '_', LEFT, rule+1);
    SetRule(rule, '1', '_', LEFT, rule+5);
    SetRule(rule, '$', '$', LEFT, rule+9);  // Exit point  rule+9
    rule++;

    // 2 Seek left to marked bit of register A and write 0
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '0', LEFT, rule);

    // 3 Skip right current bit of B
    rule = nextLEFT(rule);

    // 4 Mark next bit in register A
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++;

    // 5 Seek back to marked bit of MMB, remark it, write 0 and repeat with next bit
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '0', LEFT, rule-5);

    // 6 Seek left to marked bit of register B and write 1
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '1', LEFT, rule);

    // 7 Skip right current bit of A
    rule = nextLEFT(rule);

    // 8 Mark next bit in register A
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++;

    // 9 Seek back to marked bit of MMB, remark it, write 1 and repeat with next bit
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '1', LEFT, rule-9);

    // ------- Exit ------
    
    // 10 Seek left to the last marker left from A/B, remark it and go to A7
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);

    return rule;

}
