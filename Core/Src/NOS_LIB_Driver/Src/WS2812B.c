#include "WS2812B.h"
#include "CharLib(7x5).h"
#include <stdbool.h>
#include "NOS_Math.h"

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

uint16_t GetPixelPos(uint8_t x, uint8_t y)
{
    if (x % 2 == 0)
    {
        return (x * 8) + y;
    }
    else
    {
        return(x * 8) + (8 - y);
    }
}

void NOS_WS2812B_Matrix_Update(WS2812B_Matrix* matrix,uint16_t offset)
{
     uint16_t currColumn = offset;
     uint8_t currByte = 0;
     bool bit = false;
     uint8_t symvolCount = matrix->size->col / 6;
     //kastil
     if(symvolCount > 15)
     {
        symvolCount--;
     }
     int currX = 0;

     for(int currSymvol = 0; currSymvol < symvolCount; currSymvol++)
     {
        for(int Column = 0; Column < 5 ; Column++)
        {
            currByte = matrix->symvols[currSymvol].data[Column];

            if(currColumn % 2 == 0)
            {
               currX = 0;
               for(int currBit = 0; currBit < 8; currBit++)
               {
                   bit = ((1 << currBit) & currByte);

                   if(bit)
                   {
                       NOS_WS2812B_Matrix_SetPixel(matrix,matrix->textColor,GetPixelPos(NOS_Math_NormalizeValue(currColumn,matrix->size->col),currX));
                   }
                   else
                   {
                       NOS_WS2812B_Matrix_SetPixel(matrix,matrix->foneColor,GetPixelPos(NOS_Math_NormalizeValue(currColumn,matrix->size->col),currX));
                   }
                   currX++;
                }
            }
            else
            {
               currX = 1;
               for(int currBit = 0; currBit < 8; currBit++)
               {
                   bit = ((1 << currBit) & currByte);

                   if(bit)
                   {
                       NOS_WS2812B_Matrix_SetPixel(matrix,matrix->textColor,GetPixelPos(NOS_Math_NormalizeValue(currColumn,matrix->size->col),currX));
                   }
                   else
                   {
                       NOS_WS2812B_Matrix_SetPixel(matrix,matrix->foneColor,GetPixelPos(NOS_Math_NormalizeValue(currColumn,matrix->size->col),currX));
                   }
                   currX++;
                }

            }
            currColumn++;
        }

        for(int i = 1; i < 9; i++)
        {
            NOS_WS2812B_Matrix_SetPixel(matrix,matrix->foneColor,GetPixelPos(NOS_Math_NormalizeValue(currColumn,matrix->size->col),i));
        }
        currColumn++;
     }
}

void NOS_WS2812B_Matrix_SetSymvol(WS2812B_Matrix* matrix,char sym,uint8_t pos)
{
    uint8_t* buff = Symvol_GetBitMap(sym);
    for(int i = 0; i < 5; i++)
    {
        matrix->symvols[pos].data[i] = buff[i];
    }
}

void NOS_WS2812B_Matrix_PrintStaticString(WS2812B_Matrix* matrix,char string[],uint8_t startpos,uint8_t lenght)
{
    for(int i = startpos; i < startpos + lenght; i++)
    {
        NOS_WS2812B_Matrix_SetSymvol(matrix,string[i - startpos],i);
    }
}

void NOS_WS2812B_Matrix_PrintIntNumber(WS2812B_Matrix* matrix,int num,uint8_t startpos,uint8_t lenght)
{

    int probe = 10;
    int a = 0;
    int power = NOS_Math_GetNumberPower(num);
    char buff[16];


    for (int i = power - 1; i > 0; i--)
    {
        buff[a] = num / pow(10,i);
        num = num - (buff[a] * pow(10,i));
        a++;
    }

    buff[a] = num % 10;

    for(int i = 0; i < a + 1; i++)
    {
        buff[i] += 48;
        NOS_WS2812B_Matrix_SetSymvol(matrix,buff[i],i + startpos);
    }
}

