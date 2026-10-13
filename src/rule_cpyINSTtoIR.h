/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_cpyINSTtoIR.h

    Copyright(C) 2026 by Oberoner

    Copies the instruction of the current program step into the IR.
    The instruction is stored in the high nibble of the opcode.
    Entry conditions: Nothing is marked
    Entry point     : NP pointer position P3 of Program Counter PC
    Exit point      : NP pointer position I3 of Instruction Registers IR
    Rules           : 6 
*/

uint16_t INSTtoIR(uint16_t rule){

    uint8_t i, j;  

    // Mark IR
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '_', RIGHT, rule);

    // Mark MAR and seek to M3
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '_', RIGHT, rule);

    // ------ Mark the memory byte, addressed by register MAR ------
    rule = MarkMemoryByte(rule);

    // ++++ Copy high nibble of marked memory byte to instruction register IR ++++
    // Destination register IR is marked
    // NP position: Bit7 of the marked byte

    // Copy 4 bits
    for(i=0; i<4; i++) {

        for(j=0; j<i; j++){
 
            // Skip jx right in marked memory byte 
            rule = nextRIGHT(rule);     
        }

        // Read current bit of marked memory Byte and branche
        SetRule(rule, '0', '0', RIGHT, rule+1);
        SetRule(rule, '1', '1', RIGHT, rule+4+i);
        rule++;

        // Skip left to the memory byte marker
        rule = next_LEFT(rule, LEFT); 

        // Skip to bit3 of the marked register IR 
        rule = next_LEFT(rule);

        for(j=0; j<i; j++){
 
            // Skip jx bit right in IR
            rule = nextRIGHT(rule);     
        }

        // Write a 0 into current bit of IR
        SetRule(rule, '0', '0', RIGHT, rule+4+i);
        SetRule(rule, '1', '0', RIGHT, rule+4+i);
        rule++;

    
        // Skip to memory byte marker 
        rule = next_LEFT(rule, LEFT);
        // Skip the market destination register IR 
        rule = next_LEFT(rule);

        for(j=0; j<i; j++){
 
            // Skip jx bit right in IR
            rule = nextRIGHT(rule);     
        }

        // Write a 1 into current bit of IR
        SetRule(rule, '0', '1', RIGHT, rule+1);
        SetRule(rule, '1', '1', RIGHT, rule+1);
        rule++;

        // Skip back to Bit7 of the marked byte 
        rule = next_RIGHT(rule);
    }

    // ---- End of copy Instruction to IR ------

    // Change the last rule
    // Skip back left to the marker of marked memory byte
    SetRule(rule-1, '_', '_', LEFT, rule);
    // Skip back right to the marker of the Instruction Register IR to I3
    // and go to RULE_DECODE
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '_', RIGHT, RULE_DECODE);

    // Exitpoint:  NP pointer position: I3 of Instruction Registers IR
    //             Mark of Instruction register IR and of the marked memory
    //             are not cleared.

    return rule;
}
