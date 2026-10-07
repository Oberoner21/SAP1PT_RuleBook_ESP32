/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_ldi.h

    Copyright(C) 2026 by Oberoner
*/

/*
    Instruction rule for ADD
    Entry conditions: IR and current MMB is marked.
    Entry point     : NP pointer position I0 of Instruction Register IR
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : 366 
*/
void GenerateRuleADD(uint16_t rule) {

    // Mark A/B
    // Remark IR
    // Seek to MAR an mark it
    // Seek to marked memory byte
    // Copy low nibble from marked memory byte to MAR
    // Remark the marked memory byte
    // Mark the MAR addressed memory byte
    // Remark MAR
    // Seek to marked memory byte bit 7
    // Copy the marked memory byte into register B
    // Remark the marked memory byte

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

    // Mark the MMB, addressed by register MAR
    rule = MarkMemoryByte(rule);
    // Change the direction of last rule to RIGHT to seek to bit7 of MMB
    SetRule(rule-1, '_', '_', RIGHT, rule);

    // copy MMB to register B
    rule = CopyMMBToAB(rule, true);

    // Seek to B0
    rule = next$RIGHT(rule);
    // Change the rule to new value of next rule -> Rule 1 NoCarry
    SetRule(rule-1, '$', '$', LEFT, rule+1);

    // Add A + B and set the C and Z flag
    //----------------- A + B ----------------------

    // 0
    SetRule(rule, '0', '0', LEFT, rule+1);      // SUM = 0
    SetRule(rule, '1', '1', LEFT, rule+1);
    rule++;
    // 1
    rule_ALU = rule;
    SetRule(rule, '0', '_', LEFT, rule-1);      // No Carry
    SetRule(rule, '1', '_', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+4);     // End of Add with no carry
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