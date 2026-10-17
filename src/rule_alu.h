/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_ldi.h

    Copyright(C) 2026 by Oberoner

    Instruction rule for ALU
    Adds A and B, sets or clears the C and Z flags, and jumps to RULE_FETCH
    Entry conditions: Nothing is marked
    Entry point     : NP pointer position P3 of register PC
    Exit conditions : Nothing is marked, jump to RULE_FETCH
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : 23 
*/
void GenerateRuleALU(uint16_t rule) {

    // Skip left
    rule = nextLEFT(rule);
    // Skip left and go to the entry point of NoCarry
    SetRule(rule, '$', '$', LEFT, rule+2);
    rule++;

    // Add A + B and set/reset the C and Z flag
    //----------------- A + B ----------------------

    // 0
    SetRule(rule, '0', '0', LEFT, rule+1);      // SUM = 0
    SetRule(rule, '1', '1', LEFT, rule+1);
    rule++;
    // 1
    SetRule(rule, '0', '_', LEFT, rule-1);      // No Carry = Entry point
    SetRule(rule, '1', '_', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+4);      // End of Add with no carry
    rule++;
    // 2
    SetRule(rule, '0', '1', LEFT, rule-1);      // SUM = 1
    SetRule(rule, '1', '0', LEFT, rule+1);
    rule++;
    // 3
    SetRule(rule, '0', '_', LEFT, rule-1);      // Carry
    SetRule(rule, '1', '_', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+3);      // End of Add with carry
    rule++;
    // 4
    SetRule(rule, '0', '0', LEFT, rule-1);      // SUM = 2
    SetRule(rule, '1', '1', LEFT, rule-1);
    rule++;

    // 5 - Reset carry
    SetRule(rule, '0', '0', RIGHT, rule+2);
    SetRule(rule, '1', '0', RIGHT, rule+2);
    rule++;

    // 6 - Set carry
    SetRule(rule, '0', '1', RIGHT, rule+1);
    SetRule(rule, '1', '1', RIGHT, rule+1);
    rule++;

    // 7 - Skip right
    rule = nextRIGHT(rule);

    // ----- Check zero flag ------
    // 8 - Look to a one in register A/B
    SetRule(rule, '_', '_', RIGHT, rule);       // Skip underscore and repeate
    SetRule(rule, '0', '0', RIGHT, rule);       // Skip 0 and repeat
    SetRule(rule, '1', '1', LEFT, rule+1);      // Found a 1 -> No zero
    SetRule(rule, '$', '$', LEFT, rule+4);      // A = 0 -> Set zero flag
    rule++;

    // Reset zero flag
    // 9 - Seek left to carry
    rule = next$LEFT(rule, LEFT);
    // 10 - Skip left to zero flag
    rule = nextLEFT(rule);
    // 11 - Write 0 to zero flag
    SetRule(rule, '0', '0', RIGHT, rule+4);
    SetRule(rule, '1', '0', RIGHT, rule+4);
    rule++;

    // Set zero flag
    // 12 - Seek left to carry
    rule = next$LEFT(rule, LEFT);
    // 13 - Skip left to zero flag
    rule = nextLEFT(rule);
    // 14 - Write 1 to zero flag
    SetRule(rule, '0', '1', RIGHT, rule+1); // ++++++ Rule 1158
    SetRule(rule, '1', '1', RIGHT, rule+1);
    rule++; 
 
    // Fill register B with 0
    // 15 - Skip to right over carry
    rule = nextRIGHT(rule);
    // 16 - Skip to right over $
    rule = nextRIGHT(rule);

    // 17 - Fill the B register
    SetRule(rule, '_', '0', RIGHT, rule);
    SetRule(rule, '0', '0', RIGHT, rule);
    SetRule(rule, '1', '1', RIGHT, rule);
    SetRule(rule, '$', '$', RIGHT, rule+1);   
    rule++;

    // Skip right PC
    rule = next$RIGHT(rule); 
    // Skip right Instruction Register IR
    rule = next$RIGHT(rule);     

    // Skip right Memory Address Register MAR and go to RULE_FETCH
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH);  

}