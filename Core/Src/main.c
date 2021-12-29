/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "gpio.h"
#include "tim.h"
#include "display.h"
#include "assignment.h"
#include "i2c.h"
#include"stdio.h"

#include "../barometer/lps25hb.h"
#include "../temperature_humidity/hts221.h"

void SystemClock_Config(void);

extern uint64_t disp_time;
uint64_t saved_time;
//char text_to_display[] = "Lukas_Pocsik_98388";

char text_to_display [20];
char data[4];
int shift = 4;
int x = 0;
int i;
int j;
int refresh;
int switch_state = 0;

float pressure;
float temperature;
float humidity;
float altitude;

int main(void)
{

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  SystemClock_Config();

  MX_GPIO_Init();

  setSegments();
  setDigits();

  LL_mDelay(2000);

  resetDigits();
  resetSegments();

  MX_TIM3_Init();
  MX_I2C1_Init();

  //type your code for EXTI configuration (priority, enable EXTI, setup EXTI for input pin, trigger edge) here:


  SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI3);
  SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PB;

  EXTI->IMR |= EXTI_IMR_MR3;

  //Set EXTI trigger to falling edge
  EXTI->RTSR &= ~(EXTI_IMR_MR3);
  EXTI->FTSR |= EXTI_IMR_MR3;

  NVIC_SetPriority(EXTI3_IRQn, 2); // Priority
  NVIC_EnableIRQ(EXTI3_IRQn); // Enable Interrupt

  if(!lps25hb_init() || !hts221_init())
	 refresh = 1;

  while (1)
  {
		  lps25hb_get_pressure(&pressure);
		  lps25hb_get_altitude(&altitude);
		  hts221_get_temperature(&temperature);
		  hts221_get_humidity(&humidity);

		  /// TEMPERATURE

		  if (switch_state == 0)
		  {
			  if(temperature >= 100)
		  		 temperature = 99.9;

		  	  if(temperature <= -100)
		  		 temperature = -99.9;

		  	  sprintf(text_to_display, "TEMP_%2.1f", temperature);

		  	  for(i = 0; i<strlen(text_to_display); i++)
		  	  {
		  		  if(text_to_display[i] == '.')
		  		  {
		  			  text_to_display[i] = text_to_display[i+1];
		  			  text_to_display[i+1] = '\0';
		  		  }
		  	  }

			  if(shift<9)
			  {

				  for(i=0; i<5; i++)
				  {
					  for(j=i; j<shift; j++)
					  {
						  data[x] = text_to_display[j];
						  displaySign(data, i, switch_state);
						  x++;
					  }

					  LL_mDelay(500);
					  memset(data, 0, sizeof data);
					  x = 0;
					  shift++;
				  }
			  }else
			  {
				  for(i=3; i>=0; i--)
				  {
					  for(j=i; j<shift-2; j++)
					  {
						  data[x] = text_to_display[j];
						  displaySign(data, i, switch_state);
						  x++;
					  }

					  LL_mDelay(500);
					  memset(data, 0, sizeof data);
					  x = 0;
					  shift--;
				  }
			  }
		  }

			  /// HUMIDITY

			  if (switch_state == 1)
			  {
				  if(humidity >= 100)
					 humidity = 99;

			  	  if(humidity <= 0)
			  		humidity = 0;

			  	  sprintf(text_to_display, "HUM_%2.0f", humidity);



				  if(shift<7)
				  {

					  for(i=0; i<3; i++)
					  {
						  for(j=i; j<shift; j++)
						  {
							  data[x] = text_to_display[j];
							  displaySign(data, i, switch_state);
							  x++;
						  }

						  LL_mDelay(500);
						  memset(data, 0, sizeof data);
						  x = 0;
						  shift++;
					  }
				  }else
				  {
					  for(i=1; i>=0; i--)
					  {
						  for(j=i; j<shift-2; j++)
						  {
							  data[x] = text_to_display[j];
							  displaySign(data, i, switch_state);
							  x++;
						  }

						  LL_mDelay(500);
						  memset(data, 0, sizeof data);
						  x = 0;
						  shift--;
					  }
				  }
			   }

			  /// PRESSURE

			  if (switch_state == 2)
			  {

				  if(pressure >= 10000)
					 pressure = 9999.99;

			  	  if(pressure <= 0)
			  		 pressure = 0;

			  	  sprintf(text_to_display, "BAR_%4.2f", pressure);

			  	  for(i = 0; i<strlen(text_to_display); i++)
			  	  {
			  		  if(text_to_display[i] == '.')
			  		  {
			  			text_to_display[i] = text_to_display[i+1];
			  			text_to_display[i+1] = text_to_display[i+2];
			  			text_to_display[i+2] = '\0';
			  		  }
			  	  }

				  if(shift<10)
				  {

					  for(i=0; i<6; i++)
					  {
						  for(j=i; j<shift; j++)
						  {
							  data[x] = text_to_display[j];
							  displaySign(data, i, switch_state);
							  x++;
						  }

						  LL_mDelay(500);
						  memset(data, 0, sizeof data);
						  x = 0;
						  shift++;
					  }
				  }else
				  {
					  for(i=4; i>=0; i--)
					  {
						  for(j=i; j<shift-2; j++)
						  {
							  data[x] = text_to_display[j];
							  displaySign(data, i, switch_state);
							  x++;
						  }

						  LL_mDelay(500);
						  memset(data, 0, sizeof data);
						  x = 0;
						  shift--;
					  }
				  }
			  }

			  /// ALTITUDE

			  if (switch_state == 3)
			  {
				  if(altitude >= 10000)
					 altitude  = 9999.9;

			  	  if(altitude  <= -10000)
			  		 altitude  = -9999.9;

			  	  sprintf(text_to_display, "ALT_%4.1f", altitude);

			  	  for(i = 0; i<strlen(text_to_display); i++)
			  	  {
			  		  if(text_to_display[i] == '.')
			  		  {
			  			  text_to_display[i] = text_to_display[i+1];
			  			  text_to_display[i+1] = '\0';
			  		  }
			  	  }

				  if(shift<9)
				  {

					  for(i=0; i<5; i++)
					  {
						  for(j=i; j<shift; j++)
						  {
							  data[x] = text_to_display[j];
							  displaySign(data, i, switch_state);
							  x++;
						  }

						  LL_mDelay(500);
						  memset(data, 0, sizeof data);
						  x = 0;
						  shift++;
					  }
				  }else
				  {
					  for(i=3; i>=0; i--)
					  {
						  for(j=i; j<shift-2; j++)
						  {
							  data[x] = text_to_display[j];
							  displaySign(data, i, switch_state);
							  x++;
						  }

						  LL_mDelay(500);
						  memset(data, 0, sizeof data);
						  x = 0;
						  shift--;
					  }
				  }
			  }

			  if(switch_state == 4)
				  switch_state = 0;

  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SYSTICK_EnableIT();
  LL_SetSystemCoreClock(8000000);
}

/* USER CODE BEGIN 4 */
uint8_t checkButtonState(GPIO_TypeDef* PORT, uint8_t PIN, uint8_t edge, uint8_t samples_window, uint8_t samples_required)
{
	uint8_t button_state = 0, timeout = 0;

	while(button_state < samples_required && timeout < samples_window)
	{
		if(!(PORT->IDR & (1 << PIN)))
		{
			button_state += 1;
		}
		else
		{
			button_state = 0;
		}

		timeout += 1;
		LL_mDelay(1);
	}

	if((button_state >= samples_required) && (timeout <= samples_window))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void EXTI3_IRQHandler(void)
{
	if(checkButtonState(GPIO_PORT_BUTTON,
						GPIO_PIN_BUTTON,
						BUTTON_EXTI_TRIGGER,
						BUTTON_EXTI_SAMPLES_WINDOW,
						BUTTON_EXTI_SAMPLES_REQUIRED))
	{
		//switch_state ^= 1;
		switch_state++;
	}

	/* Clear EXTI4 pending register flag */

	EXTI->PR |= (EXTI_PR_PIF3);
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
void assert_failed(char *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