void NOS_WS2812B_Matrix_PrintFloatNumber(WS2812B_Matrix* matrix,float num,uint8_t startpos)
{
    int before = num;
    float after = num - (float)before;
    int af = after * 100;
    int pow = NOS_Math_GetNumberPower(before);
    NOS_WS2812B_Matrix_PrintIntNumber(matrix,before,startpos,pow);
    NOS_WS2812B_Matrix_PrintStaticString(matrix,".",startpos + pow,1);
    NOS_WS2812B_Matrix_PrintIntNumber(matrix,af,startpos + pow + 1,2);
}

void NOS_WS2812B_Matrix_PrintRealTime(WS2812B_Matrix* matrix,RealTime* rt)
{
    int startpos = 0;
    if(rt->format == Hour24)
    {
        startpos = 1;
    }

    if(rt->format == Hour12)
    {
        if(rt->hour > 12)
        {
            NOS_WS2812B_Matrix_SetSymvol(matrix,(char)(rt->hour - 12) / 10 + 48,startpos);
            NOS_WS2812B_Matrix_SetSymvol(matrix,(char)(rt->hour - 12) % 10 + 48,startpos + 1);
            NOS_WS2812B_Matrix_SetSymvol(matrix,'P',startpos + 8);
            NOS_WS2812B_Matrix_SetSymvol(matrix,'M',startpos + 9);
        }
        else
        {
            NOS_WS2812B_Matrix_SetSymvol(matrix,(char)rt->hour / 10 + 48,startpos);
            NOS_WS2812B_Matrix_SetSymvol(matrix,(char)rt->hour % 10 + 48,startpos + 1);
            NOS_WS2812B_Matrix_SetSymvol(matrix,'A',startpos + 8);
            NOS_WS2812B_Matrix_SetSymvol(matrix,'M',startpos + 9);
        }
    }
    else
    {
            NOS_WS2812B_Matrix_SetSymvol(matrix,(char)rt->hour / 10 + 48,startpos);
            NOS_WS2812B_Matrix_SetSymvol(matrix,(char)rt->hour % 10 + 48,startpos + 1);
            NOS_WS2812B_Matrix_SetSymvol(matrix,' ',startpos + 8);
            NOS_WS2812B_Matrix_SetSymvol(matrix,' ',startpos + 9);
    }

    NOS_WS2812B_Matrix_SetSymvol(matrix,(char)(rt->min/10) + 48,startpos + 3);
    NOS_WS2812B_Matrix_SetSymvol(matrix,(char)(rt->min%10) + 48,startpos + 4);

    NOS_WS2812B_Matrix_SetSymvol(matrix,(char)rt->sec/10 + 48,startpos + 6);
    NOS_WS2812B_Matrix_SetSymvol(matrix,(char)rt->sec%10 + 48,startpos + 7);

    if(rt->sec % 2 == 0)
    {
        NOS_WS2812B_Matrix_SetSymvol(matrix,':',startpos + 2);
        NOS_WS2812B_Matrix_SetSymvol(matrix,':',startpos + 5);
    }
    else
    {
        NOS_WS2812B_Matrix_SetSymvol(matrix,' ',startpos + 2);
        NOS_WS2812B_Matrix_SetSymvol(matrix,' ',startpos + 5);
    }
}

void NOS_WS2812B_SetPixelColor(PixelColor* color,uint8_t R,uint8_t G,uint8_t B)
{
    color->R = R;
    color->G = G;
    color->B = B;
}

