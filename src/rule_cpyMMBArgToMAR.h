/*
    SAP1PT - SAP1 Pure Turing
    Modul:  cpyMMBArgToMAR.h

    Copyright(C) 2026 by Oberoner
*/

/*
    Copies the argument (low nibble) from a marked memory byte (MMB) into 
    the Memory Address Register MAR.
    Entry conditions: PC and MMB is marked
    Entry point     : NP pointer position bit7 of the MMB
    Exit point      : NP pointer position one position left from marker of the MMB. 
                      The MMB is now remarked. 
    Rules           : 66 
*/
uint16_t CopyMMBArgToMAR(uint16_t rule) {

    uint8_t i, j;

    // ---------- Copy low nibble of MMB into MAR  -----------

    for(i=0; i<4; i++) {

        // Skip 4 byte right to bit3 of the marked memory register
        rule = nextRIGHT(rule);
        rule = nextRIGHT(rule);
        rule = nextRIGHT(rule);
        rule = nextRIGHT(rule);

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

        // Skip to bit3 of the marked register MAR 
        rule = next_LEFT(rule);

        for(j=0; j<i; j++){
 
            // Skip jx bit right in MAR
            rule = nextRIGHT(rule);     
        }

        // Write a 0 into current bit of MAR
        SetRule(rule, '0', '0', RIGHT, rule+4+i);
        SetRule(rule, '1', '0', RIGHT, rule+4+i);
        rule++;

    
        // Skip to memory byte marker 
        rule = next_LEFT(rule, LEFT);
        // Skip the market destination register MAR 
        rule = next_LEFT(rule);

        for(j=0; j<i; j++){
 
            // Skip jx bit right in MAR
            rule = nextRIGHT(rule);     
        }

        // Write a 1 into current bit of MAR
        SetRule(rule, '0', '1', RIGHT, rule+1);
        SetRule(rule, '1', '1', RIGHT, rule+1);
        rule++;

        // Skip back to Bit7 of the marked byte 
        rule = next_RIGHT(rule);
    }
    // Change last rule
    // Remark the marked memory byte
    SetRule(rule-1, '_', '$', LEFT, rule);

    return rule;
}