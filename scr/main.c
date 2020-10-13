#include "stm32f4xx.h"
#include <stdio.h>
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"

void exti0_init(void);

void timer_init(void);
void delay_10_us(uint32_t time);

void request_dht11(void);
void response_dht11(void);
uint8_t receive_dht11(void);
void readDHT11(void);
	
void adc_init(void);
uint16_t ADC_Read(void);

volatile long int count_1=0, count_0=0;

GPIO_InitTypeDef DHT11_TypeDef; //DHT11

uint8_t c, I_RH, D_RH, I_Temp, D_Temp, CheckSum;

uint16_t adcValue;
char check;

int main(void)
{	
	timer_init();
	adc_init();
	
	while(1)
	{
		readDHT11();
		adcValue = ADC_Read();
		check = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8);
		delay_10_us(200000);
	}
}

void delay_10_us(uint32_t time)
{
	for(int i=0; i<time; i++)
	{
		TIM_SetCounter(TIM6,0);
		while(TIM_GetCounter(TIM6)<10);
	}
}

void timer_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	TIM_TimeBaseInitTypeDef myTimer6;
	myTimer6.TIM_Prescaler = 84-1;
	myTimer6.TIM_Period = 0xffff;
	TIM_TimeBaseInit(TIM6,&myTimer6);
	TIM_Cmd(TIM6,ENABLE);
}

void exti0_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	GPIO_InitTypeDef myLED;
	GPIO_InitTypeDef myBtn;
	GPIO_InitTypeDef myBtn1;
	EXTI_InitTypeDef myExti;
	NVIC_InitTypeDef myNvic;
	NVIC_InitTypeDef myNvic1;
	
	myLED.GPIO_Mode = GPIO_Mode_OUT;
	myLED.GPIO_OType = GPIO_OType_PP;
	myLED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myLED.GPIO_Speed = GPIO_Low_Speed;
	myLED.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD,&myLED);
	
	myBtn.GPIO_Mode = GPIO_Mode_IN;
	myBtn.GPIO_OType = GPIO_OType_PP;
	myBtn.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myBtn.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA,&myBtn);
	
	myBtn1.GPIO_Mode = GPIO_Mode_IN;
	myBtn1.GPIO_OType = GPIO_OType_PP;
	myBtn1.GPIO_PuPd = GPIO_PuPd_DOWN;
	myBtn1.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA,&myBtn1);
	
	myExti.EXTI_Line = EXTI_Line0 | EXTI_Line1;
	myExti.EXTI_Mode = EXTI_Mode_Interrupt;
	myExti.EXTI_Trigger = EXTI_Trigger_Falling;
	myExti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&myExti);
	
	//attach gpio pin to exti
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	myNvic.NVIC_IRQChannel = EXTI0_IRQn;
	myNvic.NVIC_IRQChannelPreemptionPriority = 0;
	myNvic.NVIC_IRQChannelSubPriority = 0;
	myNvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&myNvic);
	
	myNvic1.NVIC_IRQChannel = EXTI1_IRQn;
	myNvic1.NVIC_IRQChannelPreemptionPriority = 1;
	myNvic1.NVIC_IRQChannelSubPriority = 0;
	myNvic1.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&myNvic1);
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		delay_10_us(26);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))
		{
			
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	{
		delay_10_us(26);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))
		{
			
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

void request_dht11(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	DHT11_TypeDef.GPIO_Mode = GPIO_Mode_OUT;
	DHT11_TypeDef.GPIO_OType = GPIO_OType_PP;
	DHT11_TypeDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	DHT11_TypeDef.GPIO_Speed = GPIO_High_Speed;
	DHT11_TypeDef.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA,&DHT11_TypeDef);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
	
	delay_10_us(2000);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_6);
}

void response_dht11(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	DHT11_TypeDef.GPIO_Mode = GPIO_Mode_IN;
	DHT11_TypeDef.GPIO_OType = GPIO_OType_PP;
	DHT11_TypeDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	DHT11_TypeDef.GPIO_Speed = GPIO_High_Speed;
	DHT11_TypeDef.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA,&DHT11_TypeDef);
	
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6));
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0);
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6));
}
uint8_t receive_dht11(void)
{
	for (int q=0; q<8; q++)
	{
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0);
		delay_10_us(5);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))/* if high pulse is greater than 30us */
		{
			c = (c<<1)|(0x01);
		}			/* then its logic HIGH */
		else
		{			/* otherwise its logic LOW */
			c = (c<<1);
		}
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6));
	}
	return c;
}
void readDHT11(void)
{
		request_dht11();
		response_dht11();
		I_RH=receive_dht11();	/* store first eight bit in I_RH */
		D_RH=receive_dht11();	/* store next eight bit in D_RH */
		I_Temp=receive_dht11();	/* store next eight bit in I_Temp */
		D_Temp=receive_dht11();	/* store next eight bit in D_Temp */
		CheckSum=receive_dht11();
		
		if ((I_RH + D_RH + I_Temp + D_Temp) == CheckSum)
		{
			
		}
}
void adc_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef myLED;
	myLED.GPIO_Mode = GPIO_Mode_AN;
	myLED.GPIO_OType = GPIO_OType_PP;
	myLED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myLED.GPIO_Speed = GPIO_Low_Speed;
	myLED.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA,&myLED);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	ADC_InitTypeDef myADC;
	myADC.ADC_ContinuousConvMode = DISABLE;
	myADC.ADC_DataAlign = ADC_DataAlign_Right;
	myADC.ADC_Resolution = ADC_Resolution_12b;
	myADC.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1,&myADC);	
	
	ADC_RegularChannelConfig (ADC1,ADC_Channel_7,1,ADC_SampleTime_144Cycles);
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	ADC_Cmd(ADC1,ENABLE);

}
uint16_t ADC_Read(void)
{
    ADC_SoftwareStartConv(ADC1);

    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

    return ADC_GetConversionValue(ADC1);
}
void water_sensor_digital_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	DHT11_TypeDef.GPIO_Mode = GPIO_Mode_IN;
	DHT11_TypeDef.GPIO_OType = GPIO_OType_PP;
	DHT11_TypeDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	DHT11_TypeDef.GPIO_Speed = GPIO_High_Speed;
	DHT11_TypeDef.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA,&DHT11_TypeDef);
}