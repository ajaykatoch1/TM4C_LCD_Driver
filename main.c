//TM4C 16x2 LCD Driver
#include <tm4c123gh6pm.h> //ARM CMSIS 
#include <stdio.h>
#include <string.h>

#define EIGHT_BIT_MODE 0x38
#define SHIFT_CURSOR_RIGHT 0x06
#define DISPLAY_ON_CURSOR_OFF 0x0C
#define DISPLAY_ON_CURSOR_ON 0x0E
#define CLEAR_DISPLAY 0x01
#define CURSOR_START_OF_ROW 0x80

void lcd_cmd(unsigned char command);
void LCD_init(void);
void delay_milli(int n);
void lcd_data(unsigned char data);
void lcd_string(char const *str);

int main()
{
  LCD_init();
  while(1){
    lcd_cmd(0x80); //places cursor on top row at start
    lcd_string("Hello World!");
  } 
}

void LCD_init(void)
{
  SYSCTL->RCGC2 |= 0x02; //port B clock
  SYSCTL->RCGC2 |= 0x08; //port D clock

  GPIOD->DIR |= 0xBF;
  GPIOD->DIR &= ~0x40;
  GPIOD->DEN |= 0xFF;

  GPIOB->DIR |= 0xFF;
  GPIOB->DEN |= 0xFF; //digital enable and ouput direction selected for all pins on port B and D

  lcd_cmd(EIGHT_BIT_MODE); 
  lcd_cmd(SHIFT_CURSOR_RIGHT);
  lcd_cmd(DISPLAY_ON_CURSOR_OFF);
  lcd_cmd(CLEAR_DISPLAY);
}

void delay_milli(int n)
{
  int i,j;
  for(i=0;i<n;i++)
  for(j=0;j<3180;j++)
  {}
}

void lcd_data(unsigned char data)
{
  GPIOB->DATA = data; //set port B equal to data bits
  GPIOD->DATA |= 0x01; //RS set high
  GPIOD->DATA &= ~0x02;//RW set low
  GPIOD->DATA |= 0x04;//Enable set high
  delay_milli(10);
  GPIOD->DATA &= ~0x04; //E set low
}

void lcd_cmd(unsigned char command)
{
  GPIOB->DATA = command; //set port B equal to command bits
  GPIOD->DATA &= ~0x01; //RS set low
  GPIOD->DATA &= ~0x02; //RW set low
  GPIOD->DATA |= 0x04; //Enable set high
  delay_milli(10);
  GPIOD->DATA &= ~0x04; //Enable set low
}

void lcd_string(char const *str)
{
  char i = 0;
  for(i = 0; i< strlen(str);i++){
    lcd_data(str[i]);
  }
}
