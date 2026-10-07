/*
    SAP1PT - SAP1 Pure Turing
    Modul:  cpyAtoMMB.h

    Copyright(C) 2026 by Oberoner
*/

/*
    Copies the A Register into marked memory byte MMB or marked Output register
    Entry conditions: A/B and MMB is marked
    Entry point     : NP pointer position A7 of A/B Register
    Exit point      : NP pointer position A7 of A/B Register
    Rules           : 66 
*/
uint16_t CopyAtoMMB(uint16_t rule) {

    uint8_t i, j;

    // Copy interleaved A register into marked memory byte
    for(i=0; i<8; i++){

        for(j=0; j<i; j++){

            // Skip register bit and constant bit
            rule = nextRIGHT(rule);
            rule = nextRIGHT(rule);         
        }

        // Read current bit in A and branch
        SetRule(rule, '0', '0', RIGHT, rule+3+i);
        SetRule(rule, '1', '1', RIGHT, rule+1);
        rule++;

        // Readed bit was a 1
        // Seek to bit7 of the marked memory byte
        rule = next_RIGHT(rule); 

        for(j=0; j<i; j++){

            // Skip jx bit in marked memory byte
            rule = nextRIGHT(rule);  
        }

        // Write a 1 into current bit of marked memory byte
        SetRule(rule, '0', '1', RIGHT, rule+3+i);
        SetRule(rule, '1', '1', RIGHT, rule+3+i);  
        rule++;

        // Readed bit was a 0
        // Seek to bit7 of the marked memory byte
        rule = next_RIGHT(rule); 

        for(j=0; j<i; j++){

            // Skip jx bit in marked memory byte
            rule = nextRIGHT(rule);  
        }

        // Write a 0 into current bit of marked memory byte
        SetRule(rule, '0', '0', RIGHT, rule+1);
        SetRule(rule, '1', '0', RIGHT, rule+1);  
        rule++;

        // Skip left marked memory byte
        rule = next_LEFT(rule);
        SetRule(rule-1, '_', '_', LEFT, rule);

        // Jump back to A7 of register A/B
        rule = next_LEFT(rule);
    }

    return rule;
}