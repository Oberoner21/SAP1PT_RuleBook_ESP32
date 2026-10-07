/*
    SAP1PT - SAP1 Pure Turing
    Modul:  cpyMMBToAB.h

    Copyright(C) 2026 by Oberoner
*/

/*
    Copies the value from a marked memory byte (MMB) into 
    the interleaved register A/B, default destination is register A
    Entry conditions: Register A/B and MMB is marked
    Entry point     : NP pointer position bit7 of the MMB
    Exit point      : NP pointer position bit7 of the register AB
                      MMB and register A/B are remarked 
    Rules           : Destination register A: 205
                      Destination register B: 221
*/
uint16_t CopyMMBToAB(uint16_t rule, bool toB = false) {

    uint8_t i, j, offset;

    if(toB) offset = 5;
    else offset = 4;

    for(i=0; i<8; i++) {

        for(j=0; j<i; j++){
 
            // Skip jx right in marked memory byte 
            rule = nextRIGHT(rule);     
        }

        // Read current bit of MMB and branche
        SetRule(rule, '0', '0', RIGHT, rule+1);
        SetRule(rule, '1', '1', RIGHT, rule+offset+i*2);
        rule++;

        // Seek left to the memory byte marker
        rule = next_LEFT(rule, LEFT); 

        // Seek to A7 of the marked register A/B 
        rule = next_LEFT(rule);

        if(toB) {
            // Skip right to B7
            rule = nextRIGHT(rule);
        }

        for(j=0; j<i; j++){
 
            // Skip jx bit right in A/B
            rule = nextRIGHT(rule); 
            rule = nextRIGHT(rule); 
        }

        // Write a 0 into current bit of B
        SetRule(rule, '0', '0', RIGHT, rule+offset+i*2);
        SetRule(rule, '1', '0', RIGHT, rule+offset+i*2);
        rule++;

        // Seek to memory byte marker 
        rule = next_LEFT(rule, LEFT);
        // Seek to the market destination register A/B
        rule = next_LEFT(rule);

        if(toB) {
            // Skip right to B7
            rule = nextRIGHT(rule);
        }

        for(j=0; j<i; j++){
 
            // Skip jx bit right in A/B
            rule = nextRIGHT(rule);    
            rule = nextRIGHT(rule);   
        }

        // Write a 1 into current bit of B
        SetRule(rule, '0', '1', RIGHT, rule+1);
        SetRule(rule, '1', '1', RIGHT, rule+1);
        rule++;
        
        // Seek back to Bit7 of the marked byte 
        rule = next_RIGHT(rule);
    }

    // Change last rule
    // Remark the marked memory byte
    SetRule(rule-1, '_', '$', LEFT, rule);

    // Seek back to marked register A/B and remark it
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);

    return rule;
}