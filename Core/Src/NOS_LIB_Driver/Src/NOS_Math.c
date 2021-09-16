#include "NOS_Math.h"

int NOS_Math_NormalizeValue(int value,int maxValue)
{
    while(value >= maxValue)
    {
        value = value - maxValue;
    }
    

    return value;
}

int NOS_Math_GetNumberPower(int value)
{
    int probe = 10;
    int power = 1;
    while (value >= probe)
    {
        power++;
        probe = probe * 10;
    }
    return power;
}