void NOS_WS2812B_EffectRainbow(WS2812B_Matrix*matrix,uint32_t counter)
{

    if(counter > 0 && counter < matrix->bright)
    {
        NOS_WS2812B_SetPixelColor(matrix->textColor,0,counter,matrix->bright);
    }

    if(counter > matrix->bright && counter < matrix->bright * 2)
    {
        counter = counter - matrix->bright;
        NOS_WS2812B_SetPixelColor(matrix->textColor,0,matrix->bright,matrix->bright - counter);
    }

    if(counter > matrix->bright * 2 && counter < matrix->bright * 3)
    {
        counter = counter - matrix->bright * 2;
        NOS_WS2812B_SetPixelColor(matrix->textColor,counter,matrix->bright,0);
    }

    if(counter > matrix->bright * 3 && counter < matrix->bright * 4)
    {
                counter = counter - matrix->bright * 3;
        NOS_WS2812B_SetPixelColor(matrix->textColor,matrix->bright,matrix->bright - counter,0);
    }

    if(counter > matrix->bright * 4 && counter < matrix->bright * 5)
    {
                counter = counter - matrix->bright * 4;
        NOS_WS2812B_SetPixelColor(matrix->textColor,matrix->bright,counter,0);
    }

    if(counter > matrix->bright * 5 && counter < matrix->bright * 6)
    {
                counter = counter - matrix->bright * 5;
        NOS_WS2812B_SetPixelColor(matrix->textColor,matrix->bright - counter,matrix->bright,0);
    }

    if(counter > matrix->bright * 6 && counter < matrix->bright * 7)
    {        
        counter = counter - matrix->bright * 6;
        NOS_WS2812B_SetPixelColor(matrix->textColor,0,matrix->bright,counter);
    }

        if(counter > matrix->bright * 7 && counter < matrix->bright * 8)
    {        
        counter = counter - matrix->bright * 7;
        NOS_WS2812B_SetPixelColor(matrix->textColor,0,matrix->bright - counter,matrix->bright);
    }
}

uint8_t* Symvol_GetBitMap(char sym)
{
    switch (sym)
    {
/* English upper case */
case 'A':
    return Eng_A;
    break;
case 'B':
    return Eng_B;
    break;
case 'C':
    return Eng_C;
    break;
case 'D':
    return Eng_D;
    break;
case 'E':
    return Eng_E;
    break;
case 'F':
    return Eng_F;
    break;
case 'G':
    return Eng_G;
    break;
case 'H':
    return Eng_H;
    break;
case 'I':
    return Eng_I;
    break;
case 'J':
    return Eng_J;
    break;
case 'K':
    return Eng_K;
    break;
case 'L':
    return Eng_L;
    break;
case 'M':
    return Eng_M;
    break;
case 'N':
    return Eng_N;
    break;
case 'O':
    return Eng_O;
    break;
case 'P':
    return Eng_P;
    break;
case 'Q':
    return Eng_Q;
    break;
case 'R':
    return Eng_R;
    break;
case 'S':
    return Eng_S;
    break;
case 'T':
    return Eng_T;
    break;
case 'U':
    return Eng_U;
    break;
case 'V':
    return Eng_V;
    break;
case 'W':
    return Eng_W;
    break;
case 'X':
    return Eng_X;
    break;
case 'Y':
    return Eng_Y;
    break;
case 'Z':
    return Eng_Z;
    break;
/* English upper case */

/* English lower case */
case 'u':
    return Eng_u;
    break;
case 'v':
    return Eng_v;
    break;
case 'a':
    return Eng_a;
    break;
case 'c':
    return Eng_c;
    break;
/* English lower case */

case 'm':
    return Ukr_m;
    break;
case 'k':
    return Ukr_k;
    break;
case 'd':
    return Ukr_d;
    break;    
case 'o':
    return Eng_o;
    break;     
 case 'g':
    return Ukr_g;
    break;       
case 'y':
    return Ukr_ch;
    break;

/* Special symvols */
case ' ':
    return Space;
    break;
case '+':
    return Plus;
    break;
case '-':
    return Minus;
    break;    
case '/':
    return Slash;
    break;
case '*':
    return Celcium;
    break;
case '.':
    return Dot;
    break;
case ':':
    return DoubleDot;
    break;



/* Special symvols */

/* Numbers */
case '0':
    return Num_0;
    break;
    case '1':
    return Num_1;
    break;
    case '2':
    return Num_2;
    break;
    case '3':
    return Num_3;
    break;
    case '4':
    return Num_4;
    break;
    case '5':
    return Num_5;
    break;
    case '6':
    return Num_6;
    break;
    case '7':
    return Num_7;
    break;
    case '8':
    return Num_8;
    break;
    case '9':
    return Num_9;
    break;
/* Numbers */

    default:
    return Space;
        break;
    }
}

