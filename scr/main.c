#include "stm32f4xx.h"
#include <stdio.h>
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"

/*
-PA6: DHT11
-PA7: gas sensor
-PA8: water sensor
-PC6: servo: 20~140
-PA2-PA3: TX-RX
*/

void exti0_init(void);

void timer6_delay_init(void);
void delay_10_us(uint32_t time);

void led_init(void);

void request_dht11(void);
void response_dht11(void);
uint8_t receive_dht11(void);
void readDHT11(void);
	
void gas_sensor_init(void);
uint16_t read_gas_sensor(void);

void water_sensor_init(void);
uint16_t read_water_sensor(void);

void timer3_pwm_sg90(void);

void timer7_interrput(void);

void gpio_init(void);

void uart2_init(void);
void usart_send_char(char data);
void usart_send_string(char *s);

uint8_t c, I_RH=0, D_RH, I_Temp=0, D_Temp, CheckSum;

uint16_t gas_sensor_value;
uint16_t water_sensor_value;

volatile char data_receive;
char buffer[4];
uint16_t test=11;
float test1=123.45;


int main(void)
{	
	gpio_init();
	timer6_delay_init();
	/*
	gas_sensor_init();
  water_sensor_init();
	timer3_pwm_sg90();
	*/
	led_init();
	//timer7_interrput();
	uart2_init();
	while(1)
	{
		/*
		gas_sensor_value = read_gas_sensor();
		if (gas_sensor_value>2500)
		{
			TIM_SetCompare1(TIM3,140);
		}
		else
		{
			TIM_SetCompare1(TIM3,80);
		}
		water_sensor_value = read_water_sensor();
		
		*/
		
		//readDHT11();
		I_RH=32;
		sprintf(buffer,"a%dz",I_Temp);
		usart_send_string(buffer);
		sprintf(buffer,"b%dy",I_RH);
		usart_send_string(buffer);

		delay_10_us(200000); //2s
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

void timer6_delay_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	TIM_TimeBaseInitTypeDef myTimer6;
	myTimer6.TIM_Prescaler = 84-1;
	myTimer6.TIM_Period = 0xffff;
	TIM_TimeBaseInit(TIM6,&myTimer6);
	TIM_Cmd(TIM6,ENABLE);
}
void led_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	GPIO_InitTypeDef myLED;
	myLED.GPIO_Mode = GPIO_Mode_OUT;
	myLED.GPIO_OType = GPIO_OType_PP;
	myLED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myLED.GPIO_Speed = GPIO_Low_Speed;
	myLED.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD,&myLED);
}

void request_dht11(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef DHT11_TypeDef;
	
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
	
	GPIO_InitTypeDef DHT11_TypeDef;
	
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
void gas_sensor_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef myGPIO;
	myGPIO.GPIO_Mode = GPIO_Mode_AN;
	myGPIO.GPIO_OType = GPIO_OType_PP;
	myGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myGPIO.GPIO_Speed = GPIO_Low_Speed;
	myGPIO.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA,&myGPIO);
	
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
uint16_t read_gas_sensor(void)
{
    ADC_SoftwareStartConv(ADC1);

    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

    return ADC_GetConversionValue(ADC1);
}

