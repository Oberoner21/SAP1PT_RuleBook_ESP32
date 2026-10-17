/*
    SAP1PT - SAP1 Pure Turing
    Modul:  rule_cpyINSTtoIR.h

    Copyright(C) 2026 by Oberoner

    Copies the instruction of the current program step into the IR.
    The instruction is stored in the high nibble of the opcode.
    Entry conditions: Nothing is marked
    Entry point     : NP pointer position P3 of Program Counter PC
    Exit conditions : IR and MMB are marked, jump to RULE_DECODE
    Exit point      : NP pointer position I3 of Instruction Registers IR
    Rules           : 64 
*/

void CopyINSTtoIR(uint16_t rule){

    // 0 Skip PC
    rule = next$RIGHT(rule);

    // 1 Skip IR
    rule = next$RIGHT(rule, LEFT);

    // 2 Mark I0
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    rule++;

    // 3 Mark MAR and seek to M3
    rule = next$RIGHT(rule);
    SetRule(rule-1, '$', '_', RIGHT, rule);

    // 3 Mark the memory byte, addressed by register MAR
    rule = MarkMemoryByte(rule);

    // 4 Skip one position left to the MMB marker (Kompatinilität!!!)
    rule = nextLEFT(rule);

    // 5 Remark MMB and skip right to bit7 of MMB
    rule = nextRIGHT(rule);
    SetRule(rule-1, '_', '$', RIGHT, rule); // Remark MMB (Kompatibilität !!!)

    // 6 Skip MMB one position to right to bit6
    rule = nextRIGHT(rule);
    // 7 Skip MMB one position to right to bit5
    rule = nextRIGHT(rule);
    // 8 Skip MMB one position to right to bit4
    rule = nextRIGHT(rule);

    // 9 Read current bit of MMB, mark it and branch
    SetRule(rule, '0', '_', LEFT, rule+1);
    SetRule(rule, '1', '_', LEFT, rule+4);
    SetRule(rule, '$', '_', LEFT, rule+7);   // Exit point
    rule++;

    // 10 Seek left to the marked bit of MAR, remark it and write 0
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '0', LEFT, rule);

    // 11 Skip left next byte of IR and mark it
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++; 

    // 12 Seek right to the marked bit of MMB, write 0 and repeat with next bit
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '0', LEFT, rule-4);

    // 13 Seek left to the marked bit of MAR, remark it and write 1
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '1', LEFT, rule);

    // 14 Skip right next byte of IR and mark it
    SetRule(rule, '0', '_', RIGHT, rule+1);
    SetRule(rule, '1', '_', RIGHT, rule+1);
    SetRule(rule, '$', '_', RIGHT, rule+1);
    rule++; 

    // 15 Seek right to the marked bit of MMB, write 1 and repeat with next bit
    rule = next_RIGHT(rule);
    SetRule(rule-1, '_', '1', LEFT, rule-7);

    // -------- Exit point ----------

    // 16 Seek to M3 and jump to RULE_DECODE
    rule = next_LEFT(rule);
    SetRule(rule-1, '_', '_', RIGHT, RULE_DECODE);

}
