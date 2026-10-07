/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_lda.h

    Copyright(C) 2026 by Oberoner
*/

/*
    Instruction rule for LDA
    Entry conditions: IR and current MMB is marked
    Entry point     : NP pointer position I0 of Instruction Register IR
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : 332 
*/
void GenerateRuleLDA(uint16_t rule) {

    uint8_t i, j;

    SetRule(rule, '_', '_', LEFT, rule+1);
    SetRule(rule, '0', '0', LEFT, rule+1);
    SetRule(rule, '1', '1', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+1);
    rule++;

    // Skip left IR (is yet market) and output register
    rule = next$LEFT(rule);

    // Skip left A/B register and mark it
    rule = next$LEFT(rule);
    SetRule(rule-1, '$', '_', RIGHT, rule);

    // Seek to IR marker and remark the register
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);

    // Skip IR
    rule = next$RIGHT(rule);

    // Seek right to register MAR and mark it
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '_', RIGHT, rule);

    // Seek right to marked memory byte to bit7
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '_', RIGHT, rule);

    // Copy low nibble of MMB into MAR
    rule = CopyMMBArgToMAR(rule);

    // Seek back to bit3 of MAR
    rule = next_LEFT(rule);

    // Mark the MMB, addressed by register MAR
    rule = MarkMemoryByte(rule);
    // Change the direction of last rule to RIGHT
    SetRule(rule-1, '_', '_', RIGHT, rule);

    // copy MMR to register B
    rule = CopyMMBToAB(rule);

    // Skip right A/B register
    rule = next$RIGHT(rule); 
    // Skip right Output register
    rule = next$RIGHT(rule); 
    // Skip right Instruction Register IR
    rule = next$RIGHT(rule);     
    // Skip right Program counter PC
    rule = next$RIGHT(rule); 
    // Skip right Memory Address Register MAR and go to RULE_FETCH
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH);  

}