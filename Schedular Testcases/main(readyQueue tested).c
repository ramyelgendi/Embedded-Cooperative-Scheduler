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
#include "stdio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;
struct Queue readyQueue;
struct Queue delayedQueue;
int flag = 0;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
typedef void(*func)();
struct Task{int priority,delay;func task;};
struct Queue{int currentSize,maxSize;struct Task tasksList[10]; };
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

void Init()   // initialize the scheduler data structures
{
   readyQueue.currentSize=0;
   delayedQueue.currentSize=0;
   for (int i = 0; i<10;i++)
	{
		delayedQueue.tasksList[i].delay=1000;
	}
}

int ReadyQueueIsEmpty()
{
if(readyQueue.currentSize==0)
	return 1;
else
	return 0;
}

int DelayQueueIsEmpty()
{
	if(delayedQueue.currentSize==0)
		return 1;
	else
		return 0;
}
void bubbleSortPriority()
{
    for (int i = 0; i<readyQueue.currentSize;i++)
    {
        for(int j =0; j<readyQueue.currentSize-1;j++)
        {
            if (readyQueue.tasksList[i].priority>readyQueue.tasksList[j].priority)
            {
                struct Task temp =readyQueue.tasksList[i];
                readyQueue.tasksList[i] = readyQueue.tasksList[j];
                readyQueue.tasksList[j] = temp;
            }
        }
    }
}
void bubbleSortDelay()
{
    for (int i = 0; i< delayedQueue.currentSize;i++)
    {
        for(int j =i; j<delayedQueue.currentSize;j++)
        {
            if (delayedQueue.tasksList[i].delay>delayedQueue.tasksList[j].delay)
            {
                struct Task temp =delayedQueue.tasksList[i];
                delayedQueue.tasksList[i] = delayedQueue.tasksList[j];
                delayedQueue.tasksList[j] = temp;
            }
        }
    }
}
void Enqueue(int priority,int delay,func task)
{
	if(delay==0)
	{
		readyQueue.tasksList[readyQueue.currentSize].task=task;
		readyQueue.tasksList[readyQueue.currentSize].priority=priority;
		readyQueue.currentSize++;
		bubbleSortPriority();
	}
	else{
		delayedQueue.tasksList[delayedQueue.currentSize].task = task;
		delayedQueue.tasksList[delayedQueue.currentSize].delay = delay;
		delayedQueue.tasksList[delayedQueue.currentSize].priority=readyQueue.tasksList[0].priority;
		delayedQueue.currentSize++;
		bubbleSortDelay();
			}
}
void QueTask(func task,int priority)
{
  Enqueue(priority,0,task);
}

void ReRunMe(int delay)
{
  if (delay==0)
     QueTask(readyQueue.tasksList[0].task,readyQueue.tasksList[0].priority);
  else
     Enqueue(readyQueue.tasksList[0].priority,delay,readyQueue.tasksList[0].task);
}

void Dequeue()
{
for(int i=1;i<readyQueue.currentSize;i++)
  {
    readyQueue.tasksList[i-1]=readyQueue.tasksList[i];
  }
    readyQueue.tasksList[readyQueue.currentSize-1].task = NULL;
    readyQueue.tasksList[readyQueue.currentSize-1].priority = 0;
    readyQueue.currentSize--;
}

void DequeueDelay()
{
 for(int i=1;i<delayedQueue.currentSize;i++)
   {
     delayedQueue.tasksList[i-1]=delayedQueue.tasksList[i];
   }
     delayedQueue.tasksList[delayedQueue.currentSize-1].task = NULL;
     delayedQueue.tasksList[delayedQueue.currentSize-1].delay = 1000;
     delayedQueue.tasksList[delayedQueue.currentSize-1].priority = -1;
     delayedQueue.currentSize--;
}
void Dispatch()
{
    while(delayedQueue.tasksList[0].delay==0)
    {
        QueTask(delayedQueue.tasksList[0].task,delayedQueue.tasksList[0].priority);
        DequeueDelay();
    }
    if(!ReadyQueueIsEmpty())
		{
			readyQueue.tasksList[0].task();
			Dequeue();
		}
    if ((DelayQueueIsEmpty())&&(ReadyQueueIsEmpty()))
			flag=1;
		if(ReadyQueueIsEmpty())
			{
				HAL_UART_Transmit(&huart2,(uint8_t *)"Idle\r\n", sizeof("Idle \r\n"),500);
			}
}

void TaskA(void)
{
   //do something
   HAL_UART_Transmit(&huart2,(uint8_t *)"Task 1\r\n", sizeof("Task 1\r\n"),500);
// Rerun again after 12 ticks
  // ReRunMe(0);
}
void TaskB(void)
{
   //do something
   HAL_UART_Transmit(&huart2,(uint8_t *)"Task 2\r\n", sizeof("Task 2\r\n"),500);
// Rerun again after 12 ticks
  // ReRunMe(0);
}
void TaskC(void)
{
   //do something
   HAL_UART_Transmit(&huart2,(uint8_t *)"Task 3\r\n", sizeof("Task 3\r\n"),500);
// Rerun again after 12 ticks
  // ReRunMe(0);
}
void TaskD()
{
  //do something
  HAL_UART_Transmit(&huart2,(uint8_t *)"Task 4\r\n", sizeof("Task 4\r\n"),500);
// Rerun again after 10 ticks
 // ReRunMe(0);
}
void TaskE()
{
  //do something
  HAL_UART_Transmit(&huart2,(uint8_t *)"Task 5\r\n", sizeof("Task 5\r\n"),500);
// Rerun again after 10 ticks
 // ReRunMe(0);
}
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
	MX_I2C1_Init();
	MX_USART2_UART_Init();	

	Init();  // initialize the scheduler data structures
	QueTask(TaskA,5);
	QueTask(TaskB,4);
	QueTask(TaskC,3);
	QueTask(TaskD,2);
	QueTask(TaskE,1);
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
while (1)
  {
	   Dispatch();
  }
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
 HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()*0.05); //0.05 represets 1 tick (50ms)
/**Configure the Systick
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
HAL_RCCEx_EnableMSIPLLMode();
}
/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{
  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000E14;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */
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
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
