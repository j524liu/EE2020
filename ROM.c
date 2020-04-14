/*
 * ROM.c
 *
 *  Created on: 2020Äê4ÔÂ14ÈÕ
 *      Author: Leon
 */

#include <msp430.h>
#include <ROM.h>

#define ROM_IN_H P2OUT |= BIT5
#define ROM_IN_L P2OUT &=~ BIT5
#define ROM_SCL_H P2OUT |= BIT6
#define ROM_SCL_L P2OUT &=~ BIT6
#define ROM_CS_H P2OUT |= BIT7
#define ROM_CS_L P2OUT &=~ BIT7
#define ROM_OUT P1IN & BIT7

void ROM_Send_Data(unsigned char data) //send data to ROM
{
    unsigned int i;
    for(i = 0;i < 8;i++)
    {
        ROM_SCL_L;
        if(data & 0x80)
        {
            ROM_IN_H;
        }
        else
        {
            ROM_IN_L;
        }
        ROM_SCL_H;
        data <<= 1;
    }
}

unsigned char ROM_Read_Data() //read data from ROM(one byte)
{
    unsigned int i;
    unsigned char data_read = 0;
    ROM_SCL_H;
    for(i = 0;i < 8;i++)
    {
        ROM_SCL_L;
        data_read <<= 1;
        if(ROM_OUT)
        {
            data_read += 1;
        }
        else
        {
            data_read += 0;
        }
        ROM_SCL_H;
    }
    return data_read;
}

void Get_Bytes_From_ROM(unsigned char AddrH, unsigned char AddrM, unsigned char AddrL,
                        unsigned char* buffer, unsigned int DataLength) //read n bytes from ROM to buffer
{
    unsigned int i;
    ROM_CS_L;
    ROM_Send_Data(0x03);
    ROM_Send_Data(AddrH);
    ROM_Send_Data(AddrM);
    ROM_Send_Data(AddrL);
    for(i = 0;i < DataLength;i++)
    {
        *(buffer + i) = ROM_Read_Data();
    }
    ROM_CS_H;
}
