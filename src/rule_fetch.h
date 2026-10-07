/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_fetch.h

    Copyright(C) 2026 by Oberoner
*/

// Kopiert den Wert des Programm counters in das Memory Address Register
uint16_t PCtoMAR(uint16_t rule){

    // Entrypoint: NP pointer position: M0 (MAR bit 0)
    // Exitpoint : NP pointer position: P3 (PC bit 7)

    uint8_t i, j;

    // ++++++++++++++++++++ Copy Program Counter to MAR +++++++++++++++++++++++++++

    // Skip to PC and marke it
    rule = next$LEFT(rule);                     // Skip MAR
    rule = next$LEFT(rule);                     // Skip PC
    SetRule(rule-1, '$', '_', RIGHT, rule);     // Mark the PC with '_' and go to right

    for(i=0; i<4; i++){

        for(j=0; j<i; j++){

            // Skip PC bit and constant bit
            rule = nextRIGHT(rule);             // Skip PC and next constant bit
            rule = nextRIGHT(rule);         
        }

        // Read current bit in PC and branch
        SetRule(rule, '0', '0', RIGHT, rule+3+i);
        SetRule(rule, '1', '1', RIGHT, rule+1);
        rule++;

        // Readed bit was a 1
        // Skip to M3
        rule = next$RIGHT(rule); 

        for(j=0; j<i; j++){

            // Skip jx bit in MAR
            rule = nextRIGHT(rule);  
        }

        // Write a 1 into current bit of MAR
        SetRule(rule, '0', '1', RIGHT, rule+3+i);
        SetRule(rule, '1', '1', RIGHT, rule+3+i);  
        rule++;

        // Readed bit was a 0
        // Seek to M3
        rule = next$RIGHT(rule); 

        for(j=0; j<i; j++){

            // Skip jx bit in MAR
            rule = nextRIGHT(rule);  
        }

        // Write a 0 into current bit of MAR
        SetRule(rule, '0', '0', RIGHT, rule+1);
        SetRule(rule, '1', '0', RIGHT, rule+1);  
        rule++;

        // Jump back to P3 into the marked PC register
        rule = next_LEFT(rule);
    }

    // Change the last rule to remark PC
    SetRule(rule-1, '_', '$', RIGHT, rule);     // Remark PC

    return rule;
}


// Increment the interleaved Programm counter
uint16_t IncPC(uint16_t rule) {

    // Entrypoint: NP pointer position: P3 of Program Counter PC
    // Exitpoint:  NP pointer position: P3 of Program Counter PC

    rule = next$RIGHT(rule, LEFT);                  // Skip to C1
    SetRule(rule-1, '$', '$', LEFT, rule+1);        // Next rule is NoCarry

    SetRule(rule, '0', '0', LEFT, rule+1);          // SUM = 0
    SetRule(rule, '1', '1', LEFT, rule+1);
    rule++;

    SetRule(rule, '0', '0', LEFT, rule-1);          // No Carry
    SetRule(rule, '1', '1', LEFT, rule+1);
    SetRule(rule, '$', '$', RIGHT, rule+4);         // Down without carry
    rule++;

    SetRule(rule, '0', '1', LEFT, rule-1);          // SUM = 1
    SetRule(rule, '1', '0', LEFT, rule+1);
    rule++;

    SetRule(rule, '0', '0', LEFT, rule-1);          // Carry
    SetRule(rule, '1', '1', LEFT, rule+1);
    SetRule(rule, '$', '$', RIGHT, rule+2);         // Down with carry
    rule++;

    SetRule(rule, '0', '0', LEFT, rule-1);          // SUM = 2
    rule++;

    return rule;
}


// Kopiert den Befehl (Bit 7 bis 4) aus der Speicherstelle, auf die
// der PC zeigt in das Instruction Register IR
void INSTtoIR(uint16_t rule){

    // Entrypoint: NP pointer position: P3 of Program Counter PC
    // Exitpoint:  NP pointer position: I3 of Instruction Registers IR

    uint8_t i, j;  

    // ------------- Mark the IR -------------
    rule = next$LEFT(rule);                     // Skip MAR
    rule = next$LEFT(rule);                     // Skip PC
    SetRule(rule-1, '$', '_', RIGHT, rule);     // Mark IR

    // ------- Skip to MAR and Mark MAR -------
    rule = next$RIGHT(rule);                    // Skip IR
    rule = next$RIGHT(rule);                    // Skip PC
    SetRule(rule-1, '$', '_', RIGHT, rule);     // Mark MAR

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
}


uint16_t GenerateRuleFETCH(uint16_t rule) {

    // Entrypoint: NP pointer position: M0 of Memory Address Register MAR

    rule = PCtoMAR(rule);   // Copy Program Counter into MAR
    rule = IncPC(rule);     // Increment Program Counter
    INSTtoIR(rule);         // Copy the current instruction to Register IR

    // Exitpoint:  NP pointer position: I3 of Instruction Registers IR
    
    return rule;
}