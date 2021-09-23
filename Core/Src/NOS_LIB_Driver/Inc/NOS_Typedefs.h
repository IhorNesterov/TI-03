#ifndef NOS_TYPEDEFS
#define NOS_TYPEDEFS
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"

/* Unions begin */

typedef union Short_t //uint16_t data wrap
{ 
    uint8_t bytes[2];
    uint16_t data;
} NOS_Short;

typedef union Float_t //float data wrap
{
    uint8_t bytes[4];
    float data;
} NOS_Float;

typedef union Long_t
{
  uint8_t bytes[4];
  uint32_t data;
} NOS_Long;

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
    uint8_t bright;
    NOS_Short ledsCount;
    MatrixSize* size;
    PixelColor* textColor;
    PixelColor* foneColor;
    Symvol* symvols;
} WS2812B_Matrix;

typedef enum WS2812B_EffectEncoder_e {NONE,Breathe,Rainbow} WS2812B_EffectEncoder;

typedef struct WS2812B_Effect_t
{
  WS2812B_Matrix* matrix;
  WS2812B_EffectEncoder effect;
} WS2812B_Effect;

typedef struct WS2812B_EffectEngine_t
{
  WS2812B_Effect* effects;
  uint8_t effectsCount;
  uint32_t counter;
} WS2812B_EffectEngine;

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

typedef struct URE_Detector_t
{
  uint8_t Address;
  NOS_Float value;
  NOS_Float first_danger;
  NOS_Float second_danger;
  uint16_t voltage;
  NOS_Short temperature;
}URE_Detector;

/*ModBus*/
typedef enum ModBus_State_t{Free,ReceiveFromMaster,ReceiveFromSlave,TransmitToMaster,TransmitToSlave} ModBus_State;
typedef struct ModBus_Master_Command_t
{
    uint8_t Addr;
    uint8_t Command;
    uint8_t Byte_Count;
    uint16_t Reg_Addr;
    uint16_t Reg_Count;
    NOS_Short ShortValue;
    NOS_Float FloatValue;
} ModBus_Master_Command;

typedef struct ModBus_Slave_Command_t
{
    uint8_t type;
    uint8_t Addr;
    uint8_t Command;
    uint8_t Byte_Count;
    NOS_Short ShortValue;
    NOS_Float FloatValue;
} ModBus_Slave_Command;

typedef struct NOS_Modbus_Device_t
{
  uint8_t Descriptor;
  uint8_t Addr;
  uint8_t RegisterCount;
  NOS_Short* RegisterAddress;
} Modbus_Device;

typedef struct GPIO_PIN_t
{
  GPIO_TypeDef* Port;
  uint16_t Pin;
} GPIO_PIN;

typedef struct NOS_ModBus_Struct_t
{
  Modbus_Device devices[10];
  ModBus_Master_Command master;
  ModBus_Slave_Command slave;
  UART_HandleTypeDef* huart;
  uint8_t* buff;
  GPIO_PIN RW;
  ModBus_State state;
  uint8_t devicesCount;
  uint8_t messageLenght;
  uint8_t currCommand;
  uint8_t currDevice;
  uint8_t expectedMessageLenght;
  bool addressOk;
  bool rx_buff;
  bool tx_buff;
} ModBus_Struct;

/*ModBus*/

typedef enum Language_e{English,Ukrainian,Russian} Language;

/* STM32 Core structures begin*/



/* STM32 Core structures end*/

/* Structures end */
#endif