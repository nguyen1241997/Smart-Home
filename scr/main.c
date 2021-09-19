#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "main.h"
#include "mfrc522.h"

uint8_t status;
uint8_t g_ucTempbuf[4];
bool flag_loop=0;
uint8_t defaultKeyA[16] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t MyID[4] = {0x81,0x46,0xec,0x09};
int c=0;

uint8_t I_RH=0, D_RH, I_Temp=0, D_Temp;
bool flag_dht = false;

volatile uint16_t adc_value[2];

volatile uint16_t x=0;

volatile char data_receive;
char buffer1[20];
char buffer[10];


int8_t password[5]={"12345"};
int8_t check=0;
int8_t key_enter[5];
int8_t key;
volatile uint8_t out=0;

int main(void)
{
	
	
	//keypad_init();
	/*
	lcd_gpio();
	lcd_init();
	lcd_write(0,0,(int8_t *)" ");
	lcd_write(0,3,(int8_t *)"SMART HOME");
	
	
	
	adc_dma_init();
	
	exti0_init();//lcd interrupt
	
	fire_sensor_init();
	
	*/
	led_init();
	gpio_init();
	timer7_interrput();
	timer6_delay_init();
	timer3_pwm_sg90();
	uart2_init();
	
	MF522_init();
  MFRC522_Reset();
  MFRC522_AntennaOn();
	CLOSE_GATE;

	while(1)
	{
		
		
		//key=read_keypad();
		//check_password2(key);
//		if (adc_value[0]>110)
//		{
//			TIM_SetCompare3(TIM3,35);;//open window
//			strcat(buffer,"KL");
//		}
//		else
//		{
//			strcat(buffer,"l");
//		}
//				

		if (flag_dht)
		{
			flag_dht = false;
//			readDHT11();
//			uart_to_esp();
		}
		check_RFID();
		delay_ms(50);
	}
}


//interrupt program
void TIM7_IRQHandler(void) //3s
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		flag_dht = true;
	}
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		data_receive = USART_ReceiveData(USART2);
		
		char x = data_receive;
		
		strcat(buffer,&x);
		
		switch(data_receive)
		{
			case 'A':
				GPIO_SetBits(GPIO_PORT, LED_LVR_PIN);
				break;
			
			case 'a':
				GPIO_ResetBits(GPIO_PORT, LED_LVR_PIN);
				break;
			
			case 'B':
				GPIO_SetBits(GPIO_PORT, LED_KCR_PIN);
				break;
			
			case 'b':
				GPIO_ResetBits(GPIO_PORT, LED_KCR_PIN);
				break;
			
			case 'C':
				GPIO_SetBits(GPIO_PORT, LED_BedR_PIN);
				break;
			
			case 'c':
				GPIO_ResetBits(GPIO_PORT, LED_BedR_PIN);
				break;
			
			case 'D':
				GPIO_SetBits(GPIO_PORT, LED_BaR_PIN);
				break;
			
			case 'd':
				GPIO_ResetBits(GPIO_PORT, LED_BaR_PIN);
				break;
			
			case 'E':
				GPIO_SetBits(GPIO_PORT, LED_Garage_PIN);
				break;
			
			case 'e':
				GPIO_ResetBits(GPIO_PORT, LED_Garage_PIN);
				break;
			
			case 'F':
				GPIO_SetBits(GPIO_PORT, FAN_LVR_PIN);
				break;
			
			case 'f':
				GPIO_SetBits(GPIO_PORT, FAN_LVR_PIN);
				break;
			
			case 'G':
				GPIO_SetBits(GPIO_PORT, FAN_KCR_PIN);
				break;
			
			case 'g':
				GPIO_SetBits(GPIO_PORT, FAN_KCR_PIN);
				break;
			
			case 'H':
				GPIO_SetBits(GPIO_PORT, FAN_BedR_PIN);
				break;
			
			case 'h':
				GPIO_ResetBits(GPIO_PORT, FAN_BedR_PIN);
				break;
			
			case 'I':
				OPEN_GARAGE;
				break;
			
			case 'i':
				CLOSE_GARAGE;
				break;
			
			case 'J':
				if (adc_value[1]>110)	OPEN_CLOTHES;
				break;
			
			case 'j':
				CLOSE_CLOTHES;
				break;
			
			case 'K':
				OPEN_WINDOW;
				break;
			
			case 'k':
				if (adc_value[0]>110)	CLOSE_WINDOW;
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
			//state_fire = 1;
			GPIO_SetBits(GPIOB, GPIO_Pin_10);//buzzer
			strcat(buffer,"M");
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
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
void delay_ms(uint32_t time)
{
	for(int i=0; i<time; i++)
	{
		TIM_SetCounter(TIM6,0);
		while(TIM_GetCounter(TIM6)<1000);
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
	DHT11_TypeDef.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA,&DHT11_TypeDef);
	
	GPIO_ResetBits(DHT11_PORT, DHT11_PIN);
	
	delay_ms(20);
	
	GPIO_SetBits(DHT11_PORT, DHT11_PIN);
}

void response_dht11(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef DHT11_TypeDef;
	
	DHT11_TypeDef.GPIO_Mode = GPIO_Mode_IN;
	DHT11_TypeDef.GPIO_OType = GPIO_OType_PP;
	DHT11_TypeDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	DHT11_TypeDef.GPIO_Speed = GPIO_High_Speed;
	DHT11_TypeDef.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA,&DHT11_TypeDef);
	
	while(GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN));
	while(GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)==0);
	while(GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN));
}

