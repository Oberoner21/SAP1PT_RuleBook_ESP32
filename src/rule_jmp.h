/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_jmp.h

    Copyright(C) 2026 by Oberoner
*/

/*
    Instruction rule for JMP
    Entry conditions: IR and current MMB is marked
    Entry point     : NP pointer position I0 of Instruction Register IR
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : 87 
*/
void GenerateRuleJMP(uint16_t rule) {

    //  Seek to PC and mark it
    //  Copy low nibble MMB into interleaved PC
    //  Remark PC and END
    //  Remark MMB,
    //  Remark IR
    //  Seek to MAR0
    //  Go to RULE_FETCH

    uint8_t i, j;

    SetRule(rule, '_', '_', RIGHT, rule+1);
    SetRule(rule, '0', '0', RIGHT, rule+1);
    SetRule(rule, '1', '1', RIGHT, rule+1);
    SetRule(rule, '$', '$', RIGHT, rule+1);
    rule++;
    
    // Seek to PC and mark it
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '_', RIGHT, rule);
    // Seek to bit7 of MMB
    rule = next_RIGHT(rule);

    // Copy low nibbel of MMB into interleaved PC
    rule = CopyMMBArgToPC(rule);

    // Remark PC
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);
    
    // ++++++++++++ END ++++++++++++++++++

    // Seek back to MMB an remark it
    rule = next_RIGHT(rule); 
    SetRule(rule-1, '_', '$', LEFT, rule);
    // Seek to marked register IR and remark it
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);
    // Skip right Instruction Register IR
    rule = next$RIGHT(rule);     
    // Skip right Program counter PC
    rule = next$RIGHT(rule); 
    // Skip right Memory Address Register MAR and go to RULE_FETCH
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH);  

}