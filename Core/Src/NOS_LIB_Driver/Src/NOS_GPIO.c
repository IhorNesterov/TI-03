#include "NOS_GPIO.h"

void NOS_GPIO_PinInit(GPIO_PIN* gpin,GPIO_TypeDef* port,uint16_t pin,GPIO_PIN_Mode mode)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    gpin->Port = port;
    gpin->Pin = pin;
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}