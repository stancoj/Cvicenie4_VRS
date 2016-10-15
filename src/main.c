/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO/STM32
**                STMicroelectronics STM32Lxx Standard Peripherals Library
**
**  Distribution: The file is distributed "as is", without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. This file may only be built (assembled or compiled and linked)
**  using the Atollic TrueSTUDIO(R) product. The use of this file together
**  with other tools than Atollic TrueSTUDIO(R) is not permitted.
**
*****************************************************************************
*/

/* Includes */
#include <stddef.h>
#include "stm32l1xx.h"


void LED_init()
{
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	  GPIO_InitTypeDef GPIO_Init_LED;

	  GPIO_Init_LED.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_Init_LED.GPIO_OType = GPIO_OType_PP;
	  GPIO_Init_LED.GPIO_Pin = GPIO_Pin_5;
	  GPIO_Init_LED.GPIO_Speed = GPIO_Speed_40MHz;

	  GPIO_Init(GPIOA,&GPIO_Init_LED);
}

void adc_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Configure ADCx Channel 2 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);

	/* ADC1 configuration */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	/* ADCx regular channel8 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_16Cycles);
	/* Enable the ADC */
	ADC_Cmd(ADC1, ENABLE);
	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET){}
	/* Start ADC Software Conversion */
	ADC_SoftwareStartConv(ADC1);
}

int main(void)
{
	float i;
	adc_init();
	LED_init();
	uint16_t AD_value;


  while (1)
  {
	  	ADC_SoftwareStartConv(ADC1);
	   //wait for conversion complete

	  	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){}
	  	//read ADC value

	  	AD_value = ADC_GetConversionValue(ADC1);

	  	if ((AD_value > 1900) && (AD_value < 2100))
		{
	  		GPIO_SetBits(GPIOA,GPIO_Pin_5);
	  		for (i=0;i<(65326*0.5);i++){};
	  		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	  		for (i=0;i<(65326*0.5);i++){};
		}
	  	if ((AD_value > 2800) && (AD_value < 3000))
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_5);
			for (i=0;i<65326;i++){};
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);
			for (i=0;i<65326;i++){};
		}
	  	if ((AD_value > 3300) && (AD_value < 3550))
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_5);
			for (i=0;(i<65326*1.5);i++){};
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);
			for (i=0;i<(65326*1.5);i++){};
		}
	  	if ((AD_value > 3550) && (AD_value < 3750))
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_5);
			for (i=0;i<(65326*2);i++){};
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);
			for (i=0;i<(65326*2);i++){};
		}
  }
  return 0;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
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

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}
