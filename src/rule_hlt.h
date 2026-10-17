/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_ldi.h

    Copyright(C) 2026 by Oberoner
*/


void GenerateRuleHLT(uint16_t rule) {

    // Entrypoint: NP pointer position: I0 of Instruction Register IR
    // Exitpoint : NP pointer position: M0 of Memory Address Register MAR

    // Mark register A
    // Remark IR
    // Mark MAR
    // Seek to MMB bit 7

    SetRule(rule, '_', '_', LEFT, RULE_END);
    SetRule(rule, '0', '0', LEFT, RULE_END);
    SetRule(rule, '1', '1', LEFT, RULE_END);
    SetRule(rule, '$', '$', LEFT, RULE_END);
    rule++;
}