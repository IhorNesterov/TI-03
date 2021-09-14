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
typedef struct Point_t
{
  uint16_t x;
  uint16_t y;
}Point;


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

typedef struct  Symvol_t
{
  uint8_t data[5];
} Symvol;


typedef struct WS2812B_Matrix_t
{
    uint8_t* buffer;
    NOS_Short ledsCount;
    MatrixSize* size;
    PixelColor* textColor;
    PixelColor* foneColor;
    Symvol* symvols;
} WS2812B_Matrix;

/*WS2812B Matrix*/

typedef enum TimeFormat_e{Hour24,Hour12} TimeFormat;

typedef struct RealTime_t
{
  uint16_t milliseconds;
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
  TimeFormat format;
} RealTime;


/* Structures end */
#endif