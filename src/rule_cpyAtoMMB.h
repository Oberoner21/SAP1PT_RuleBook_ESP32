/*
    SAP1PT - SAP1 Pure Turing
    Modul:  cpyAtoMMB.h

    Copyright(C) 2026 by Oberoner

    Copies the A Register into marked memory byte of MMB
    Entry conditions: A/B and bit7 of MMB is marked
    Entry point     : NP pointer position A7 of A/B Register
    Exit conditions : Nothing is yet marked
    Exit point      : NP pointer position A7 of A/B Register
    Rules           : 66 
*/
uint16_t CopyAtoMMB(uint16_t rule) {

    // 0 Skip one position left to the A/B marker
    rule = nextLEFT(rule);

    // 1 Skip current bit of register B
    rule = nextRIGHT(rule);

    // 2 Read current bit of register A, mark it and branch
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+4);
    SetRule(rule, '$', '$', LEFT, rule+7);
    rule++;

    // 3 Seek right to marked bit of MMB and write 0
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '0', RIGHT, rule);

    // 4 Skip left next byte of MMB and mark it
    SetRule(rule, '0', '_', LEFT, rule+1);
    SetRule(rule, '1', '_', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+1);
    rule++; 

    // 5 Seek left to marked bit of A and write 0
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '0', RIGHT, rule-5);

    // 6 Seek right to marked bit of MMB and write 1
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '1', RIGHT, rule);

    // 7 Skip left next byte of MMB and mark it
    SetRule(rule, '0', '_', LEFT, rule+1);
    SetRule(rule, '1', '_', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+1);
    rule++; 

    // 8 Seek left to marked bit of A and write 1
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '1', RIGHT, rule-8);    

    // ------ Exit point -----
    // 9 Seek left to the A/B marker, remark it and go to A7
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);

    return rule;

}