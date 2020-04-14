#include <msp430.h> 
#include <LCD.h>
#include <CharTable.h>

unsigned int state = 0;
unsigned int state_last = 0;

void Show_Home();
void Init_Clock();
void Init_IO();
void Show_Measure();
void Show_Wave();

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	Init_Clock();
	Init_IO();
	LCD_Init();

	LCD_Cls();
	_EINT();
    while(1)
    {
        switch(state)
        {
        case 0:
            Show_Home();
            break;
        case 1:
            Show_Measure();
            break;
        case 2:
            Show_Wave();
            break;
        default:
            break;
        }
    }

//	return 0;
}

void Show_Home()
{
    if(state != state_last)
    {
        LCD_Cls();
        state_last = state;
    }
    LCD_Display_Zh(1, 1, "王璋&刘家");
    LCD_Display_16x16(73, 1, jun);
    LCD_Display_Zh(1, 3, "2020F012");
}

void Show_Measure()
{
    if(state != state_last)
    {
        LCD_Cls();
        state_last = state;
    }
    LCD_Display_Zh(1, 1, "Vpp: 3.0V");
    LCD_Display_Zh(1, 3, "Vrms: 1.5V");
}

void Show_Wave()
{
    if(state != state_last)
    {
        LCD_Cls();
        state_last = state;
    }
    LCD_Display_Zh(1, 1, "当前波形");
}

void Init_Clock()
{
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;
}

void Init_IO()
{
    P1DIR &=~ BIT3;
    P1REN |= BIT3;
    P1OUT |= BIT3;
    P1IES |= BIT3;
    P1IE = BIT3;
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_IRS()
{
    unsigned char key = P1IN & P1IFG;
    __delay_cycles(160000);
    if(key == (P1IN & BIT3))
    {
        if(state == 2)
        {
            state = 0;
        }
        else
        {
            state_last = state;
            state++;
        }
    }
    P1IFG = 0;
}
