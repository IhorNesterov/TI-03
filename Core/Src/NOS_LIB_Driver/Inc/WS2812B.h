#ifndef WS2812B_MATRIX
#define WS2812B_MATRIX
#include "NOS_Typedefs.h"


void NOS_WS2812B_Matrix_Init(WS2812B_Matrix* matrix,uint8_t* buff,uint16_t ledsCount); //First initialization of matrix parameters

void NOS_WS2812B_Matrix_SetPixel(WS2812B_Matrix* matrix,PixelColor* color,uint16_t pixelPos); //Set Pixel in Leds strip by pos number

void NOS_WS2812B_Matrix_Update(WS2812B_Matrix* matrix,uint16_t offset); //Update frame. For stable work do it every 35-50 ms in while

void NOS_WS2812B_Matrix_SetSymvol(WS2812B_Matrix* matrix,char sym,uint8_t pos);

void NOS_WS2812B_Matrix_PrintStaticString(WS2812B_Matrix* matrix,char string[],uint8_t startpos,uint8_t lenght);

void NOS_WS2812B_Matrix_PrintIntNumber(WS2812B_Matrix* matrix,int num,uint8_t startpos,uint8_t lenght);

void NOS_WS2812B_Matrix_PrintFloatNumber(WS2812B_Matrix* matrix,float num,uint8_t startpos);

uint8_t* Symvol_GetBitMap(char sym);

uint16_t GetPixelPos(uint8_t x, uint8_t y);

#endif