#ifndef NOS_MATH
#define NOS_MATH
#include "NOS_Includes.h"

void NOS_RealTime_Handler(RealTime* rt);
void NOS_RealTime_SetTime(RealTime* rt,uint8_t _hour,uint8_t _min,uint8_t _sec,TimeFormat _format);
#endif