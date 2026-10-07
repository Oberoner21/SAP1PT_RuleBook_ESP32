/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_sta.h

    Copyright(C) 2026 by Oberoner
*/

/*
    Instruction rule for STA
    Entry conditions: IR and current MMB is marked
    Entry point     : NP pointer position I0 of Instruction Register IR
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : 298 
*/
void GenerateRuleSTA(uint16_t rule) {

    // Mark A/B
    // Remark IR
    // Seek to MAR an mark it
    // Seek to marked memory byte
    // Copy low nibble from marked memory byte to MAR
    // Remark the marked memory byte
    // Mark the MAR addressed memory byte
    // Remark MAR
    // Copy A to marked memory byte
    // Remark marked memory byte
    // Remark register A/B
    // Seek to bit M0 of Memory address register MR
    // Go back to RULE_FETCH

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

    // Skip Program counter PC
    rule = next$RIGHT(rule);

    // Seek to MAR and mark it
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '_', RIGHT, rule); 

    // Seek to bit7 of the marked memory byte
    rule = next_RIGHT(rule);

    // Copy low nibble of MMB into MAR
    rule = CopyMMBArgToMAR(rule);

    // Seek back to bit3 of MAR
    rule = next_LEFT(rule);

    // ------ Mark the memory byte, addressed by register MAR ------
    rule = MarkMemoryByte(rule);
    // Change the direction of last rule to LEFT
    SetRule(rule-1, '_', '_', LEFT, rule);

    // Seek to A7 of marked register A/B
    rule = next_LEFT(rule); 

    // Copy A into MMB
    rule = CopyAtoMMB(rule);

    // Seek back to MMB an remark it
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '$', LEFT, rule);
    // Seek to marked register A/B
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);

    // Skip right register A/B
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