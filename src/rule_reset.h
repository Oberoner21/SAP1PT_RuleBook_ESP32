/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_reset.h

    Copyright(C) 2026 by Oberoner
*/

/*
    Reset procedure to start the machine
    Entry conditions: Nothing is marked
    Entry point     : NP pointer position 0
    Exit point      : NP pointer position 0 
    Rules           : 87 
*/

void GenerateRuleRESET(uint16_t rule, uint8_t USE_FSM) {

    if(USE_FSM) {
        // Copy Notepad from EPROM into SRAM
        SetRule(rule, '_', '_', RIGHT, rule);
        SetRule(rule, '0', '0', RIGHT, rule);  
        SetRule(rule, '1', '1', RIGHT, rule);
        SetRule(rule, '$', '$', RIGHT, rule+1);
        rule++;

        SetRule(rule, '_', '_', RIGHT, rule-1);
        SetRule(rule, '0', '0', RIGHT, rule-1);  
        SetRule(rule, '1', '1', RIGHT, rule-1);
        SetRule(rule, '$', '$', RIGHT, rule+1);
        rule++;

        SetRule(rule, '_', '_', RIGHT, rule-2);
        SetRule(rule, '0', '0', RIGHT, rule-2);  
        SetRule(rule, '1', '1', RIGHT, rule-2);
        SetRule(rule, '$', '$', RIGHT, rule+1);
        rule++;
    }
    // After EPROM copy the Notepad pointer is on position 0
    // Entrypoint: NP pointer position: 0
    // Exitpoint : NP pointer position: M0 (MAR bit 0)

    // ------------ Skip to position MAR0 -------------

    rule = next$RIGHT(rule);                        // Skip first $
    rule = next$RIGHT(rule);                        // Skip second $
    rule = next$RIGHT(rule);                        // Skip the flags
    rule = next$RIGHT(rule);                        // Skip the A/B Register
    rule = next$RIGHT(rule);                        // Skip the Program Counter
    rule = next$RIGHT(rule);                        // Skip the Instruction Register

    rule = next$RIGHT(rule);                        // Skip the MAR and go to rule FETCH
    SetRule(rule-1, '$', '$', LEFT, RULE_FETCH);
}