/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_ldi.h

    Copyright(C) 2026 by Oberoner

    Instruction rule for ADD
    Entry conditions: IR and current MMB is marked.
    Entry point     : NP pointer position I0 of Instruction Register IR
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : 82 
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

    SetRule(rule, '_', '_', LEFT, rule+1);
    SetRule(rule, '0', '0', LEFT, rule+1);
    SetRule(rule, '1', '1', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+1);
    rule++;

    // Skip left IR (is yet market) and PC
    rule = next$LEFT(rule);

    // Mark B0
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

    // Seek to bit7 of the marked memory byte
    rule = next_RIGHT(rule);

    // Copy low nibble of MMB into MAR
    rule = CopyMMBArgToMAR(rule);

    // Mark the MMB, addressed by register MAR
    rule = MarkMemoryByte(rule);
    // Remark MMB and skip right to bit7 of MMB
    SetRule(rule-1, '_', '$', RIGHT, rule);

    // copy MMB to register B
    rule = CopyMMBToB(rule);

    // Seek to P3
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', RIGHT, RULE_ALU);

}