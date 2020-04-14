/*
 * LCD.h
 *
 *  Created on: 2020Äê4ÔÂ13ÈÕ
 *      Author: Leon
 */

#ifndef LCD_H_
#define LCD_H_

void LCD_Send_Ctrl(unsigned char ctrl); //send control to LCD
void LCD_Send_Data(unsigned char data); //send data to LCD
void LCD_Init(); //initialize LCD
void LCD_Cls(); //clear LCD
void LCD_Set_Address(unsigned int x, unsigned int y); //set display address(x=>column,y=>page)
void LCD_Display_8x16(unsigned int x, unsigned y, unsigned char* data); //display 16x16 on LCD(x=>column, y=>page, data=>data to display)
void LCD_Display_16x16(unsigned int x, unsigned y, unsigned char* data); //display 16x16 on LCD(x=>column, y=>page, data=>data to display)
void LCD_Display_Zh(unsigned int x, unsigned int y, unsigned char *text); //display Chinese(GB2312 15x16) on LCD
                                                                          //(x=>column, y=>page, *text=>text to display)
void LCD_Display_En(unsigned int x, unsigned int y, unsigned char *text); //display English and number(ASCII 8*16) on LCD
                                                                          //(x=>column, y=>page, *text=>text to display)


#endif /* LCD_H_ */
