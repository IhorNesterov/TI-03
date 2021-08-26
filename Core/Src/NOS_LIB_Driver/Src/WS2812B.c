#include "WS2812B.h"

void NOS_WS2812B_Matrix_Init(WS2812B_Matrix* matrix,uint8_t* buff,uint16_t ledsCount)
{
     matrix->buffer = buff;
     matrix->ledsCount.data = ledsCount;
}

void NOS_WS2812B_Matrix_SetPixel(WS2812B_Matrix* matrix,PixelColor* color,uint16_t pixelPos)
{
    matrix->buffer[pixelPos * 3] = color->R;
    matrix->buffer[pixelPos * 3 + 1] = color->G;
    matrix->buffer[pixelPos * 3 + 2] = color->B;
}

/*
void NOS_WS2812B_Matrix_Update(WS2812B_Matrix* matrix,uint8_t offset)
{
    uint8_t symvolsCount = matrix->ledsCount.data / 48;

    for(uint8_t currSymvol = 0; currSymvol < symvolsCount; currSymvol++)
    {
        for(uint8_t currColumn = 0; currColumn < 6; currColumn++)
        {
            for(int currPixel = 0; currPixel < 8; currPixel++)
            {
                if(matrix->symvols[currSymvol].data[currColumn] & (1 << currPixel) && currColumn <= 5)
                {
                     NOS_WS2812B_Matrix_SetPixel(matrix,matrix->textColor,((currSymvol * 48) + ((offset + currColumn) * 8) + currPixel));
                }
                else
                {
                    NOS_WS2812B_Matrix_SetPixel(matrix,matrix->foneColor,((currSymvol * 48) + ((offset + currColumn) * 8) + currPixel));
                }
            }
        }
    }
}
*/