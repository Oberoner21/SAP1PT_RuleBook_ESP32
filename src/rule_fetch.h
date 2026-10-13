/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_fetch.h

    Copyright(C) 2026 by Oberoner

    Fetch the current instruction into IR and jump to RULE_DECODE
    Entry conditions: Nothing is marked
    Entry point     : NP pointer position M0 of the register MAR
    Exit point      : NP pointer position I3 of Instruction Registers IR
    Exit conditions : IR and MMB is marked, next rule is RULE_DECODE
    Rules           : 6 
*/

void GenerateRuleFETCH(uint16_t rule) {

    rule = CopyPCtoMAR(rule);   // Copy Program Counter into MAR
    rule = IncPC(rule);         // Increment Program Counter
    rule = INSTtoIR(rule);      // Copy the current instruction to Register IR and jump to RULE_DECODE
}