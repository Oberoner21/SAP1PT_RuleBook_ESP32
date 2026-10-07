#pragma once
#include <Arduino.h>

const char cSymbols[] = ("01$_");

uint8_t symbol2num(char symbol){

    uint8_t num;
    switch(symbol){
        
        case '0': num = 0;   break;
        case '1': num = 1;   break;
        case '$': num = 2;   break;
        case '_': num = 3;   break;
    }

    return num;
}