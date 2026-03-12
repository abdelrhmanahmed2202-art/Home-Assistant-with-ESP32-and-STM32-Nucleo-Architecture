/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : FAST FarmBot (Reduced Delays & High Speed)
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Variables */
UART_HandleTypeDef huart1;

/* --- ROBOT STATE --- */
long current_x = 0;
long current_y = 0;
long current_z = 0;

long target_x = 0;
long target_y = 0;
long target_z = 0;

/* --- UART Parsing --- */
uint8_t rx_byte;
uint8_t rx_buffer[50];
int rx_index = 0;
volatile uint8_t data_ready = 0;

/* Prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void Micro_Delay(int delay);

/* --- 1. UART CALLBACK (The Ear) --- */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    if (rx_byte == '\n') {
        rx_buffer[rx_index] = 0; // End of string
        data_ready = 1;          // Flag main loop
        rx_index = 0;
    } else {
        if (rx_index < 50) rx_buffer[rx_index++] = rx_byte;
    }
    HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
  }
}

/* --- 2. MAIN LOOP (The Brain & Muscle) --- */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();

  // Start UART Listening
  HAL_UART_Receive_IT(&huart1, &rx_byte, 1);

  while (1)
  {
    if (data_ready)
    {
        // A. Parse Command "X...Y...Z..."
        char *pX = strchr((char*)rx_buffer, 'X');
        char *pY = strchr((char*)rx_buffer, 'Y');
        char *pZ = strchr((char*)rx_buffer, 'Z');

        if (pX) target_x = atoi(pX + 1);
        if (pY) target_y = atoi(pY + 1);
        if (pZ) target_z = atoi(pZ + 1);

        // =========================================================
        // MOVE X AXIS (Dual Motors) with ACCELERATION
        // =========================================================
        if (current_x != target_x)
        {
            // Reset Speed for this move
            int step_delay = 2000; // Start Slow
            int steps_taken = 0;

            while(current_x != target_x)
            {
                // Set Direction
                if (target_x > current_x) {
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);   // X1 Dir
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);   // X2 Dir
                    current_x++;
                } else {
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
                    current_x--;
                }

                // Pulse Both Motors
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
                Micro_Delay(30); // Short pulse
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

                // Speed Control
                Micro_Delay(step_delay);

                // Accelerate Logic: Every 5 steps, go faster until limit (400)
                steps_taken++;
                if(steps_taken % 5 == 0 && step_delay > 800) {
                    step_delay -= 10;
                }
            }
            // REDUCED DWELL TIME: 0.1s (Was 1s)
            HAL_Delay(100);
        }

        // =========================================================
        // MOVE Y AXIS with ACCELERATION
        // =========================================================
        if (current_y != target_y)
        {
            int step_delay = 2000;
            int steps_taken = 0;

            while(current_y != target_y)
            {
                if (target_y > current_y) {
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET); // Y Dir Fwd
                    current_y++;
                } else {
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
                    current_y--;
                }

                // Pulse Y
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
                Micro_Delay(30);
                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);

                Micro_Delay(step_delay);

                steps_taken++;
                if(steps_taken % 5 == 0 && step_delay > 800) step_delay -= 10;
            }
            // REDUCED DWELL TIME: 0.1s
            HAL_Delay(100);
        }

        // =========================================================
        // MOVE Z AXIS with ACCELERATION
        // =========================================================
        if (current_z != target_z)
        {
            int step_delay = 2000;
            int steps_taken = 0;

            while(current_z != target_z)
            {
                if (target_z > current_z) {
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // Z Dir Fwd
                    current_z++;
                } else {
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
                    current_z--;
                }

                // Pulse Z
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
                Micro_Delay(30);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

                Micro_Delay(step_delay);

                steps_taken++;
                if(steps_taken % 5 == 0 && step_delay > 800) step_delay -= 10;
            }
            // No dwell needed after Z
        }

        data_ready = 0; // Ready for next command
    }
  }
}

/* --- HELPER: Microsecond Delay --- */
void Micro_Delay(int delay) {
    for(int i=0; i<delay; i++) {
        __NOP();
    }
}

/* --- STANDARD INIT FUNCTIONS --- */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}

static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_10, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  // X2 & Y
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // X1
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Z
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Error_Handler(void) { __disable_irq(); while (1) {} }
