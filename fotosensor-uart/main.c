#include <p18f4550.h>
#include <math.h>
#include <delays.h>

// Variáveis Constantes
#define BOTAO_SA    PORTAbits.RA0
#define BOTAO_SB    PORTAbits.RA1 
#define TX          LATCbits.LATC0
#define DISTANCIA   0.016   // DISTANCIA = 0.16km.

// Variáveis Globais
unsigned int unidade, dezena;
unsigned char i, uart;
/***** Protótipo de funções ********/
void Ini_Regs(void);
void Servico_Alta_Prioridade (void);
unsigned int leSensores(void);
void calculaVelocidade(float tempoVelocidade);
/***********************************/
/**** Rotina de tratamento de interrupção *******/
#pragma interrupt Servico_Alta_Prioridade
void Servico_Alta_Prioridade()
{
	switch(i)
	{
		case 0: TX = 0;
				if (INTCONbits.TMR0IF)
					INTCONbits.TMR0IF = 0;
				i++;
				break;
                    
		case 1:	TX = dezena;
				dezena = dezena >> 1;
				if (INTCONbits.TMR0IF)
					INTCONbits.TMR0IF = 0;
				i++;
				break;
                    
		case 2: TX = dezena;
				dezena = dezena >> 1;
				if (INTCONbits.TMR0IF)
					INTCONbits.TMR0IF = 0;
				i++;
				break;
                    
		case 3: TX = dezena;
				dezena = dezena >> 1;
				if (INTCONbits.TMR0IF)
					INTCONbits.TMR0IF = 0;
				i++;
				break;
                    
		case 4: TX = dezena;
				dezena = dezena >> 1;
				if (INTCONbits.TMR0IF)
					INTCONbits.TMR0IF = 0;
				i++;
				break;

		case 5: TX = dezena;
				dezena = dezena >> 1;
				if (INTCONbits.TMR0IF)
					INTCONbits.TMR0IF = 0;
				i++;
				break;

		case 6: TX = dezena;
				dezena = dezena >> 1;
				if (INTCONbits.TMR0IF)
					INTCONbits.TMR0IF = 0;
				i++;
				break;

		case 7: TX = dezena;
				dezena = dezena >> 1;
				if (INTCONbits.TMR0IF)
					INTCONbits.TMR0IF = 0;
				i++;
				break;

		case 8: TX = dezena;
				dezena = dezena >> 1;
				if (INTCONbits.TMR0IF)
					INTCONbits.TMR0IF = 0;
				i++;
				break;

		case 9: TX = 1;
				if (INTCONbits.TMR0IF)
				{
					INTCONbits.TMR0IF = 0;
					i = 0;
					T0CONbits.TMR0ON = 0;
				}
				break;

        }
/*
	if( !INTCONbits.TMR0IF )
	{
		if(x == 0)
		{
			TX = 0;
			x = 1;
		}
	}
	else
	{
		INTCONbits.TMR0IF = 0;		//interrupção de estouro de TMR0?
		TX = dezena;
		dezena = dezena >> 1;
		TMR0L = 0x05;				//inicializa TMR0 
        TMR0H = 0xE6;
	}
*/
}
/********* Fim da rotina de interrupção ***********/
void main(void)
{
    unsigned int tempoVeloc;
	Ini_Regs();
	while(1)
	{
		tempoVeloc = leSensores();
        calculaVelocidade(tempoVeloc);
		T0CONbits.TMR0ON = 1;
        //enviaVelocidade(dezena, unidade);
	}
}
void Ini_Regs()
{
    TRISA = 0b00000011;   	// PORTA saída
    TRISB = 0x01;   		// RB0 como entrada e os demais como saída
    TRISC = 0b00000001;   		// PORTC saída
    TRISD = 0x00;   		// PORTD saída
    TRISE = 0x00;   		// PORTE saída
    ADCON1 = 0x0F;  		// configura pinos dos PORTA e PORTE como digitais

	PORTA = 0;		// limpa PORTA
	PORTB = 0;  	// limpa PORTB
	PORTC = 0;		// limpa PORTC
	PORTD = 0x00;	// limpa PORTD
	PORTE = 0;		// limpa PORTE


    // configura TMR0 para operar como temporizador e estourar a cada 3,333ms(tempo desejado)
	T0CON = 0b00001110;				//Dispara ou Para o TMR0 <7>
                                    //Timer0 configurado como contador de 16 bits<6>
                                    //Timer0 configurado como temporizador<5>
									//prescaler desativado<3>
									//fator de prescaler de 1:128<2:0>   
	
    TMR0L = 0x05;				//inicializa TMR0 
    TMR0H = 0xE6;

    // Habilita interrupção de estouro de TMR0
	INTCONbits.TMR0IE = 1;			// Ativa a interrupção do TMR0
	INTCON2bits.TMR0IP = 1;			// Define a prioridade da interrupção do TMR0
	INTCONbits.GIE = 1;				// Liga chave geral de interrupção de alta prioridade.

}

unsigned int leSensores()
{
	unsigned int tempoVelocidade = 0;

	while(BOTAO_SA){} 
	while(BOTAO_SB)    
	{
    	Delay1KTCYx(2);	// Delay de 1ms. Calculo: 2*1000*0.5us = 1ms
		tempoVelocidade++;
	} 

	return tempoVelocidade;
}

void calculaVelocidade(float tempoVelocidade)
{
	//unsigned int tempoVelocidade = leSensores();
	unsigned int velocidade;
	//unsigned int unidade;
	//unsigned int dezena;
    
    // Conversao de ms para h
    tempoVelocidade = tempoVelocidade * ( 0.28 * pow(10, -6) );
    
	velocidade = DISTANCIA / tempoVelocidade;
	unidade = velocidade % 10;
	dezena  = velocidade / 10;
	dezena  = dezena  + 0x30;
	unidade = unidade + 0x30;
	//dezena = 'U';
	//unidade = 'U';
}
/*
void enviaVelocidade(unsigned int dezena, unsigned int unidade)
{
	unsigned int i = 0, j = 0;
	//Inicio do envio da dezena com protocolo serial
	//TX em repouso	

	//TX start
	TX = 0;
	TMR0ON = 1;
	if (INTCONbits.TMR0IF)
	while (i < 8)
	{
		j = 0;
		//dezena = dezena % 2;
		//dezena = dezena / 2;
		TX = dezena;
		dezena = dezena >> 1;
        while (j  < 3){
            Delay_1ms(); //Fazer a função delay para 3,33 ms
            j++;
        }
		i++;
	}
	//TX stop
	TX = 1;
	Delay_1ms();
	Delay_1ms();
	Delay_1ms();
	//Fim do envio da dezena com protocolo serial

	//Inicio do envio da unidade com protocolo serial
	//TX em repouso	
	//TX = 1;
	//TX start
	TX = 0;
	Delay_1ms();
	Delay_1ms();
	Delay_1ms();
    i = 0;
	while (i < 8)
	{
		j = 0;
		//unidade = unidade % 2;
		//unidade = unidade / 2;
		TX = unidade;
		unidade = unidade>>1;
        while(j < 3 ){
            Delay_1ms(); //Fazer a função delay para 1200 ms
            j++;
        }
		i++;
	}
	//TX stop
	TX = 1;
	//Fim do envio da unidade com protocolo serial
}
*/
