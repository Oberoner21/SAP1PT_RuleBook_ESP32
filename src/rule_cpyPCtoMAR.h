/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_cpyPCtoMAR.h

    Copyright(C) 2026 by Oberoner

    Copies the value from interleaved PC into MAR
    Entry conditions: Nothing is marked
    Entry point     : NP pointer position M0 of the register MAR
    Exit point      : NP pointer position I0 of the Instruction register IR
    Rules           : 56 
*/

uint16_t CopyPCtoMAR(uint16_t rule){

    uint8_t i, j;

    // Skip MAR to left and mark it
    rule = next$LEFT(rule);
    SetRule(rule-1, '$', '_', RIGHT, rule);
    // Skip left IR
    rule = next$LEFT(rule);
    // Skip left PC and mark it
    rule = next$LEFT(rule);
    SetRule(rule-1, '$', '_', RIGHT, rule);

    // Copy the bits
    for(i=0; i<4; i++){

        for(j=0; j<i; j++){

            // Skip PC bit and constant bit
            rule = nextRIGHT(rule);             // Skip PC and next constant bit
            rule = nextRIGHT(rule);         
        }

        // Read current PC bit and branch
        SetRule(rule, '0', '0', RIGHT, rule+3+i);
        SetRule(rule, '1', '1', RIGHT, rule+1);
        rule++;

        // Readed bit was a 1
        // Skip to M3
        rule = next_RIGHT(rule); 

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
        rule = next_RIGHT(rule); 

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
        SetRule(rule-1, '_', '_', LEFT, rule);
        rule = next_LEFT(rule);
    }

    // Change the last rule to remark PC
    SetRule(rule-1, '_', '$', RIGHT, rule);     // Remark PC

    // Remark MAR and skip to left of the bit I0 of Instruction Register IR
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '$', LEFT, rule);

    return rule;
}