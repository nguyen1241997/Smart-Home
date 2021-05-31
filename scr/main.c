#include "stm32f4xx.h"
#include <stdio.h>
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "lcd.h"

#define led_LVR_pin			GPIO_Pin_0
#define led_KCR_pin			GPIO_Pin_1
#define led_BedR_pin		GPIO_Pin_2
#define led_BaR_pin			GPIO_Pin_3
#define led_Garage_pin	GPIO_Pin_4
#define fan_LVR_pin			GPIO_Pin_6
#define fan_KCR_pin			GPIO_Pin_5
#define fan_BedR_pin		GPIO_Pin_7

/*
-PA6: DHT11
-PA0: gas sensor
-PA1: water sensor
-PA2-PA3: TX-RX
-PA7: LCD contrast

-PB0-PB7: gpio
-PB8-9: led-gate
-PB10: buzz

-PC6-PC8: servo: 20~140
-PC8: window
-PC7: garage door / door
-PC6: clothes
-PC9: gate
-PC0-PC3: KEYPAD IN --5678

-PD0: button
-PD2-5: KEYPAD OUT --1234

-PE0-PE5: LCD
*/

void exti0_init(void);

void timer6_delay_init(void);
void delay_10_us(uint32_t time);

void led_init(void);

void request_dht11(void);
void response_dht11(void);
uint8_t receive_dht11(void);
void readDHT11(void);
	
void adc_dma_init(void);

void timer3_pwm_sg90(void);

void timer7_interrput(void);

void gpio_init(void);

void uart2_init(void);
void usart_send_char(char data);
void usart_send_string(char *s);

void uart_to_esp(void);

void lcd_gpio(void);
void keypad_init(void);
int8_t read_keypad (void);
void check_password(int8_t key_var);

void exti0_init(void);

void fire_sensor_init(void);

uint8_t c=0, I_RH=0, D_RH, I_Temp=0, D_Temp;

volatile uint16_t adc_value[2];
volatile uint16_t x=0;

volatile char data_receive;
char buffer[20];

char state_led_LVR=0, state_led_KCR=0, state_led_BedR=0, state_led_BaR=0, state_led_Garage=0;
char state_fan_LVR=0, state_fan_KCR=0, state_fan_BedR=0;
char state_door_garage=0, state_clothes=0, state_window=0, state_gate=0, state_gas=0, state_fire=0;

volatile char gate_check=0;

int8_t password[5]={"12345"};
int8_t check=0;
int8_t key_enter[5];
int8_t key;
volatile uint8_t out=0;