uint8_t receive_dht11(void)
{
	int t = 0;
	for (int i=0; i<8; i++)
	{
		while(GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)==0);
		delay_10_us(5);
		if(GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN))/* if high pulse is greater than 30us */
		{
			t |= (1<<(7-i));
		}
		else
		{
			t &= ~(1<<(7-i));
		}
		while(GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN));
	}
	return t;
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

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE); //PB14 PB15
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef myGPIO;
	myGPIO.GPIO_Mode = GPIO_Mode_AF;
	myGPIO.GPIO_OType = GPIO_OType_PP;
	myGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myGPIO.GPIO_Speed = GPIO_Low_Speed;
	myGPIO.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOC,&myGPIO);
	
	GPIO_InitTypeDef myGPIO3;
	myGPIO3.GPIO_Mode = GPIO_Mode_AF;
	myGPIO3.GPIO_OType = GPIO_OType_PP;
	myGPIO3.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myGPIO3.GPIO_Speed = GPIO_Low_Speed;
	myGPIO3.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB,&myGPIO3);
	
	TIM_TimeBaseInitTypeDef myTimer;
	myTimer.TIM_Prescaler = 1680-1;
	myTimer.TIM_Period = 1000-1;
	myTimer.TIM_ClockDivision = TIM_CKD_DIV1;
	myTimer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&myTimer);
	TIM_Cmd(TIM3,ENABLE);
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
	TIM_OC1Init(TIM12,&myPWM);
	TIM_OC2Init(TIM12,&myPWM);
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM12);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);
}

void timer7_interrput(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
	
	TIM_TimeBaseInitTypeDef myTim7;
	myTim7.TIM_Prescaler = 8400-1;
	myTim7.TIM_Period = 30000-1; //10000=1s
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
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF; //This is important. We will this pin except of INPUT, OUTPUT and ANALOG so we set as Alternate Function
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6;   //Communicate on PA2 and PA3
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//We set the pin as Push Pull
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; //And Pull Up. Keep it always on HIGH
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; //And pin frequency
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
	
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
	sprintf(buffer1,"#%d$^%d*!%d@",I_Temp,I_RH,adc_value[0]);

	usart_send_char('_');
	usart_send_string(buffer1);
	usart_send_string(buffer);
	
	for (int i=0; i<10; i++)
	{
		buffer[i]=NULL;
	}
}

void lcd_gpio (void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitTypeDef myGPIO;
	myGPIO.GPIO_Mode = GPIO_Mode_OUT;
	myGPIO.GPIO_OType = GPIO_OType_PP;
	myGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myGPIO.GPIO_Speed = GPIO_Low_Speed;
	myGPIO.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOE,&myGPIO);
}

void keypad_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef myGPIO;
	myGPIO.GPIO_Mode = GPIO_Mode_OUT;
	myGPIO.GPIO_OType = GPIO_OType_PP;
	myGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
	myGPIO.GPIO_Speed = GPIO_Low_Speed;
	myGPIO.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7;
	GPIO_Init(GPIOD,&myGPIO);
	
	GPIO_InitTypeDef myGPIO1;
	myGPIO1.GPIO_Mode = GPIO_Mode_IN;
	myGPIO1.GPIO_OType = GPIO_OType_PP;
	myGPIO1.GPIO_PuPd = GPIO_PuPd_UP;
	myGPIO1.GPIO_Speed = GPIO_Low_Speed;
	myGPIO1.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOC,&myGPIO1);	
}

