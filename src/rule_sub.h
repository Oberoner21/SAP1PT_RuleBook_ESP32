/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_sub.h

    Copyright(C) 2026 by Oberoner

    Instruction rule for SUB
    Entry conditions: IR and current MMB is marked
    Entry point     : NP pointer position I0 of Instruction Register IR
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : 95 
*/
void GenerateRuleSUB(uint16_t rule) {
    //           _
    // A-B = A+1+B
    // Set B = 1
    // A = A + B
    // Copy MMB to B
    // Negate B
    // A = A + B

    // ------------ Set register B to 1 ----------------

    // Remark IR
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '$', LEFT, rule);

    // Skip LEFT PC
    rule = next$LEFT(rule);

    // Set B0 to 1
    SetRule(rule, '0', '1', LEFT, rule+1);
    SetRule(rule, '1', '1', LEFT, rule+1);
    rule++;

    // -------- Set bits B1 to B7 to zero -----------
    // Skip LEFT current bit of A
    SetRule(rule, '0', '0', LEFT, rule+1);
    SetRule(rule, '1', '1', LEFT, rule+1);
    rule++;
    // Set current bit of B to 0, skip to LEFT and repeat bit for bit
    SetRule(rule, '0', '0', LEFT, rule-1);
    SetRule(rule, '1', '0', LEFT, rule-1);
    // Exit the loop upon reaching the $ symbol at position 4.
    SetRule(rule, '$', '$', RIGHT, rule+1);
    rule++;

    // Seek back RIGHT to B0
    rule = next$RIGHT(rule, LEFT);
    SetRule(rule-1, '$', '$', LEFT, rule+1);

    //----------------- A + 1 ----------------------

    // 0
    SetRule(rule, '0', '0', LEFT, rule+1);      // SUM = 0
    SetRule(rule, '1', '1', LEFT, rule+1);
    rule++;
    // 1
    SetRule(rule, '0', '0', LEFT, rule-1);      // No Carry
    SetRule(rule, '1', '0', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+4);      // End of Add with no carry
    rule++;
    // 2
    SetRule(rule, '0', '1', LEFT, rule-1);      // SUM = 1
    SetRule(rule, '1', '0', LEFT, rule+1);
    rule++;
    // 3
    SetRule(rule, '0', '0', LEFT, rule-1);      // Carry
    SetRule(rule, '1', '0', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+2);      // End of Add with carry
    rule++;
    // 4
    SetRule(rule, '0', '0', LEFT, rule-1);      // SUM = 2
    SetRule(rule, '1', '1', LEFT, rule-1);
    rule++;

    // ---------- Copy subtrahend from MMB to register B ------

    // Mark B0 of register A/B
    rule = next$RIGHT(rule);
    rule = next$RIGHT(rule, LEFT);
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    rule++;

    // Skip RIGHT A/B
    rule = next$RIGHT(rule);
    // Skip RIGHT PC
    rule = next$RIGHT(rule);  

    // Seek to MAR to bit M3
    rule = next$RIGHT(rule);

    // Mark M3 and skip right
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++;

    // Seek to bit7 of the MMB
    rule = next_RIGHT(rule);

    // Copy low nibble of MMB into MAR
    rule = CopyMMBArgToMAR(rule);

    // Mark the MMB, addressed by register MAR
    rule = MarkMemoryByte(rule);
    // Remark MMB and skip right to bit7 of MMB
    SetRule(rule-1, '_', '$', RIGHT, rule);

    // Copy MMB Value to Register B
    rule = CopyMMBToB(rule);

    // -------- Negate B and jump to RULE_ALU -------
    // Skip current bit of A
    SetRule(rule, '0', '0', RIGHT, rule+1);
    SetRule(rule, '1', '1', RIGHT, rule+1);
    // Exit the loop upon reaching the $ symbol at position 21
    SetRule(rule, '$', '$', RIGHT, RULE_ALU);
    rule++;
    // Negate the current bit of register B and repeat
    SetRule(rule, '0', '1', RIGHT, rule-1);
    SetRule(rule, '1', '0', RIGHT, rule-1);
    rule++;

}
