/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include "CGlobale.h"
#include "retarget.h"

CGlobale Application;

// --------------------------------------------------
uint8_t uart_irq_rxbuff[1];
void irq_uart2()
{
    //HAL_UART_Transmit(&huart2, uart_irq_rxbuff, 1, 100);
    Application.m_menu_interactive.receive_car(uart_irq_rxbuff[0]);
    HAL_UART_Receive_IT(&huart2, uart_irq_rxbuff, 1);
}

// =====================================================
int main_app(void)
{
    // Active les codeurs
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_2);

    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_2);

    // Active les sorties PWM moteur
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);

    // Active les sorties PWM servomoteurs
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

    // Redirection du printf vers l'UART2
    RetargetInit(&huart2);

    Application.m_menu_interactive.enable_echo(true);

    HAL_UART_Receive_IT(&huart2, uart_irq_rxbuff, 1);

    Application.Run();
    while (1)
    {
    }
    return 0;
}

