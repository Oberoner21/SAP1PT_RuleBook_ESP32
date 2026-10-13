/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_incPC.h

    Copyright(C) 2026 by Oberoner

    Increments the Program Counter
    Entry conditions: Nothing is marked
    Entry point     : NP pointer position I0 of the Instruction register IR
    Exit point      : NP pointer position P3 of Program Counter PC
    Rules           : 6 
*/
uint16_t IncPC(uint16_t rule) {

    rule = next$LEFT(rule, LEFT);                  // Skip to bit 0 (constant 1) of PC
    SetRule(rule-1, '$', '$', LEFT, rule+1);       // Next rule is NoCarry

    SetRule(rule, '0', '0', LEFT, rule+1);          // SUM = 0
    SetRule(rule, '1', '1', LEFT, rule+1);
    rule++;

    SetRule(rule, '0', '0', LEFT, rule-1);          // No Carry
    SetRule(rule, '1', '1', LEFT, rule+1);
    SetRule(rule, '$', '$', RIGHT, rule+4);         // Down without carry
    rule++;

    SetRule(rule, '0', '1', LEFT, rule-1);          // SUM = 1
    SetRule(rule, '1', '0', LEFT, rule+1);
    rule++;

    SetRule(rule, '0', '0', LEFT, rule-1);          // Carry
    SetRule(rule, '1', '1', LEFT, rule+1);
    SetRule(rule, '$', '$', RIGHT, rule+2);         // Down with carry
    rule++;

    SetRule(rule, '0', '0', LEFT, rule-1);          // SUM = 2
    rule++;

    return rule;
}