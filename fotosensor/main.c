#include <p18f4550.h>
#include <math.h>

// Variáveis Constantes
#define BOTAO_SA    PORTAbits.RA0
#define BOTAO_SB    PORTAbits.RA1 
#define TX          LATCbits.LATC0
#define DISTANCIA   0.016   // DISTANCIA = 0.16km.

// Variáveis Globais
unsigned int unidade;
unsigned int dezena;

void configPic()
{
	TRISAbits.RA0 = 1;
	TRISAbits.RA1 = 1;
	TRISCbits.RC0 = 0;
    TX = 1;
	ADCON1 = 0x0F; // Define as portas para TTL
}

void Delay_1ms()
{
	unsigned int i;

	for(i = 0; i<67; i++) {}
}

unsigned int leSensores()
{
	unsigned int tempoVelocidade = 0;

	while(BOTAO_SA){} 
	while(BOTAO_SB)    
	{
		Delay_1ms();
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

void enviaVelocidade(unsigned int dezena, unsigned int unidade)
{
	unsigned int i = 0, j = 0;


	
	//Inicio do envio da dezena com protocolo serial
	//TX em repouso	

	//TX start
	TX = 0;
	Delay_1ms();
	Delay_1ms();
	Delay_1ms();
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
void main(void)
{
    unsigned int tempoVeloc;
	configPic();
	while(1)
	{
		tempoVeloc = leSensores();
        calculaVelocidade(tempoVeloc);
        enviaVelocidade(dezena, unidade);
	}
}
