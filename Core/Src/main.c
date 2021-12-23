/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <locale.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "NOS_Lib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint32_t Time = 0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
MatrixSize msA = {96,8};
MatrixSize msB = {64,8};
MatrixSize msC = {32,8};

RealTime realtime = {0};

extern uint8_t frameBuffer1[3*8*96];
extern uint8_t frameBuffer2[3*8*64];
extern uint8_t frameBuffer3[3*8*32];

WS2812B_Matrix matrixA = {0};
WS2812B_Matrix matrixB = {0};
WS2812B_Matrix matrixC = {0};

PixelColor red = {255,0x00,0x00};
PixelColor green = {0x00,255,0x00};
PixelColor yellow = {255,255,0x00};
PixelColor white = {255,255,255};
PixelColor blue = {0x00,0x00,255};
PixelColor fone = {0,0,0};

Symvol matrixAsymc[15];
Symvol matrixBsymc[10];
Symvol matrixCsymc[5];

uint8_t x = 1;
uint16_t y = 1;
uint32_t coun = 0;
int num = 0;

float uSv_Value = 0.11f;
float first_danger = 0.25f;
float second_danger = 1.25f;
int16_t temperature = 25;

uint32_t UpdateFrameTime = 25;
Language language = English;
RTC_TimeTypeDef sTime = {0};
ModBus_Struct ModBus;
GPIO_PIN PA1;
uint8_t rx_buff[16];
uint8_t tx_buff[16];
URE_Detector detector = {0};
ModBus_State mbState = Free;
uint8_t messageLenght = 0;
bool rx_flag;
bool tx_flag;
bool AddressOK = false;
uint8_t TIaddr = 102;
uint8_t currCommand = 0;
uint8_t expectedMessageLenght = 0;
uint8_t TxCommandCounter = 0;
uint8_t currMessageLenght = 0;
static uint8_t fuckBuff[1024] = {0};
static uint16_t fuckIndex = 0;
uint16_t timelastReceiveMessage = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback (UART_HandleTypeDef* huart) {
  //NOS_ModBus_ReceiveHandler(&ModBus);
static uint8_t* rx_buff_ptr = rx_buff;

fuckBuff[fuckIndex] = *rx_buff_ptr;
fuckIndex++;
if(fuckIndex > 1023)
{
  fuckIndex = 0;
}

if(*rx_buff_ptr == 101 && !AddressOK)
{
  AddressOK = true;
  currMessageLenght = 0;
}

if(currMessageLenght == 2 && AddressOK)
{
  expectedMessageLenght = *rx_buff_ptr + 4;
}

if(currMessageLenght > 10 || currMessageLenght > expectedMessageLenght && expectedMessageLenght != 0)
{
  rx_buff_ptr = rx_buff;
  currMessageLenght = 0;
  AddressOK = false;
  for(int i = 0; i < 10; i++)
  {
    rx_buff[i] = 0;
  }
}

if(AddressOK && currMessageLenght == expectedMessageLenght && expectedMessageLenght != 0)
{
  rx_buff_ptr = rx_buff;
  AddressOK = false;
  currMessageLenght = 0;
  rx_flag = true;
}
else
{
  rx_buff[currMessageLenght] = *rx_buff_ptr;
  ++rx_buff_ptr;
  currMessageLenght++;
}
    HAL_UART_Receive_IT (&huart2, rx_buff_ptr, 1); 
}