int main(void)
{
	keypad_init();
	timer6_delay_init();
	/*
	lcd_gpio();
	lcd_init();
	lcd_write(0,0,(int8_t *)" ");
	lcd_write(0,3,(int8_t *)"SMART HOME");
	
	//led_init();
	//timer7_interrput();
	
	gpio_init();
	GPIO_SetBits(GPIOB, fan_LVR_pin);//tich cuc thap
	GPIO_SetBits(GPIOB, fan_KCR_pin);
	//GPIO_SetBits(GPIOB, fan_BedR_pin);
	timer6_delay_init();
	
	adc_dma_init();
	uart2_init();
	timer3_pwm_sg90();
	//led_init();
	timer7_interrput();
	
	exti0_init();//lcd interrupt
	
	fire_sensor_init();
	
	TIM_SetCompare1(TIM3,30);//close clothes

	TIM_SetCompare3(TIM3,80);;//close window
	TIM_SetCompare1(TIM14,440);//lcd contrast
	TIM_SetCompare1(TIM12,75); //garage door
	TIM_SetCompare4(TIM3,55); //close gate
	TIM_SetCompare2(TIM3,90);//close door
	
	//GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
	*/
	while(1)
	{
/*
		//gas
		if ((adc_value[0]<2500) && (state_window==0)) //close window
		{
			TIM_SetCompare3(TIM3,80);
		}
		else
		{
			state_window=1;
			TIM_SetCompare3(TIM3,35);
		}
		
		//water
		if ((adc_value[1]>2000) && (state_clothes==1)) //open clothes
		{
			TIM_SetCompare1(TIM3,80);
		}
		else
		{
			state_clothes=0;
			TIM_SetCompare1(TIM3,30);
		}
		
		
		readDHT11();
		
		uart_to_esp();

		delay_10_us(800000);*/
		key=read_keypad();
		delay_10_us(50000);
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
		I_RH=receive_dht11();
		D_RH=receive_dht11();
		I_Temp=receive_dht11();
		D_Temp=receive_dht11();	
}
void adc_dma_init(void)
{
	ADC_InitTypeDef myADC;
	DMA_InitTypeDef myDMA;
	GPIO_InitTypeDef myLED;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	myDMA.DMA_Channel = DMA_Channel_0;
	myDMA.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;
	myDMA.DMA_Memory0BaseAddr = (uint32_t) &adc_value;
	myDMA.DMA_BufferSize = 2;
	myDMA.DMA_DIR = DMA_DIR_PeripheralToMemory;
	myDMA.DMA_Mode = DMA_Mode_Circular;
	myDMA.DMA_FIFOMode = DMA_FIFOMode_Disable;
	myDMA.DMA_Mode = DMA_Mode_Circular;
	myDMA.DMA_Priority = DMA_Priority_High;
	myDMA.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	myDMA.DMA_MemoryInc = DMA_MemoryInc_Enable;
	myDMA.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	myDMA.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	myDMA.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	myDMA.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	myDMA.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0,&myDMA);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
	myLED.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
	myLED.GPIO_Mode = GPIO_Mode_AIN;
	myLED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&myLED);
	
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	myADC.ADC_ScanConvMode = ENABLE;
	myADC.ADC_ContinuousConvMode = ENABLE;
	myADC.ADC_DataAlign = ADC_DataAlign_Right;
	myADC.ADC_Resolution = ADC_Resolution_12b;
	myADC.ADC_NbrOfConversion = 2;
	myADC.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_Init(ADC1,&myADC);
	
	ADC_RegularChannelConfig (ADC1,ADC_Channel_0,1,ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig (ADC1,ADC_Channel_1,2,ADC_SampleTime_480Cycles);
	
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
	
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_SoftwareStartConv(ADC1);
}
void timer3_pwm_sg90(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE); //PA7
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE); //Pb14
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef myGPIO;
	myGPIO.GPIO_Mode = GPIO_Mode_AF;
	myGPIO.GPIO_OType = GPIO_OType_PP;
	myGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myGPIO.GPIO_Speed = GPIO_Low_Speed;
	myGPIO.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOC,&myGPIO);
	
	GPIO_InitTypeDef myGPIO2;
	myGPIO2.GPIO_Mode = GPIO_Mode_AF;
	myGPIO2.GPIO_OType = GPIO_OType_PP;
	myGPIO2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myGPIO2.GPIO_Speed = GPIO_Low_Speed;
	myGPIO2.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA,&myGPIO2);
	
	GPIO_InitTypeDef myGPIO3;
	myGPIO3.GPIO_Mode = GPIO_Mode_AF;
	myGPIO3.GPIO_OType = GPIO_OType_PP;
	myGPIO3.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myGPIO3.GPIO_Speed = GPIO_Low_Speed;
	myGPIO3.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOB,&myGPIO3);
	
	TIM_TimeBaseInitTypeDef myTimer;
	myTimer.TIM_Prescaler = 1680-1;
	myTimer.TIM_Period = 1000-1;
	myTimer.TIM_ClockDivision = TIM_CKD_DIV1;
	myTimer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&myTimer);
	TIM_Cmd(TIM3,ENABLE);
	TIM_TimeBaseInit(TIM14,&myTimer);
	TIM_Cmd(TIM14,ENABLE);
	TIM_TimeBaseInit(TIM12,&myTimer);
	TIM_Cmd(TIM12,ENABLE);
	
	TIM_OCInitTypeDef myPWM;
	myPWM.TIM_OCMode = TIM_OCMode_PWM1;
	myPWM.TIM_OCPolarity = TIM_OCPolarity_High;
	myPWM.TIM_Pulse = 0;
	myPWM.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC1Init(TIM3,&myPWM);
	TIM_OC2Init(TIM3,&myPWM);
	TIM_OC3Init(TIM3,&myPWM);
	TIM_OC4Init(TIM3,&myPWM);
	TIM_OC1Init(TIM14,&myPWM);
	TIM_OC1Init(TIM12,&myPWM);
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM14);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM12);
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
	myGPIO.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOB,&myGPIO);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitTypeDef myGPIOo;
	myGPIOo.GPIO_Mode = GPIO_Mode_OUT;
	myGPIOo.GPIO_OType = GPIO_OType_PP;
	myGPIOo.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myGPIOo.GPIO_Speed = GPIO_High_Speed;
	myGPIOo.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOE,&myGPIOo);
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
void uart_to_esp(void)
{

	
	//readDHT11();
	sprintf(buffer,"#%d$",I_Temp);
	usart_send_string(buffer);
	sprintf(buffer,"^%d*",I_RH);
	usart_send_string(buffer);
	
	//gas
	if (adc_value[0]<100)
		sprintf(buffer,"!00%d@",adc_value[0]);
	else if (adc_value[0]<1000)
		sprintf(buffer,"!0%d@",adc_value[0]);
	else
		sprintf(buffer,"!%d@",adc_value[0]);
	
	usart_send_string(buffer);
	
	buffer[0]='(';
	state_led_LVR = GPIO_ReadOutputDataBit(GPIOB, led_LVR_pin);
	if(state_led_LVR)
	{
		buffer[1]='M';
	}
	else
	{
		buffer[1]='m';
	}
	
	state_led_KCR = GPIO_ReadOutputDataBit(GPIOB, led_KCR_pin);
	if(state_led_KCR)
	{
		buffer[2]='N';
	}
	else
	{
		buffer[2]='n';
	}
	
	state_led_BedR = GPIO_ReadOutputDataBit(GPIOB, led_BedR_pin);
	if(state_led_BedR)
	{
		buffer[3]='O';
	}
	else
	{
		buffer[3]='o';
	}
	
	state_led_BaR = GPIO_ReadOutputDataBit(GPIOB, led_BaR_pin);
	if(state_led_BaR)
	{
		buffer[4]='P';
	}
	else
	{
		buffer[4]='p';
	}
	
	state_led_Garage = GPIO_ReadOutputDataBit(GPIOB, led_Garage_pin);
	if(state_led_Garage)
	{
		buffer[5]='Q';
	}
	else
	{
		buffer[5]='q';
	}
	
	state_fan_LVR = GPIO_ReadOutputDataBit(GPIOB, fan_LVR_pin);
	if(state_fan_LVR==0)
	{
		buffer[6]='R';
	}
	else
	{
		buffer[6]='r';
	}
	
	state_fan_KCR = GPIO_ReadOutputDataBit(GPIOB, fan_KCR_pin);
	if(state_fan_KCR==0)
	{
		buffer[7]='S';
	}
	else
	{
		buffer[7]='s';
	}
	
	state_fan_BedR = GPIO_ReadOutputDataBit(GPIOB, fan_BedR_pin);
	if(state_fan_BedR)
	{
		buffer[8]='T';
	}
	else
	{
		buffer[8]='t';
	}

	if(state_door_garage)
	{
		buffer[9]='U';
	}
	else
	{
		buffer[9]='u';
	}

	if(state_clothes)
	{
		buffer[10]='V';
	}
	else
	{
		buffer[10]='v';
	}

	if(state_window)
	{
		buffer[11]='W';
	}
	else
	{
		buffer[11]='w';
	}
	
	if(adc_value[0]>2500) state_gas=1;
	else state_gas=0;
	if(state_gas)
	{
		buffer[12]='X';
	}
	else
	{
		buffer[12]='x';
	}
	
	if(state_fire)
	{
		buffer[13]='Y';
	}
	else
	{
		buffer[13]='y';
	}
	
	buffer[14]=')';
	usart_send_string(buffer);
}
void lcd_gpio (void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitTypeDef myLED;
	myLED.GPIO_Mode = GPIO_Mode_OUT;
	myLED.GPIO_OType = GPIO_OType_PP;
	myLED.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myLED.GPIO_Speed = GPIO_Low_Speed;
	myLED.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOE,&myLED);
}
void keypad_init(void)
{
	//init gpio
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef myLED1;
	myLED1.GPIO_Mode = GPIO_Mode_OUT;
	myLED1.GPIO_OType = GPIO_OType_PP;
	myLED1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myLED1.GPIO_Speed = GPIO_Low_Speed;
	myLED1.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOD,&myLED1);
	
	GPIO_InitTypeDef myLED2;
	myLED2.GPIO_Mode = GPIO_Mode_IN;
	myLED2.GPIO_OType = GPIO_OType_PP;
	myLED2.GPIO_PuPd = GPIO_PuPd_UP;
	myLED2.GPIO_Speed = GPIO_Low_Speed;
	myLED2.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOC,&myLED2);	
}
int8_t read_keypad (void)
{
	
	GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	GPIO_SetBits(GPIOD, GPIO_Pin_3);
	GPIO_SetBits(GPIOD, GPIO_Pin_4);
	GPIO_SetBits(GPIOD, GPIO_Pin_5);
	//delay_10_us(100);
	
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0))
	{
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));
		return '1';
	}
	
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1))
	{
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1));
		return '2';	
	}
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2))
	{
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2));
		return '3';	
	}
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3))
	{
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3));
		return 'A';
	}
	
	GPIO_ResetBits(GPIOD, GPIO_Pin_3);
	GPIO_SetBits(GPIOD, GPIO_Pin_4);
	GPIO_SetBits(GPIOD, GPIO_Pin_5);
	GPIO_SetBits(GPIOD, GPIO_Pin_2);
	//delay_10_us(100);
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0))
	{
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));
		return '4';
	}
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1))
	{
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1));
		return '5';
	}
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2))
	{
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2));
		return '6';	
	}
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3))
	{
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3));
		return 'B';
	}
	
	GPIO_ResetBits(GPIOD, GPIO_Pin_4);
	GPIO_SetBits(GPIOD, GPIO_Pin_5);
	GPIO_SetBits(GPIOD, GPIO_Pin_2);
	GPIO_SetBits(GPIOD, GPIO_Pin_3);
	//delay_10_us(100);
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0))
	{
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));
		return '7';
	}
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1))
	{
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1));
		return '8';
	}
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2))
	{
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2));
		return '9';
	}
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3))
	{
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3));
		return 'C';
	}
	
	GPIO_ResetBits(GPIOD, GPIO_Pin_5);
	GPIO_SetBits(GPIOD, GPIO_Pin_2);
	GPIO_SetBits(GPIOD, GPIO_Pin_3);
	GPIO_SetBits(GPIOD, GPIO_Pin_4);
	//delay_10_us(100);
	
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0))
	{
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0));
		return '*';
	}
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1))
	{
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1));
		return '0';
	}
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2))
	{
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2));
		return '#';
	}
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3))
	{
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3));
		return 'D';
	}
	
	return 'X';
}

