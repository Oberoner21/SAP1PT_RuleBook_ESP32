/*
    SAP1PT - SAP1 Pure Turing
    Modul:  cpyMMBArgToPC.h

    Copyright(C) 2026 by Oberoner
*/

/*
    Copies the argument (low nibble) from a marked memory byte (MMB) into 
    the interleaved program counter.
    Entry conditions: PC and MMB is marked
    Entry point     : NP pointer position bit7 of the MMB
    Exit point      : NP pointer position one position left from marker of the MMB 
    Rules           : 78
*/
uint16_t CopyMMBArgToPC(uint16_t rule) {

    uint8_t i, j;

    // Copy low nibbel of MMB into the interleaved PC
    for(i=0; i<4; i++){

        // Skip to bit 3 of MMB
        rule = nextRIGHT(rule);
        rule = nextRIGHT(rule);
        rule = nextRIGHT(rule);
        rule = nextRIGHT(rule);

        for(j=0; j<i; j++){

            // Skip next left MMB bit
            rule = nextRIGHT(rule);       
        }

        // Read current bit in MMB and branch
        SetRule(rule, '0', '0', RIGHT, rule+4+i*2);
        SetRule(rule, '1', '1', RIGHT, rule+1);
        rule++;

        // Readed bit was a 1
        // Seek to P3 of PC
        rule = next_LEFT(rule, LEFT); 
        rule = next_LEFT(rule);

        for(j=0; j<i; j++){

            // Skip jx bit in PC
            rule = nextRIGHT(rule);  
            rule = nextRIGHT(rule);
        }

        // Write a 1 into current bit of PC
        SetRule(rule, '0', '1', RIGHT, rule+4+i*2);
        SetRule(rule, '1', '1', RIGHT, rule+4+i*2);  
        rule++;

        // Readed bit was a 0
        // Seek to P3 of PC
        rule = next_LEFT(rule, LEFT); 
        rule = next_LEFT(rule);

        for(j=0; j<i; j++){

            // Skip jx bit in PC
            rule = nextRIGHT(rule);  
            rule = nextRIGHT(rule);
        }

        // Write a 0 into current bit of PC
        SetRule(rule, '0', '0', RIGHT, rule+1);
        SetRule(rule, '1', '0', RIGHT, rule+1);  
        rule++;

        // Jump back to bit7 of MMB
        rule = next_RIGHT(rule);
    }

    // Change the last rule
    SetRule(rule-1, '_', '_', LEFT, rule);

    return rule;
}