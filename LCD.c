/*
 * LCD.c
 *
 *  Created on: 2020Äê4ÔÂ13ÈÕ
 *      Author: Leon
 */

#include <msp430.h>
#include <LCD.h>
#include <ROM.h>

#define SDA_H P2OUT |= BIT0
#define SDA_L P2OUT &=~ BIT0
#define SCL_H P2OUT |= BIT1
#define SCL_L P2OUT &=~ BIT1
#define A0_H P2OUT |= BIT2
#define A0_L P2OUT &=~ BIT2
#define RES_H P2OUT |= BIT3
#define RES_L P2OUT &=~ BIT3
#define CS_H P2OUT |= BIT4
#define CS_L P2OUT &=~ BIT4

void LCD_Send_Ctrl(unsigned char ctrl) //send control to LCD
{
    A0_L;
    CS_L;
    unsigned int i;
    for(i = 0;i < 8; i++)
    {
        SCL_L;
        if(ctrl & 0x80)
        {
            SDA_H;
        }
        else
        {
            SDA_L;
        }
        SCL_H;
        ctrl <<= 1;
    }
    SCL_L;
    CS_H;
}

void LCD_Send_Data(unsigned char data) //send data to LCD
{
    A0_H;
    CS_L;
    unsigned int i;
    for(i = 0;i < 8; i++)
    {
        SCL_L;
        if(data & 0x80)
        {
            SDA_H;
        }
        else
        {
            SDA_L;
        }
        SCL_H;
        data <<= 1;
    }
    SCL_L;
    CS_H;
}

void LCD_Init() //initialize LCD
{
    P2DIR |= 0xff;
    P1DIR &=~ BIT7;
    P2SEL &=~ 0xff;
    RES_L;
    RES_H;
    LCD_Send_Ctrl(0xe2);

    LCD_Send_Ctrl(0x2c);
    LCD_Send_Ctrl(0x2e);
    LCD_Send_Ctrl(0x2f);

    LCD_Send_Ctrl(0x23);
    LCD_Send_Ctrl(0x81);
    LCD_Send_Ctrl(0x28);

    LCD_Send_Ctrl(0xa2);

    LCD_Send_Ctrl(0xc8);
    LCD_Send_Ctrl(0xa0);
    LCD_Send_Ctrl(0x40);

    LCD_Send_Ctrl(0xaf);
}

void LCD_Cls() //clear LCD
{
    unsigned int i, j;
    for(i = 0; i < 9; i++)
    {
        LCD_Send_Ctrl(0xb0 + i);
        LCD_Send_Ctrl(0x10);
        LCD_Send_Ctrl(0x00);
        for(j = 0;j <132; j++)
        {
            LCD_Send_Data(0x00);
        }
    }
}

void LCD_Set_Address(unsigned int x, unsigned int y) //set display address(x=>column,y=>page)
{
    LCD_Send_Ctrl(0xb0 + y - 1);
    LCD_Send_Ctrl(0x10 + (x >> 4 & 0x0f));
    LCD_Send_Ctrl(x & 0x0f);
}

void LCD_Display_8x16(unsigned int x, unsigned y, unsigned char* data) //display 16x16 on LCD(x=>column, y=>page, data=>data to display)
{
    unsigned char *tmp = data;
    unsigned int i, j;
    for(j = 0;j < 2;j++)
    {
        LCD_Set_Address(x, y + j);
        for(i = 0;i < 8;i++)
        {
            LCD_Send_Data(*tmp);
            tmp++;
        }
    }
}

void LCD_Display_16x16(unsigned int x, unsigned y, unsigned char* data) //display 16x16 on LCD(x=>column, y=>page, data=>data to display)
{
    unsigned char *tmp = data;
    unsigned int i, j;
    for(j = 0;j < 2;j++)
    {
        LCD_Set_Address(x, y + j);
        for(i = 0;i < 16;i++)
        {
            LCD_Send_Data(*tmp);
            tmp++;
        }
    }
}

void LCD_Display_Zh(unsigned int x, unsigned int y, unsigned char *text) //display Chinese(GB2312 15x16) on LCD
                                                                          //(x=>column, y=>page, *text=>text to display)
{
    unsigned int i = 0;
    unsigned char AddrH, AddrM, AddrL;
    unsigned char buffer[32];
    unsigned long FontAddr;
    while(text[i] > 0x00)
    {
        if(((text[i] >= 0xb0) && (text[i] <= 0xf7)) && (text[i + 1] >= 0xa1))
        {
            FontAddr = (unsigned long) (text[i] - 0xb0) * 94;
            FontAddr += (unsigned long) (text[i + 1] - 0xa1) + 846;
            FontAddr = (unsigned long) FontAddr * 32;
            AddrH = (FontAddr & 0xff0000) >> 16;
            AddrM = (FontAddr & 0xff00) >>8;
            AddrL = (FontAddr & 0xff);
            Get_Bytes_From_ROM(AddrH, AddrM, AddrL, buffer, (unsigned int)(32));
            LCD_Display_16x16(x, y, buffer);
            i += 2;
            x += 16;
        }
        else if(((text[i] >= 0xa1) && (text[i] <= 0xa3)) && (text[i + 1] >= 0xa1))
        {
            FontAddr = (unsigned long) (text[i] - 0xa1) * 94;
            FontAddr += (unsigned long) (text[i + 1] - 0xa1);
            FontAddr = (unsigned long) FontAddr * 32;
            AddrH = (FontAddr & 0xff0000) >> 16;
            AddrM = (FontAddr & 0xff00) >>8;
            AddrL = (FontAddr & 0xff);
            Get_Bytes_From_ROM(AddrH, AddrM, AddrL, buffer, (unsigned int)32);
            LCD_Display_16x16(x, y, buffer);
            i += 2;
            x += 16;
        }
        else if((text[i] >= 0x20) && (text[i] <= 0x7e))
        {
            FontAddr = (unsigned long) text[i] - 0x20;
            FontAddr = (unsigned long) FontAddr * 16;
            FontAddr = (unsigned long) FontAddr + 0x3cf80;
            AddrH = (FontAddr & 0xff0000) >> 16;
            AddrM = (FontAddr & 0xff00) >>8;
            AddrL = (FontAddr & 0xff);
            Get_Bytes_From_ROM(AddrH, AddrM, AddrL, buffer, (unsigned int)16);
            LCD_Display_8x16(x, y, buffer);
            i += 1;
            x += 8;
        }
        else
        {
            i++;
        }
    }
}

void LCD_Display_En(unsigned int x, unsigned int y, unsigned char *text) //display English and number(ASCII 8*16) on LCD
                                                                          //(x=>column, y=>page, *text=>text to display)
{

}
