#ifndef NOS_GPIO
#define NOS_GPIO
#include "NOS_Includes.h"
typedef enum GPIO_PIN_Mode_e{Analog,Input,Output} GPIO_PIN_Mode;

void NOS_GPIO_PinInit(GPIO_PIN* gpin,GPIO_TypeDef* port,uint16_t pin,GPIO_PIN_Mode mode);

#endif