/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_ldi.h

    Copyright(C) 2026 by Oberoner
*/

/*
    Instruction rule for LDI
    Entry conditions: IR and current MMB is marked
    Entry point     : NP pointer position I0 of Instruction Register IR
    Exit point      : NP pointer position M0 of Memory Address Register MAR 
    Rules           : 215 
*/
void GenerateRuleLDI(uint16_t rule) {

    // Mark register A
    // Remark IR
    // Seek to marked memory byte bit 7
    // Copy the high nibble of marked byte as 0 into high nibble of A
    // Copy the low nibble as value into the low nibble of A
    // Remark the marked memory byte
    // Remark register A/B
    // Seek to bit M0 of Memory address register MR
    // Go back to RULE_FETCH

    uint8_t i, j;

    SetRule(rule, '_', '_', LEFT, rule+1);
    SetRule(rule, '0', '0', LEFT, rule+1);
    SetRule(rule, '1', '1', LEFT, rule+1);
    SetRule(rule, '$', '$', LEFT, rule+1);
    rule++;

    // Skip left IR (is yet market) and output register
    rule = next$LEFT(rule);

    // Skip left A/B register and mark it
    rule = next$LEFT(rule);
    SetRule(rule-1, '$', '_', RIGHT, rule);

    // Seek to IR marker and remark the register
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);

    // Seek right to marked memory byte to bit7
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '_', RIGHT, rule);

    // Copy marked byte into A/B 
    for(i=0; i<8; i++) {

        for(j=0; j<i; j++){
 
            // Skip jx right in marked memory byte 
            rule = nextRIGHT(rule);     
        }

        // Read current bit of marked memory Byte and branche
        SetRule(rule, '0', '0', RIGHT, rule+1);
        SetRule(rule, '1', '1', RIGHT, rule+4+i*2);
        rule++;

        // Seek left to the memory byte marker
        rule = next_LEFT(rule, LEFT); 

        // Seek to bit7 of the marked register A/B 
        rule = next_LEFT(rule);

        for(j=0; j<i; j++){
 
            // Skip jx bit right in A/B
            rule = nextRIGHT(rule); 
            rule = nextRIGHT(rule); 
        }

        // Write a 0 into current bit of A/B
        SetRule(rule, '0', '0', RIGHT, rule+4+i*2);
        SetRule(rule, '1', '0', RIGHT, rule+4+i*2);
        rule++;

    
        // Seek to memory byte marker 
        rule = next_LEFT(rule, LEFT);
        // Seek to the market destination register A/B
        rule = next_LEFT(rule);

        for(j=0; j<i; j++){
 
            // Skip jx bit right in A/B
            rule = nextRIGHT(rule);    
            rule = nextRIGHT(rule);   
        }

        if(i<4){

            // Write a 0 into the high nibble
            SetRule(rule, '0', '0', RIGHT, rule+1);
            SetRule(rule, '1', '0', RIGHT, rule+1);
            rule++;
        } else {

            // Write a 1 into current bit of A/B
            SetRule(rule, '0', '1', RIGHT, rule+1);
            SetRule(rule, '1', '1', RIGHT, rule+1);
            rule++;
        }

        // Seek back to Bit7 of the marked byte 
        rule = next_RIGHT(rule);
    }

    // Change the last rule
    // Seek left to marked memory byte and remarke it
    SetRule(rule-1, '_', '$', LEFT, rule);

    // Seek left to marked A/B register and remarke it
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule);

    // Skip right register A/B
    rule = next$RIGHT(rule);
    // Skip right Output register
    rule = next$RIGHT(rule); 
    // Skip right Instruction Register IR
    rule = next$RIGHT(rule);     
    // Skip right Program counter PC
    rule = next$RIGHT(rule); 
    // Skip right Memory Address Register MAR and go to RULE_FETCH
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH);  

}