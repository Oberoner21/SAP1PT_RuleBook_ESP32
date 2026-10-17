/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_lda.h

    Copyright(C) 2026 by Oberoner

    Instruction rule for LDA
    Entry conditions: IR and current MMB is marked
    Entry point     : NP pointer position I0 of Instruction Register IR
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : 84 
*/
void GenerateRuleLDA(uint16_t rule) {

    SetRule(rule, '_', '_', LEFT, rule+1);
    SetRule(rule, '0', '0', LEFT, rule+1);
    SetRule(rule, '1', '1', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+1);
    rule++;

    // Skip left IR (is yet market) and PC
    rule = next$LEFT(rule);

    // Skip left A/B register and mark A7
    rule = next$LEFT(rule);
    SetRule(rule-1, '$', '$', RIGHT, rule);

    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    rule++;

    // Seek to IR marker and remark the register
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);

    // Seek to MAR to bit M3
    rule = next$RIGHT(rule);

    // Mark M3 and skip right
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++;

    // Seek right to marked memory byte to bit7
    rule = next_RIGHT(rule);

    // Copy low nibble of MMB into MAR
    rule = CopyMMBArgToMAR(rule);

    // Seek to bit 7 of MAR addressed memory byte MMB
    rule = MarkMemoryByte(rule);
    // Remarke MMB
    SetRule(rule-1, '_', '$', RIGHT, rule);

    // copy MMB to register A
    rule = CopyMMBToA(rule);

    // Skip right A/B register
    //rule = next$RIGHT(rule); 
    // Skip right PC
    rule = next$RIGHT(rule); 
    // Skip right Instruction Register IR
    rule = next$RIGHT(rule);     
    // Skip right Memory Address Register MAR and go to RULE_FETCH
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH);  

}