void water_sensor_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef myGPIO;
	myGPIO.GPIO_Mode = GPIO_Mode_AN;
	myGPIO.GPIO_OType = GPIO_OType_PP;
	myGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myGPIO.GPIO_Speed = GPIO_Low_Speed;
	myGPIO.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA,&myGPIO);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	ADC_InitTypeDef myADC;
	myADC.ADC_ContinuousConvMode = DISABLE;
	myADC.ADC_DataAlign = ADC_DataAlign_Right;
	myADC.ADC_Resolution = ADC_Resolution_12b;
	myADC.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1,&myADC);	
	
	ADC_RegularChannelConfig (ADC1,ADC_Channel_8,2,ADC_SampleTime_144Cycles);
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	ADC_Cmd(ADC1,ENABLE);

}
uint16_t read_water_sensor(void)
{
    ADC_SoftwareStartConv(ADC1);

    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

    return ADC_GetConversionValue(ADC1);
}
void timer3_pwm_sg90(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	
	GPIO_InitTypeDef myGPIO;
	myGPIO.GPIO_Mode = GPIO_Mode_AF;
	myGPIO.GPIO_OType = GPIO_OType_PP;
	myGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myGPIO.GPIO_Speed = GPIO_Low_Speed;
	myGPIO.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC,&myGPIO);
	
	TIM_TimeBaseInitTypeDef myTimer;
	myTimer.TIM_Prescaler = 1680-1;
	myTimer.TIM_Period = 1000-1;
	myTimer.TIM_ClockDivision = TIM_CKD_DIV1;
	myTimer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&myTimer);
	TIM_Cmd(TIM3,ENABLE);
	
	TIM_OCInitTypeDef myPWM;
	myPWM.TIM_OCMode = TIM_OCMode_PWM1;
	myPWM.TIM_OCPolarity = TIM_OCPolarity_High;
	myPWM.TIM_Pulse = 0;
	myPWM.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC1Init(TIM3,&myPWM);
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
}

void timer7_interrput(void)
{
	//interrupt every 2s
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
	
	TIM_TimeBaseInitTypeDef myTim7;
	myTim7.TIM_Prescaler = 8400-1;
	myTim7.TIM_Period = 20000-1;
	myTim7.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM7,&myTim7);
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM7,ENABLE);

	NVIC_InitTypeDef myNvic;
	myNvic.NVIC_IRQChannel = TIM7_IRQn;
	myNvic.NVIC_IRQChannelPreemptionPriority = 1;
	myNvic.NVIC_IRQChannelSubPriority = 0;
	myNvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&myNvic);
}
void gpio_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef myGPIO;
	myGPIO.GPIO_Mode = GPIO_Mode_OUT;
	myGPIO.GPIO_OType = GPIO_OType_PP;
	myGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myGPIO.GPIO_Speed = GPIO_High_Speed;
	myGPIO.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_Init(GPIOB,&myGPIO);
}
void uart2_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_InitTypeDef myUart;
	myUart.USART_BaudRate = 9600;
	myUart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	myUart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	myUart.USART_Parity = USART_Parity_No;
	myUart.USART_StopBits = USART_StopBits_1;
	myUart.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&myUart);
	USART_Cmd(USART2,ENABLE);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF; //This is important. We will this pin except of INPUT, OUTPUT and ANALOG so we set as Alternate Function
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3;   //Communicate on PA2 and PA3
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//We set the pin as Push Pull
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; //And Pull Up. Keep it always on HIGH
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; //And pin frequency
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	NVIC_InitTypeDef myNvic;
	myNvic.NVIC_IRQChannel = USART2_IRQn;
	myNvic.NVIC_IRQChannelPreemptionPriority = 1;
	myNvic.NVIC_IRQChannelSubPriority = 1;
	myNvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&myNvic);
	
	USART_Cmd(USART2,ENABLE);
}
void usart_send_char(char data)
{
	while(!(USART2->SR & 0x00000040));
	USART_SendData(USART2,data);
}
void usart_send_string(char *s)
{
	while(*s)
	{
		usart_send_char(*s);
		s++;
	}
}

//interrupt program
void TIM7_IRQHandler(void) //2s
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

	}
}
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		data_receive = USART_ReceiveData(USART2);
		
		switch(data_receive)
		{
			case 'a':
				GPIO_SetBits(GPIOD, GPIO_Pin_12);
				break;
			
			case 'A':
				GPIO_ResetBits(GPIOD, GPIO_Pin_12);
				break;
			
			case 'b':
				GPIO_SetBits(GPIOD, GPIO_Pin_13);
				break;
			
			case 'B':
				GPIO_ResetBits(GPIOD, GPIO_Pin_13);
				break;
			
			default:
				break;
		}
		
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}