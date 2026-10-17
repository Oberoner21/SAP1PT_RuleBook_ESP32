/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_ldi.h

    Copyright(C) 2026 by Oberoner

    Instruction rule for LDI
    Entry conditions: IR and current MMB is marked
    Entry point     : NP pointer position I0 of Instruction Register IR
    Entry conditions: Nothing is marked, jump to RULE_FETCH
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : 33 
*/
void GenerateRuleLDI(uint16_t rule) {

    uint8_t i, j;

    SetRule(rule, '_', '_', LEFT, rule+1);
    SetRule(rule, '0', '0', LEFT, rule+1);
    SetRule(rule, '1', '1', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+1);
    rule++;

    // Skip left IR (is yet market) and PC
    rule = next$LEFT(rule);

    // Skip left A/B register
    rule = next$LEFT(rule, RIGHT);

    // Set A7 to A4 to 0
    SetRule(rule, '0', '0', RIGHT, rule+1);
    SetRule(rule, '1', '0', RIGHT, rule+1);
    rule++;
    SetRule(rule, '0', '0', RIGHT, rule+1); // Skip B7
    SetRule(rule, '1', '1', RIGHT, rule+1);
    rule++;
    SetRule(rule, '0', '0', RIGHT, rule+1);
    SetRule(rule, '1', '0', RIGHT, rule+1);
    rule++;
    SetRule(rule, '0', '0', RIGHT, rule+1); // Skip B6
    SetRule(rule, '1', '1', RIGHT, rule+1);
    rule++;
    SetRule(rule, '0', '0', RIGHT, rule+1);
    SetRule(rule, '1', '0', RIGHT, rule+1);
    rule++;
    SetRule(rule, '0', '0', RIGHT, rule+1); // Skip B5
    SetRule(rule, '1', '1', RIGHT, rule+1);
    rule++;
    SetRule(rule, '0', '0', RIGHT, rule+1);
    SetRule(rule, '1', '0', RIGHT, rule+1);
    rule++;
    SetRule(rule, '0', '0', RIGHT, rule+1); // Skip B4
    SetRule(rule, '1', '1', RIGHT, rule+1);
    rule++;

    // Mark A3
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    rule++;

    // Seek to IR marker and remark the register
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);

    // Seek right to marked memory byte to bit7
    rule = next_RIGHT(rule);

    // Copy low nibble MMB to A3-A0
    // Used the rules for copy argument into PC
    rule = CopyMMBArgToPC(rule);
 
    // Skip right Instruction Register IR
    rule = next$RIGHT(rule);    
    // Skip right Memory Address Register MAR and go to RULE_FETCH
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH);

}