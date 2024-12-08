/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "CGlobale.h"
#include "retarget.h"

CGlobale Application;

// --------------------------------------------------
uint32_t ADC_ReadAnalog(uint32_t channel)
{

	ADC_ChannelConfTypeDef sConfig = {0};

	  sConfig.Channel = channel;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;
	  sConfig.SingleDiff = ADC_SINGLE_ENDED;
	  //sConfig.OffsetNumber = ADC_OFFSET_NONE;
	  //sConfig.Offset = 0;
	  //sConfig.OffsetSignedSaturation = DISABLE;
	  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2, 1000);
	uint32_t analog_value = HAL_ADC_GetValue(&hadc2);
	HAL_ADC_Stop(&hadc2);
	return analog_value;
}

// --------------------------------------------------
uint8_t uart_irq_rxbuff[1];
void irq_uart2()
{
    HAL_UART_Transmit(&huart2, uart_irq_rxbuff, 1, 100);
    HAL_UART_Receive_IT(&huart2, uart_irq_rxbuff, 1);
}

// =====================================================
int main_app(void)
{
  RetargetInit(&huart2);
  HAL_UART_Receive_IT(&huart2, uart_irq_rxbuff, 1);

  Application.Run();
  while (1)
  {
  }
  return 0;
}

