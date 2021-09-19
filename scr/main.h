#include "stm32f4xx.h"

#define GPIO_PORT				GPIOB

#define LED_LVR_PIN			GPIO_Pin_0
#define LED_KCR_PIN			GPIO_Pin_1
#define LED_BedR_PIN		GPIO_Pin_2
#define LED_BaR_PIN			GPIO_Pin_3
#define LED_Garage_PIN	GPIO_Pin_4
#define FAN_LVR_PIN			GPIO_Pin_6
#define FAN_KCR_PIN			GPIO_Pin_5
#define FAN_BedR_PIN		GPIO_Pin_7

#define LED_GATE_GREEN	GPIO_Pin_8
#define LED_GATE_RED		GPIO_Pin_9

#define DHT11_PORT			GPIOA
#define DHT11_PIN				GPIO_Pin_8

#define KEYPAD_ROW_PORT		GPIOD
#define KEYPAD_COL_PORT		GPIOC

#define KEYPAD_ROW_1_PIN	GPIO_Pin_2
#define KEYPAD_ROW_2_PIN	GPIO_Pin_3
#define KEYPAD_ROW_3_PIN	GPIO_Pin_4
#define KEYPAD_ROW_4_PIN	GPIO_Pin_7

#define KEYPAD_COL_1_PIN 	GPIO_Pin_0
#define KEYPAD_COL_2_PIN 	GPIO_Pin_1
#define KEYPAD_COL_3_PIN 	GPIO_Pin_2
#define KEYPAD_COL_4_PIN 	GPIO_Pin_3

#define OPEN_CLOTHES		TIM_SetCompare1(TIM3,40)
#define CLOSE_CLOTHES		TIM_SetCompare1(TIM3,80)

#define OPEN_GARAGE			TIM_SetCompare2(TIM3,40)
#define CLOSE_GARAGE		TIM_SetCompare2(TIM3,80)

#define OPEN_WINDOW			TIM_SetCompare3(TIM3,40)
#define CLOSE_WINDOW		TIM_SetCompare3(TIM3,80)

#define OPEN_GATE				TIM_SetCompare4(TIM3,40)
#define CLOSE_GATE			TIM_SetCompare4(TIM3,80)

#define OPEN_DOOR				TIM_SetCompare1(TIM12,40)
#define CLOSE_DOOR			TIM_SetCompare1(TIM12,80)


/*
-PA6: DHT11
-PA0: gas sensor
-PA1: water sensor

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


void delay_10_us(uint32_t time);

void delay_ms(uint32_t time);

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

void check_password2(int8_t key_var);

void exti0_init(void);

void fire_sensor_init(void);

void check_RFID(void);

uint8_t CompareID(uint8_t* CardID, uint8_t* CompareID);