void NOS_ModBus_SendMasterCommand(ModBus_Struct* mb)
{
   uint8_t message[16];
   NOS_Short _CRC;
   message[0] = mb->master.Addr;
   message[1] = mb->master.Command;
   message[2] = mb->master.Reg_Addr.bytes[1];
   message[3] = mb->master.Reg_Addr.bytes[0];
   message[4] = mb->master.Reg_Count.bytes[1];
   message[5] = mb->master.Reg_Count.bytes[0];

   _CRC.data = GetCRC16(&message,6);

   message[6] = _CRC.bytes[0];
   message[7] = _CRC.bytes[1];

   HAL_UART_Transmit(&huart2,message,8,1000);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  setlocale(LC_ALL, "Rus");
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
//MX_RTC_Init();
 // MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  HAL_UART_Receive_IT(&huart2,rx_buff,1);
  MX_TIM6_Init();
  visInit();
  /* USER CODE BEGIN 2 */

  NOS_WS2812B_Matrix_FullInit(&matrixA,&frameBuffer1,&msA,&red,&fone,&matrixAsymc,40);
  NOS_WS2812B_Matrix_FullInit(&matrixB,&frameBuffer2,&msB,&yellow,&fone,&matrixBsymc,40);
  NOS_WS2812B_Matrix_FullInit(&matrixC,&frameBuffer3,&msC,&blue,&fone,&matrixCsymc,40);
  //NOS_ModBus_InitStruct(&ModBus,&huart2,NULL,PA1);
  //NOS_GPIO_PinInit(&PA1,GPIOA,GPIO_PIN_1,Output);
  uSv_Value = 1.26f;
  realtime.format = Hour24;
  detector.Address = 101;
  detector.value.data = 0.15f;
  detector.first_danger.data = 0.25f;
  detector.second_danger.data = 1.25f;
  for(int i = 0; i < 1023; i++)
  {
    fuckBuff[i] = 15;
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if(fuckBuff[10] == 12)
    {

    }

    if(rx_flag)
    {
        NOS_ModBus_ParseSlaveCommand(&ModBus.slave,rx_buff,0);
        switch (ModBus.master.Reg_Addr.data)
        {
        case 0x0001:
          detector.temperature.data = ModBus.slave.ShortValue.data;
          break;

        case 0x0002:
          detector.value.data = ModBus.slave.FloatValue.data;
          break;

        case 0x0004:
          detector.voltage = ModBus.slave.ShortValue.data;
          break;

        case 0x0005:
          detector.first_danger.data = ModBus.slave.FloatValue.data;
          break;

        case 0x0007:
          detector.second_danger.data = ModBus.slave.FloatValue.data;
          break;

        default:

          break;
        }
      
      timelastReceiveMessage = 0;
      rx_flag = false;
      if(timelastReceiveMessage >= 20)
      {
 //        tx_flag = true;
      }
      timelastReceiveMessage = 0;
    }
    
    if(tx_flag)
    {
      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,1);
      ModBus.master.Addr = 0x65;
      ModBus.master.Command = 0x03; 
      switch(TxCommandCounter)
      {
        case 0:
        ModBus.master.Reg_Addr.data = 0x0001;
        ModBus.master.Reg_Count.data = 0x0001;
        break;
        
        case 1:
        ModBus.master.Reg_Addr.data = 0x0002;
        ModBus.master.Reg_Count.data = 0x0002;
        break;

        case 2:
        ModBus.master.Reg_Addr.data = 0x0004;
        ModBus.master.Reg_Count.data = 0x0001;
        break;

        case 3:
        ModBus.master.Reg_Addr.data = 0x0005;
        ModBus.master.Reg_Count.data = 0x0002;
        break;

        case 4:
        ModBus.master.Reg_Addr.data = 0x0007;
        ModBus.master.Reg_Count.data = 0x0002;
        break;

        default:
        TxCommandCounter = 0;
        break;
      }
      
        NOS_ModBus_SendMasterCommand(&ModBus);
        tx_flag = false;
      
      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,0);
      TxCommandCounter++;
    }

    if(Time > UpdateFrameTime)
    {
       NOS_WS2812B_Matrix_PrintDetectorValue(&matrixA,&detector,language,&red,&yellow,&green);  
       NOS_WS2812B_Matrix_PrintRealTime(&matrixB,realtime);
       NOS_WS2812B_Matrix_PrintTemperature(&matrixC,temperature);
      //NOS_WS2812B_Matrix_FillColor(&matrixC,&white);

       NOS_WS2812B_Matrix_Update(&matrixA,0);
       NOS_WS2812B_Matrix_Update(&matrixB,0);
       NOS_WS2812B_Matrix_Update(&matrixC,0);

       visHandle();
       Time = 0;
    }
        /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  timelastReceiveMessage++;
  if(timelastReceiveMessage >= 5000)
  {
    timelastReceiveMessage = 0;
    tx_flag = true;
  }
  Time++;
  NOS_RealTime_Handler(&realtime);
  /* USER CODE END SysTick_IRQn 1 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

 // RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 10;
  sTime.Minutes = 23;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 0;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 65535;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,0);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_4,0);
  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
