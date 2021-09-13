#include "NOS_Math.h"

int NOS_Math_NormalizeValue(int value,int maxValue)
{
    while(value > maxValue)
    {
        value = value - maxValue;
    }

    return value;
}