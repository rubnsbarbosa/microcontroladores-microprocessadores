#include <p18f4550.h>
#include <math.h>
#include <delays.h>
#include <timers.h>

// Variáveis Constantes
#define BOTAO_SA    PORTAbits.RA0
#define BOTAO_SB    PORTAbits.RA1 
#define TX          PORTCbits.RC0
#define DISTANCIA   0.016   // DISTANCIA = 0.16km.

// Variáveis Globais
unsigned char unidade, dezena;

/***** Protótipo de funções ********/
void Ini_Regs(void);
void DelayTXBitUART(void);
unsigned int leSensores(void);
void calculaVelocidade(float tempoVelocidade);
void enviaVelocidade(unsigned char dado);

void main(void)
{
    unsigned int tempoVeloc;
	Ini_Regs();
	while(1)
	{
		tempoVeloc = leSensores();
        calculaVelocidade(tempoVeloc);
        enviaVelocidade(dezena);
        enviaVelocidade(unidade);
	}
}

// Deve gerar um atraso equivalente a 6666 ciclos de máquina
void DelayTXBitUART(void)
{
    Delay1KTCYx(6);     // Gera um atraso de 6*1000 ciclos de instrução
    Delay100TCYx(6);    // Gera um atraso de 6*100 ciclos de instrução
    Delay10TCYx(6);     // Gera um atraso de 6*10 ciclos de instrução
    //Delay1TCY();      // Gera um atraso de 1 ciclos de instrução
    //Delay1TCY();      // Gera um atraso de 1 ciclos de instrução
    //Delay1TCY();      // Gera um atraso de 1 ciclos de instrução
    //Delay1TCY();      // Gera um atraso de 1 ciclos de instrução
    //Delay1TCY();      // Gera um atraso de 1 ciclos de instrução
    //Delay1TCY();      // Gera um atraso de 1 ciclos de instrução
}

void Ini_Regs()
{
    TRISA = 0b00000011;   	// RB0 e RB1 como entrada e os demais como saída
    TRISB = 0x00;   		// PORTB saída
    TRISC = 0x00;   		// PORTC saída
    TRISD = 0x00;   		// PORTD saída
    TRISE = 0x00;   		// PORTE saída
    ADCON1 = 0x0F;  		// configura pinos dos PORTA e PORTE como digitais

	PORTA = 0;		// limpa PORTA
	PORTB = 0;  	// limpa PORTB
	PORTC = 0;		// limpa PORTC
	PORTD = 0x00;	// limpa PORTD
	PORTE = 0;		// limpa PORTE
    TX = 1;         // TX em silencio
}

unsigned int leSensores()
{
	unsigned int tempoVelocidade = 0;

	while(!BOTAO_SA){}
	WriteTimer0(25);
	while(!BOTAO_SB)	{}
	tempoVelocidade=ReadTimer0();
	CloseTimer0 ();
    return tempoVelocidade;
}

void calculaVelocidade(float tempoVelocidade)
{
	unsigned int velocidade;
    
    // Conversao de ms para h
    tempoVelocidade = tempoVelocidade * ( 0.28 * pow(10, -6) );
    
	velocidade = DISTANCIA / tempoVelocidade;
	unidade = velocidade % 10;
	dezena  = velocidade / 10;
	dezena  = dezena  + 0x30;
	unidade = unidade + 0x30;
}

void enviaVelocidade(unsigned char dado)
{
	unsigned int i = 0;
    
	//Inicio do envio da dezena com protocolo serial
	//TX inicialmente em repouso(nivel logico 1)	
	TX = 0;     //TX start
    DelayTXBitUART();
	while (i < 8)
	{
		TX = dado;
		dado = dado >> 1;
        DelayTXBitUART();
		i++;
	}
	TX = 1;     //TX stop
    DelayTXBitUART();
    dado = 0;
	//Fim do envio da dezena com protocolo serial
}

