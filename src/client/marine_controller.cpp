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

#include "marine_controller.h"


#include "stm32f0xx_hal.h"
// #include "main.h"

#include <string.h>

extern UART_HandleTypeDef huart2;

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

uint8_t rx_buffer[1];

int marineMain(void) {
  while(1) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

    // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    //HAL_UART_Receive_DMA(&huart2, rx_buffer, 4);
    HAL_UART_Receive_IT(&huart2, rx_buffer, 1);
    //HAL_UART_Receive_DMA(huart, pData, Size)
    HAL_Delay(2000);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
    // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    HAL_Delay(2000);
  }

}



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
    // const unsigned char scen e_status[sizeof(scenes)/sizeof(scenes[0])];

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
extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
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
