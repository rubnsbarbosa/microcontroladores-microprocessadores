#include <p18f4550.h>

#define LED PORTDbits.RD0

unsigned char x, uart;

/***** Prot�tipo de fun��es ********/
void Ini_Regs(void);
void Servico_Alta_Prioridade (void);
/***********************************/
#pragma code int_alta = 0x0008  // Define a interrupcao de alta prioridade

void int_alta()
{
	_asm
		GOTO Servico_Alta_Prioridade	// Desvia o programa para a fun��o
                                        // Servico_Alta_Prioridade.
	_endasm
}
#pragma code

/**** Rotina de tratamento de interrup��o *******/
#pragma interrupt Servico_Alta_Prioridade
void Servico_Alta_Prioridade()
{
	if( !INTCONbits.TMR0IF );
	else
	{
		INTCONbits.TMR0IF = 0;		//interrup��o de estouro de TMR0?
		//TMR0L = 0xF5;				//inicializa TMR0 
        //TMR0H = 0xE5;
		TMR0L = 0x05;				//inicializa TMR0 
        TMR0H = 0xE6;
        
		if( !(x == 255) ) x++;		//se x diferente de 125, incrementa x
		else                        //sen�o, incrementa contador
		{
			x = 0;
			uart++;
			if (uart == 10) uart = 0;
		} 
	}
}
/********* Fim da rotina de interrup��o ***********/
void main()
{
    Ini_Regs();
	while(1)
	{
		// escreve programa principal
	}
}

void Ini_Regs()
{
    TRISA = 0x00;   // PORTA sa�da
    TRISB = 0x01;   // RB0 como entrada e os demais como sa�da
    TRISC = 0x00;   // PORTC sa�da
    TRISD = 0x00;   // PORTD sa�da
    TRISE = 0x00;   // PORTE sa�da
    ADCON1 = 0x0F;  // configura pinos dos PORTA e PORTE como digitais

	PORTA = 0;		// limpa PORTA
	PORTB = 0;  	// limpa PORTB
	PORTC = 0;		// limpa PORTC
	PORTD = 0x00;	// limpa PORTD
	PORTE = 0;		// limpa PORTE


    // configura TMR0 para operar como temporizador e estourar a cada 3,333ms(tempo desejado)
	T0CON = 0b10001110;				//Dispara ou Para o TMR0 <7>
                                    //Timer0 configurado como contador de 16 bits<6>
                                    //Timer0 configurado como temporizador<5>
									//prescaler desativado<3>
									//fator de prescaler de 1:128<2:0>   
	
    TMR0L = 0x05;				//inicializa TMR0 
    TMR0H = 0xE6;

    // Habilita interrup��o de estouro de TMR0
	INTCONbits.TMR0IE = 1;			// Ativa a interrup��o do TMR0
	INTCON2bits.TMR0IP = 1;			// Define a prioridade da interrup��o do TMR0
	INTCONbits.GIE = 1;				// Liga chave geral de interrup��o de alta prioridade.

}

