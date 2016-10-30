/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11-February-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "vrs_cv5.h"


/** @addtogroup IO_Toggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
uint8_t status = 0;
char receivedChar;
char str[10];

int pom=0;
/* Private function prototypes -----------------------------------------------*/
RCC_ClocksTypeDef RCC_Clocks;
/* Private functions ---------------------------------------------------------*/

uint32_t value;
int switchMode = 0;
void USART_puts_Prerus();
int main(void) {

	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

	adc_init();
	NVICInit();
	UART_init();

	//enable USART
	USART_Cmd(USART1, ENABLE);
	//prerus
	 USART_ClearITPendingBit(USART1, USART_IT_TC);
	//USART_puts("init\r");

    //Initialize string to zero
    memset(str, 0 ,10);

	float voltValue = 0.00;
	while (1) {

		if (receivedChar == 'm') {
			if(switchMode == 0){
				sprintf(str,"%u\r",value);
				switchMode = 1;
			}else{
				voltValue = (value * 3.3) / 4096;
								sprintf (str, "%.2fV\r", voltValue);
				switchMode = 0;
			}

			receivedChar=0;
			USART_puts_Prerus(str);
			//USART_puts(str);
		}

	}}




void ADC1_IRQHandler(void){
	if(ADC1->SR & ADC_SR_EOC){
		value = ADC1->DR;
	}
}

void USART1_IRQHandler(void)
{	//Kontroluje,  USART preru�enie u� do�lo alebo nie.
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		receivedChar = USART_ReceiveData(USART1);
    }

	//Kontroluje,  USART preru�enie u� do�lo alebo nie.
	else if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	 	{
		//Vyma�e USART preru�enie prebiehaj�ceho bitov.
	 		USART_ClearITPendingBit(USART1, USART_IT_TC);
	 		if (pom > 0){
	 			if(str[pom] == '\0'){
	 				USART_SendData(USART1,'\r');
	 				pom = 0;
	 			}
	 			else{
	 				USART_SendData(USART1,str[pom]);
	 				pom++;
	 			}
	 		}
	 	}
}
void USART_puts_Prerus(char str[]){
 	if (pom == 0){
 		USART_SendData(USART1,str[0]);
 		pom = 1;
 	}
 }
void USART_puts(volatile char *s){

	while(*s){
		// wait until data register is empty
		while( !(USART1->SR & 0x00000040) );dlear
		USART_SendData(USART1, *s);
		*s++;
	}
}


/**
* @brief  Inserts a delay time.
* @param  nTime: specifies the delay time length, in 1 ms.
* @retval None
*/
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;
  
  while(TimingDelay != 0);
}

/**
* @brief  Decrements the TimingDelay variable.
* @param  None
* @retval None
*/
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
* @}
*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
