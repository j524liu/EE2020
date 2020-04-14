/*
 * ROM.h
 *
 *  Created on: 2020Äê4ÔÂ14ÈÕ
 *      Author: Leon
 */

#ifndef ROM_H_
#define ROM_H_

void ROM_Send_Data(unsigned char data); //send data to ROM
unsigned char ROM_Read_Data(); //read data from ROM(one byte)
void Get_Bytes_From_ROM(unsigned char AddrH, unsigned char AddrM, unsigned char AddrL,
                        unsigned char *buffer, unsigned int DataLength); //read n bytes from ROM to buffer

#endif /* ROM_H_ */
