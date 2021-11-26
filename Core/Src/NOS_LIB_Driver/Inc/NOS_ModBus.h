#ifndef NOS_MODBUS
#define NOS_MODBUS
#include "NOS_Includes.h"

uint16_t GetCRC16 (uint8_t *nData, uint8_t wLength);
void NOS_ModBus_ParseMasterCommand(ModBus_Master_Command* master,uint8_t* buff,uint8_t offset);
void NOS_ModBus_ParseSlaveCommand(ModBus_Slave_Command* slave,uint8_t* buff,uint8_t offset);

void NOS_ModBus_SetMasterCommand(ModBus_Master_Command* master,uint8_t addr,uint8_t command,uint16_t regAddr, uint16_t data);
void NOS_ModBus_SetSlaveCommand(ModBus_Slave_Command* slave,uint8_t addr,uint8_t command,uint8_t byteCount,uint8_t typ,NOS_Short sVal,NOS_Float fVal);

void NOS_ModBus_ReceiveHandler(ModBus_Struct* MB_struct);
void NOS_ModBus_InitStruct(ModBus_Struct* mb,UART_HandleTypeDef* huart,uint8_t* _buff,GPIO_PIN pin);

void NOS_ModBus_AddDevice(ModBus_Struct* mb,Modbus_Device* device);
void NOS_ModBus_DeleteDevice(ModBus_Struct* mb,uint8_t pos);

//void NOS_ModBus_SendMasterCommand(ModBus_Struct* mb);

//void NOS_ModBus_InitDevice(Modbus_Device* md,uint8_t _addr);
void NOS_ModBus_DeviceAddRegisterAddress(Modbus_Device* mb,uint16_t rAddr);
#endif 