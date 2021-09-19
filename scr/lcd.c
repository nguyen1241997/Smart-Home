#include "lcd.h"
#include "main.h"

void lcd_send(uint8_t mode, uint8_t data)
{
	//HAL_Delay(2);
	for(int i=0;i<50000;i++);
	
	if(mode) //send data
	{
		Pin_high(RS_PORT,RS_PIN);
	}
	else //send commend
	{
		Pin_low(RS_PORT,RS_PIN);
	}
	
	//send MSB bits
	if(data&0x80)
	{
		Pin_high(D7_PORT,D7_PIN);
	}
	else
	{
		Pin_low(D7_PORT,D7_PIN);
	}
	
	if(data&0x40)
	{
		Pin_high(D6_PORT,D6_PIN);
	}
	else
	{
		Pin_low(D6_PORT,D6_PIN);
	}
	
	if(data&0x20)
	{
		Pin_high(D5_PORT,D5_PIN);
	}
	else
	{
		Pin_low(D5_PORT,D5_PIN);
	}
	
	if(data&0x10)
	{
		Pin_high(D4_PORT,D4_PIN);
	}
	else
	{
		Pin_low(D4_PORT,D4_PIN);
	}
	
	Pin_high(EN_PORT,EN_PIN);
	Pin_low(EN_PORT,EN_PIN);
	
	//send LSB bits
	if(data&0x08)
	{
		Pin_high(D7_PORT,D7_PIN);
	}
	else
	{
		Pin_low(D7_PORT,D7_PIN);
	}
	
	if(data&0x04)
	{
		Pin_high(D6_PORT,D6_PIN);
	}
	else
	{
		Pin_low(D6_PORT,D6_PIN);
	}
	
	if(data&0x02)
	{
		Pin_high(D5_PORT,D5_PIN);
	}
	else
	{
		Pin_low(D5_PORT,D5_PIN);
	}
	
	if(data&0x01)
	{
		Pin_high(D4_PORT,D4_PIN);
	}
	else
	{
		Pin_low(D4_PORT,D4_PIN);
	}
	
	Pin_high(EN_PORT,EN_PIN);
	Pin_low(EN_PORT,EN_PIN);
}

void lcd_init(void)
{
	lcd_send(0,0x28); //giao tiep 4 bit
	lcd_send(0,0x0c); //bat hien thi va tat con tro
	lcd_send(0,0x06); //tu dong di chuyen con tro den vi tri tiep theo
	lcd_send(0,0x01); //xoa man hinh
}

void lcd_clear(void)
{
	lcd_send(0,0x01);
}

void lcd_write(uint8_t x, uint8_t y,int8_t *data)
{
	switch(x)
	{
		case 0:
			lcd_send(0,0x80+0x00+y);
			break;
		case 1:
			lcd_send(0,0x80+0x40+y);
			break;
	}
	
	while (*data)
	{
		lcd_send(1,*data);
		data++;
	}
	
}
void lcd_write_1(uint8_t x, uint8_t y,int8_t data)
{
	switch(x)
	{
		case 0:
			lcd_send(0,0x80+0x00+y);
			break;
		case 1:
			lcd_send(0,0x80+0x40+y);
			break;
	}
	lcd_send(1,data);
}