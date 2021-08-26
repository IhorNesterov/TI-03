#ifndef NOS_TYPEDEFS
#define NOS_TYPEDEFS
#include <stdint.h>
/* Unions begin */

typedef union Short_t //uint16_t data wrap
{ 
    uint8_t bytes[2];
    uint16_t data;
} NOS_Short;

typedef union Float_t //float data wrap
{
    uint8_t bytes[4];
    uint16_t data;
} NOS_Float;

/* Unions end */

/* Structures begin */

/*WS2812B Matrix*/
typedef struct PixelColor_t
{
   uint8_t R;
   uint8_t G;
   uint8_t B;
} PixelColor;

typedef struct MatrixSize_t
{
  uint8_t col;
  uint8_t row;
} MatrixSize;

typedef struct WS2812B_Matrix_t
{
    uint8_t* buffer;
    NOS_Short ledsCount;
    MatrixSize* size;
    PixelColor* textColor;
    PixelColor* foneColor;
} WS2812B_Matrix;



/*WS2812B Matrix*/

/* Structures end */
#endif