void check_password(int8_t key_var)
{
	if(key_var != 'X')
	{
		lcd_write_1(1,check,key_var);
		delay_10_us(50000);
		lcd_write_1(1,check,'*');
		key_enter[check] = key_var;
		check++;
		if(check == 5)
		{
			for(int i=0;i<5;i++)
			{
				if(key_enter[i] != password[i])
				{
					lcd_write(0,0,(int8_t *)"Mat khau sai!   ");
					check=0;
					break;
				}
			}
			if (check == 5)
			{
				lcd_write(0,0,(int8_t *)"Chao mung ban!  ");
				TIM_SetCompare2(TIM3,40);//open
				delay_10_us(500000);
				TIM_SetCompare2(TIM3,90);
			}
			check=0;
			out=1;
		}
	}
}
void exti0_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	GPIO_InitTypeDef myBtn1;
	EXTI_InitTypeDef myExti;
	NVIC_InitTypeDef myNvic;
	
	
	myBtn1.GPIO_Mode = GPIO_Mode_IN;
	myBtn1.GPIO_OType = GPIO_OType_PP;
	myBtn1.GPIO_PuPd = GPIO_PuPd_UP;
	myBtn1.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOD,&myBtn1);
	
	myExti.EXTI_Line = EXTI_Line0;
	myExti.EXTI_Mode = EXTI_Mode_Interrupt;
	myExti.EXTI_Trigger = EXTI_Trigger_Falling;
	myExti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&myExti);
	
	//attach gpio pin to exti
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	myNvic.NVIC_IRQChannel = EXTI0_IRQn;
	myNvic.NVIC_IRQChannelPreemptionPriority = 0;
	myNvic.NVIC_IRQChannelSubPriority = 0;
	myNvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&myNvic);
	
}
void fire_sensor_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	GPIO_InitTypeDef myBtn1;
	EXTI_InitTypeDef myExti;
	NVIC_InitTypeDef myNvic;	
	
	myBtn1.GPIO_Mode = GPIO_Mode_IN;
	myBtn1.GPIO_OType = GPIO_OType_PP;
	myBtn1.GPIO_PuPd = GPIO_PuPd_UP;
	myBtn1.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOD,&myBtn1);
	
	myExti.EXTI_Line = EXTI_Line1;
	myExti.EXTI_Mode = EXTI_Mode_Interrupt;
	myExti.EXTI_Trigger = EXTI_Trigger_Falling;
	myExti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&myExti);
	
	//attach gpio pin to exti
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource1);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	myNvic.NVIC_IRQChannel = EXTI1_IRQn;
	myNvic.NVIC_IRQChannelPreemptionPriority = 0;
	myNvic.NVIC_IRQChannelSubPriority = 1;
	myNvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&myNvic);
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
			case 'A':
				GPIO_SetBits(GPIOB, led_LVR_pin);
				break;
			
			case 'a':
				GPIO_ResetBits(GPIOB, led_LVR_pin);
				break;
			
			case 'B':
				GPIO_SetBits(GPIOB, led_KCR_pin);
				break;
			
			case 'b':
				GPIO_ResetBits(GPIOB, led_KCR_pin);
				break;
			
			case 'C':
				GPIO_SetBits(GPIOB, led_BedR_pin);
				break;
			
			case 'c':
				GPIO_ResetBits(GPIOB, led_BedR_pin);
				break;
			
			case 'D':
				GPIO_SetBits(GPIOB, led_BaR_pin);
				break;
			
			case 'd':
				GPIO_ResetBits(GPIOB, led_BaR_pin);
				break;
			
			case 'E':
				GPIO_SetBits(GPIOB, led_Garage_pin);
				break;
			
			case 'e':
				GPIO_ResetBits(GPIOB, led_Garage_pin);
				break;
			
			case 'F':
				GPIO_ResetBits(GPIOB, fan_LVR_pin);
				break;
			
			case 'f':
				GPIO_SetBits(GPIOB, fan_LVR_pin);
				break;
			
			case 'G':
				GPIO_ResetBits(GPIOB, fan_KCR_pin);
				break;
			
			case 'g':
				GPIO_SetBits(GPIOB, fan_KCR_pin);
				break;
			
			case 'H':
				GPIO_SetBits(GPIOB, fan_BedR_pin);
				break;
			
			case 'h':
				GPIO_ResetBits(GPIOB, fan_BedR_pin);
				break;
			
			case 'I':
				state_door_garage=1;
				TIM_SetCompare1(TIM12,35);
				break;
			
			case 'i':
				state_door_garage=0;
				TIM_SetCompare1(TIM12,75);
				break;
			
			case 'J':
				state_clothes=1;
				TIM_SetCompare1(TIM3,80);
				break;
			
			case 'j':
				state_clothes=0;
				TIM_SetCompare1(TIM3,30);
				break;
			
			case 'K':
				state_window=1;
				TIM_SetCompare3(TIM3,35);
				break;
			
			case 'k':
				state_window=0;
				TIM_SetCompare3(TIM3,80);
				break;
			
			case '0': //turn off buzzer
				GPIO_ResetBits(GPIOB, GPIO_Pin_10);
				state_fire = 0;
				break;
			
			
			case 'L':
				state_gate=1;
				for (int j=0;j<10;j++)
				{
					GPIO_ToggleBits(GPIOB, GPIO_Pin_8);
					delay_10_us(10000);
				}
				if(gate_check%2) TIM_SetCompare4(TIM3,55);
				else TIM_SetCompare4(TIM3,15);
				gate_check ++;
				break;
			
			case 'l':
				state_gate=0;
				for (int j=0;j<10;j++)
				{
					GPIO_ToggleBits(GPIOB, GPIO_Pin_9);
					delay_10_us(10000);
				}
				break;
			
			default:
				break;
		}
		
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		delay_10_us(10000);
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)==RESET)
		{
			x++;
			lcd_write(0,0,(int8_t *)"Nhap mat khau:  ");
			lcd_write(1,0,(int8_t *)"      ");
			while(1)
			{
				key = read_keypad();
				check_password(key);
				delay_10_us(10000);//100ms
				if (out)
				{
					out=0;
					break;
				}
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	{
		delay_10_us(10000);
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)==RESET)
		{	
			
			state_fire = 1;
			GPIO_SetBits(GPIOB, GPIO_Pin_10);//buzzer
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}