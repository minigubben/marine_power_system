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
/*
 * Protocol
 * //0x0A - length - checksum - command - {address - value} - 0x0D
 * 0xAA - length - checksum - command - {address - value}
 * {} - can be multiple until buffer size is reached
 * length includes everything after checksum byte
 * checksum is crc8 including everything after crc8 byte
 *
 * commands
 *	** system ** 0-9
 *	0 ack
 *	1 nak
 *
 *	** outputs ** 10-19
 *	10 toggle
 *	11 on - address - output
 *	12 off - address - output
 *	13 dim - address - output - dim value 0-255
 *
 *	** scenes ** 20-29
 *	20 scene toggle - address
 *	21 scene on - address
 *	22 scene off - address
 *	23 scene dim - address - dim value 0-255
 *
 *
 */


/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
const unsigned char scenes[][2][6] =
{
    // scene one
    {
        // adress
        {123,121,128},
        // outputs
        {1,2,3},
    },
    // scene two
    {
        // adress
        {12,16,10},
        // outputs
        {4,5,6},
    },
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t rx_buffer[1];

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  //Ringbuf_init(huart2);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
  //HAL_UART_Receive_DMA(&huart2, rx_buffer, 4);
  HAL_UART_Receive_IT(&huart2, rx_buffer, 1);
  //HAL_UART_Receive_DMA(huart, pData, Size)
  HAL_Delay(2000);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
  HAL_Delay(2000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
	  HAL_Delay(500);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_9B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_EVEN;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
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
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Output_1_GPIO_Port, Output_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RS485_TX_EN_Pin|Output_3_Pin|Output_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Output_1_Pin */
  GPIO_InitStruct.Pin = Output_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Output_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RS485_TX_EN_Pin Output_3_Pin Output_2_Pin */
  GPIO_InitStruct.Pin = RS485_TX_EN_Pin|Output_3_Pin|Output_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
uint8_t CRC8(uint8_t *data, uint8_t length)
{
   uint8_t crc = 0x00;
   uint8_t extract;
   uint8_t sum;
   for(uint8_t i=0;i<length;i++)
   {
      extract = *data;
      for (uint8_t tempI = 8; tempI; tempI--)
      {
         sum = (crc ^ extract) & 0x01;
         crc >>= 1;
         if (sum)
            crc ^= 0x8C;
         extract >>= 1;
      }
      data++;
   }
   return crc;
}

void handleCommands(uint8_t *data, uint8_t length)
{
	uint8_t compare[] = "test";
	if (strncmp((char*)data, (char*)compare, (int)length)== 0)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	}
}

void serialProtocolReciver(uint8_t *message)
{
	uint8_t length = message[0];
	uint8_t checksum = message[1];
	uint8_t data[10];
	for (uint8_t i = 0; i < length; i++)
	{
		data[i] = message[i+2];
	}
	if (CRC8(data, length) == checksum)
	{
		handleCommands(data, length);
	}
}

void handleData(uint8_t data[], int length)
{
    //const unsigned char scene_status[sizeof(scenes)/sizeof(scenes[0])];

    // To implement: check to see that data matches protocol.



    // find find scene and execute command.
    // loop through scences
    for (int scene_count = 0; scene_count < sizeof(scenes)/sizeof(scenes[0]); scene_count++)
    {
    	// loop through scene addresses [0] and outputs [1]
        for (int adress = 0; adress < sizeof(scenes[0][0])/sizeof(scenes[0][0][0]); adress++)
        {
        	if (scenes[scene_count][0][adress] != 0 )
        	{
        	    // broadcast scene_status[scene_count] to adress
                //printf("handle adress: %d\n\r", scenes[scene_count][0][adress]);
        	}
        	if (scenes[scene_count][1][adress] != 0 )
        	{
        	    // set output to scene_status[scene_count]
                //printf("output: %d\n\r", scenes[scene_count][1][adress]);
        	}
        }
    }
}

// Uart interrupt handler
int recived_counter = -1;
uint8_t recived_string[10];
uint8_t recive_length = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  // check if interrupt is from uart2
  if (huart->Instance == USART2)
  {
	  // Reset buffer if it's full.
	  if (recived_counter >= sizeof(recived_string))
	  {
		  recived_counter = -1;
	  }

	  // first byte after start byte is message length
	  if (recived_counter == 0)
	  {
		  recive_length = rx_buffer[0];
		  recived_string[recived_counter] = rx_buffer[0];
		  recived_counter++;
	  }
	  // copy revied charactere to buffer as long as counter is below expected length
	  else if (recived_counter < recive_length+1)
	  {
		  recived_string[recived_counter] = rx_buffer[0];
		  recived_counter++;
	  }
	  // when counter reach expected message length, copy last charactere, handle message and reset counter
	  else if (recived_counter == recive_length+1)
	  {
		  recived_string[recived_counter] = rx_buffer[0];
		  serialProtocolReciver(recived_string);
		  recived_counter = -1;

	  }
	  // if wait for start charactere
	  else if (recived_counter < -1)
	  {
		  if (rx_buffer[0] == 0xAA)
		  {
			  recived_counter = 0;
		  }
	  }

	  HAL_UART_Receive_IT(&huart2, rx_buffer, 1);
  }
}

//	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//	{
//		//uint8_t comp[] = 'test';
//		//if(strcmp((char*) rx_buffer, (char*)'test') == 0) {
//		char one = rx_buffer[0];
//		char two = rx_buffer[1];
//		char three = rx_buffer[2];
//		char four = rx_buffer[3];
//		if (rx_buffer[0] == 't' && rx_buffer[1] == 'e' && rx_buffer[2] == 's' && rx_buffer[3] == 't') {
//		//if (sizeof(rx_buffer) == 4) {
//			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//		}
//		HAL_UART_Receive_DMA(&huart2, rx_buffer, 4);
//	}


  void resetUart()
  {
	  HAL_UART_Receive_DMA(&huart2, rx_buffer, 5);
  }
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
