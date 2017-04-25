#include <p18f4550.h>
#include <delays.h>

// Variáveis Constantes
#define RX          PORTAbits.RA0

// Variáveis Globais
unsigned char unidade, dezena, dado;
unsigned int k;

/***** Protótipo de funções ********/
void Ini_Regs(void);
void DelayRXBitUART(void);
void DelayRXHalfBitUART(void);
unsigned char recebeVelocidade(void);
unsigned char mostraVelocidade( unsigned char dado );
void displaybcd(unsigned char bcd);

void main(void)
{
    unsigned char bcd;
	Ini_Regs();
	while(1)
	{
		while(RX) {};
        	dezena = recebeVelocidade();
		while(RX) {};
            unidade = recebeVelocidade();
		    k = 0;
            bcd = mostraVelocidade(dezena);
			bcd = bcd | mostraVelocidade(unidade);
			displaybcd(bcd);
       
    }
}

// Deve gerar um atraso equivalente a 6666 ciclos de máquina
void DelayRXBitUART()
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

// Deve gerar um atraso equivalente a 6666 ciclos de máquina
void DelayRXHalfBitUART(void)
{
    Delay1KTCYx(3);     // Gera um atraso de 6*1000 ciclos de instrução
    Delay100TCYx(3);    // Gera um atraso de 6*100 ciclos de instrução
    Delay10TCYx(3);     // Gera um atraso de 6*10 ciclos de instrução
    //Delay1TCY();      // Gera um atraso de 1 ciclos de instrução
    //Delay1TCY();      // Gera um atraso de 1 ciclos de instrução
    //Delay1TCY();      // Gera um atraso de 1 ciclos de instrução
    //Delay1TCY();      // Gera um atraso de 1 ciclos de instrução
    //Delay1TCY();      // Gera um atraso de 1 ciclos de instrução
    //Delay1TCY();      // Gera um atraso de 1 ciclos de instrução
}

void Ini_Regs()
{
    TRISA = 0b00000001;   	// RB0 e RB1 como entrada e os demais como saída
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
    RX = 1;
}

unsigned char recebeVelocidade(void)
{
    unsigned int i = 0, j = 0; 
	unsigned char aux = 0;   
    // Inicialmente RX é 1  
	DelayRXHalfBitUART();
    DelayRXBitUART();
       while(i < 7)
       {
			aux = RX;
			aux = aux << 7;
            dado = dado | aux;
			dado = dado >> 1;
            i++;
            DelayRXBitUART();
       }
    // Stop Bit
    DelayRXBitUART();
//    DelayRXHalfBitUART();
      
    return dado;             
}

unsigned char mostraVelocidade( unsigned char dado )
{
	unsigned int i = 0;
	unsigned char velocidade;
    
	// Tirando de ASCII e deixando em bin
   	//dezena  = dezena - 0x30;
	//unidade = unidade - 0x30;
    dado = dado - 0x30;
	if ( k == 0 )
    {
        dado = dado << 4;
        k = 1;
    }
	velocidade = dado;

	return velocidade;
}

void displaybcd(unsigned char bcd)
{
	PORTD = bcd;		
}

