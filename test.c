#include  "test.h"

uint32_t val1=0;
uint32_t val2=0;
uint32_t Difference=0;
uint8_t IsCaptured=0; //To check if the first value is captured
uint8_t Distance=0;
extern uint8_t dist;
extern TIM_HandleTypeDef htim1;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  
	{
		
		Capture();
	}
}

void delay(uint16_t time){
__HAL_TIM_SetCounter(&htim1,0);
	while(__HAL_TIM_GetCounter (&htim1)<time);

}

void HCSR04_Read (void){

HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay(10); 
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);  // pull the TRIG pin low

	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
}

void Capture(void)
	{
		if (IsCaptured==0)
		{
			val1 = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1); // read the first value using RISING edge
			IsCaptured = 1; 
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if (IsCaptured==1) 
		{
			val2 = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1);
			__HAL_TIM_SET_COUNTER(&htim1, 0);

			if (val2 > val1)
			{
				Difference = val2-val1;
			}
			else if (val1 > val2)
			{
				Difference = (0xffff - val1) + val2;
			}

			Distance = Difference * .034/2; //to convert it to cm
			IsCaptured = 0;
			dist = Distance;
			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);
			}
}
