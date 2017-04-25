#include <p18f4550.h>
#include <sw_uart.h>
#include <delays.h>

// Prot�tipo de fun��es
void Ini_Regs(void);
void DelayTXBitUART(void);
void DelayRXBitUART(void);
void DelayRXHalfBitUART(void);

void main(void)
{
    unsigned char dado;
    
    Ini_Regs();
    OpenUART();
    while(1)
    {
        dado = getcUART();
        PORTD = dado;
        
    }
    
}
void Ini_Regs(void)
{
    TRISA = 0b00000001;   	// RB0 e RB1 como entrada e os demais como sa�da
    TRISB = 0x00;   		// PORTB sa�da
    TRISC = 0b0000001;   		// PORTC sa�da
    TRISD = 0x00;   		// PORTD sa�da
    TRISE = 0x00;   		// PORTE sa�da
    ADCON1 = 0x0F;  		// configura pinos dos PORTA e PORTE como digitais

	PORTA = 0;		// limpa PORTA
	PORTB = 0;  	// limpa PORTB
	PORTC = 0;		// limpa PORTC
	PORTD = 0x00;	// limpa PORTD
	PORTE = 0;		// limpa PORTE
}
void DelayTXBitUART() //Gera um atraso equivalente a 6655 ciclos de maq.
{
    Delay1KTCYx(6);
    Delay100TCYx(6);
    Delay10TCYx(5);
    Delay1TCY();
    Delay1TCY();
    Delay1TCY();    
    Delay1TCY();
    Delay1TCY();
}

void DelayRXBitUART(void) //Gera um atraso equivalente a 6653 ciclos de maq.
{
    Delay1KTCYx(6);
    Delay100TCYx(6);
    Delay10TCYx(5);
    Delay1TCY();
    Delay1TCY();
    Delay1TCY();
    
}

void DelayRXHalfBitUART() //Gera um atraso equivalente a 3324 ciclos de maq.
{
    Delay1KTCYx(3);
    Delay100TCYx(3);
    Delay10TCYx(2);
    Delay1TCY();
    Delay1TCY();
    Delay1TCY();
    Delay1TCY();
}