#include "stm32f4xx.h"

#define LCD20xN

#define RS_PORT	GPIOE
#define RS_PIN	GPIO_Pin_0

#define EN_PORT	GPIOE
#define EN_PIN	GPIO_Pin_1

#define D4_PORT	GPIOE
#define D4_PIN	GPIO_Pin_2

#define D5_PORT	GPIOE
#define D5_PIN	GPIO_Pin_3

#define D6_PORT	GPIOE
#define D6_PIN	GPIO_Pin_4

#define D7_PORT	GPIOE
#define D7_PIN	GPIO_Pin_5


#define Pin_low(PORT,PIN)		GPIO_ResetBits(PORT,PIN);
#define Pin_high(PORT,PIN)	GPIO_SetBits(PORT,PIN);

void lcd_init(void);
void lcd_clear(void);
void lcd_send(uint8_t mode, uint8_t data);
void lcd_write(uint8_t x, uint8_t y, int8_t *data);
void lcd_write_1(uint8_t x, uint8_t y,int8_t data);
	