#ifndef BitOperations
#define BitOperations
#include "NOS_Includes.h"

bool GetBitByPos(uint16_t value,uint8_t pos)
{
    if(value & (1 << pos))
    {
        return true;
    }
    return false;
}

void SetBitByPos(uint16_t value,bool val,uint8_t pos)
{
    if(val)
    {
        value |= (1 << pos);
    }
    else
    {
        value &= ~(1 << pos);
    }
}



#endif