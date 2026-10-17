/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_nop.h

    Copyright(C) 2026 by Oberoner

    Instruction rule for NOP - No operation
    Entry conditions: IR and current MMB is marked
    Entry point     : NP pointer position I0 of Instruction Register IR
    Exit conditions : No markers
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : 5 
*/
void GenerateRuleNOP(uint16_t rule) {

    // Skip one position to RIGHT (Unique rule NOP)
    SetRule(rule, '_', '_', RIGHT, rule+1);
    SetRule(rule, '0', '0', RIGHT, rule+1);
    SetRule(rule, '1', '1', RIGHT, rule+1);
    SetRule(rule, '$', '$', RIGHT, rule+1);
    rule++;

    // Seek to MMB and remark it
    rule = next_RIGHT(rule, LEFT);
    SetRule(rule-1, '_', '$', LEFT, rule);

    // Seek to IR and remark it
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);

    // Skip IR right
    rule = next$RIGHT(rule);

    // Seek to M0 and jump to RULE_FETCH
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH);
}