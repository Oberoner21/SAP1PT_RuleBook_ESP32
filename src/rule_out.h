/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_out.h

    Copyright(C) 2026 by Oberoner

    Instruction rule for OUT
    Entry conditions: IR and current MMB is marked
    Entry point     : NP pointer position I0 of Instruction Register IR
    Exit conditions : No markers
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : 27 
*/
void GenerateRuleOUT(uint16_t rule) {

    // Skip one position to LEFT (Unique rule OUT)
    SetRule(rule, '_', '_', LEFT, rule+1);
    SetRule(rule, '0', '0', LEFT, rule+1);
    SetRule(rule, '1', '1', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+1);
    rule++;

    // Skip PC (IR is yet marked)
    rule = next$LEFT(rule);

    // Seek to A7
    rule = next$LEFT(rule, RIGHT);

    // Out A
    // Output of bit7 to bit1 (A register)
    SetRule(rule, '0', '0', RIGHT, rule+1, CLKOUT);
    SetRule(rule, '1', '1', RIGHT, rule+1, CLKOUT);
    // Latch shift register 74xx595 to output register 
    SetRule(rule, '$', '$', RIGHT, rule+2);
    rule++;
    // Skip Bx, set CLKOUT LOW and repeat next bit if A
    SetRule(rule, '0', '0', RIGHT, rule-1);
    SetRule(rule, '1', '1', RIGHT, rule-1);
    rule++;

    // Skip right the PC
    rule = next_RIGHT(rule);
    // Seek right to MMB and remarke it
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '$', LEFT, rule);
    // Seek left to the IR register and remark it
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);
    // Skip right Instruction Register IR
    rule = next$RIGHT(rule);     
    // Skip right MAR and go to RULE_FETCH
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH); 

}