int8_t read_keypad (void)
{
	
	GPIO_ResetBits(KEYPAD_ROW_PORT, KEYPAD_ROW_1_PIN);
	GPIO_SetBits(KEYPAD_ROW_PORT, KEYPAD_ROW_2_PIN);
	GPIO_SetBits(KEYPAD_ROW_PORT, KEYPAD_ROW_3_PIN);
	GPIO_SetBits(KEYPAD_ROW_PORT, KEYPAD_ROW_4_PIN);
	//delay_10_us(100);
	
	if (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_1_PIN))
	{
		while (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_1_PIN));
		return '1';
	}
	
	if (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_2_PIN))
	{
		while (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_2_PIN));
		return '2';	
	}
	if (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_3_PIN))
	{
		while (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_3_PIN));
		return '3';	
	}
	if (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_4_PIN))
	{
		while (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_4_PIN));
		return 'A';
	}
	
	GPIO_SetBits(KEYPAD_ROW_PORT, KEYPAD_ROW_1_PIN);
	GPIO_ResetBits(KEYPAD_ROW_PORT, KEYPAD_ROW_2_PIN);
	GPIO_SetBits(KEYPAD_ROW_PORT, KEYPAD_ROW_3_PIN);
	GPIO_SetBits(KEYPAD_ROW_PORT, KEYPAD_ROW_4_PIN);
	
	//delay_10_us(100);
	if (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_1_PIN))
	{
		while (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_1_PIN));
		return '4';
	}
	if (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_2_PIN))
	{
		while (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_2_PIN));
		return '5';
	}
	if (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_3_PIN))
	{
		while (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_3_PIN));
		return '6';	
	}
	if (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_4_PIN))
	{
		while (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_4_PIN));
		return 'B';
	}
	
	GPIO_SetBits(KEYPAD_ROW_PORT, KEYPAD_ROW_1_PIN);
	GPIO_SetBits(KEYPAD_ROW_PORT, KEYPAD_ROW_2_PIN);
	GPIO_ResetBits(KEYPAD_ROW_PORT, KEYPAD_ROW_3_PIN);
	GPIO_SetBits(KEYPAD_ROW_PORT, KEYPAD_ROW_4_PIN);

	//delay_10_us(100);
	if (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_1_PIN))
	{
		while (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_1_PIN));
		return '7';
	}
	if (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_2_PIN))
	{
		while (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_2_PIN));
		return '8';
	}
	if (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_3_PIN))
	{
		while (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_3_PIN));
		return '9';
	}
	if (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_4_PIN))
	{
		while (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_4_PIN));
		return 'C';
	}
	
	GPIO_SetBits(KEYPAD_ROW_PORT, KEYPAD_ROW_1_PIN);
	GPIO_SetBits(KEYPAD_ROW_PORT, KEYPAD_ROW_2_PIN);
	GPIO_SetBits(KEYPAD_ROW_PORT, KEYPAD_ROW_3_PIN);
	GPIO_ResetBits(KEYPAD_ROW_PORT, KEYPAD_ROW_4_PIN);
	//delay_10_us(100);
	
	if (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_1_PIN))
	{
		while (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_1_PIN));
		return '*';
	}
	if (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_2_PIN))
	{
		while (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_2_PIN));
		return '0';
	}
	if (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_3_PIN))
	{
		while (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_3_PIN));
		return '#';
	}
	if (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_4_PIN))
	{
		while (!GPIO_ReadInputDataBit(KEYPAD_COL_PORT, KEYPAD_COL_4_PIN));
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

void check_password2(int8_t key_var)
{
	if(key_var != 'X')
	{
		key_enter[check] = key_var;
		check++;
		if(check == 5)
		{
			for(int i=0;i<5;i++)
			{
				if(key_enter[i] != password[i])
				{
					break;
					check=0;
				}
			}
			if (check == 5)
			{
				TIM_SetCompare2(TIM3,40);//open
				delay_10_us(500000);
				TIM_SetCompare2(TIM3,90);//close
			}
			check=0;
		}
	}
}

void exti0_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	GPIO_InitTypeDef myBtn;
	EXTI_InitTypeDef myExti;
	NVIC_InitTypeDef myNvic;
	
	myBtn.GPIO_Mode = GPIO_Mode_IN;
	myBtn.GPIO_OType = GPIO_OType_PP;
	myBtn.GPIO_PuPd = GPIO_PuPd_UP;
	myBtn.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOD,&myBtn);
	
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

uint8_t CompareID(uint8_t* CardID, uint8_t* CompareID) 
{
	uint8_t i;
	for (i = 0; i < 4; i++) {
		if (CardID[i] != CompareID[i]) 
			{
			return MI_ERR;
		  }
	}
	return MI_OK;
}
void check_RFID(void)
{
	status = MFRC522_Request(PICC_REQALL, g_ucTempbuf);
    if (status != MI_OK)
			{  
		  flag_loop=0;  
      return;
    }
			
		status = MFRC522_Anticoll(g_ucTempbuf);
    if (status != MI_OK)
		{  
		 flag_loop=0;  
		 return;
		}
		
	  if(flag_loop==1) 
		{
		 MFRC522_Halt();	
		 return;
		}
		
		flag_loop=1;
		MFRC522_Halt();
		
		static bool flag_gate = false;
		
	  if (CompareID(g_ucTempbuf,MyID) == MI_OK) 
		{
			for (int i=0; i<6; i++)
			{
				GPIO_ToggleBits(GPIO_PORT, LED_GATE_GREEN);
				delay_ms(150);
			}
			if (!flag_gate)
			{
				flag_gate = true;
				OPEN_GATE;
			}
			else
			{
				flag_gate = false;
				CLOSE_GATE;
			}
		} 
		else 
		{
			for (int i=0; i<6; i++)
			{
				GPIO_ToggleBits(GPIO_PORT, LED_GATE_RED);
				delay_ms(150);
			}
		}	
}