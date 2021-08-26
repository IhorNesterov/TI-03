#ifndef WS2812B_MATRIX
#define WS2812B_MATRIX
#include "NOS_Typedefs.h"

void NOS_WS2812B_Matrix_Init(WS2812B_Matrix* matrix,uint8_t* buff,uint16_t ledsCount); //First initialization of matrix parameters

void NOS_WS2812B_Matrix_SetPixel(WS2812B_Matrix* matrix,PixelColor* color,uint16_t pixelPos); //Set Pixel in Leds strip by pos number

void NOS_WS2812B_Matrix_Update(WS2812B_Matrix* matrix,uint8_t offset); //Update frame. For stable work do it every 35-50 ms in while

#endif