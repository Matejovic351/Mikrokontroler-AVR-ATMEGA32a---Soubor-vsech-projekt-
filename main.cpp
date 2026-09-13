/*
 * Zamek.cpp
 *
 * Created: 27.08.2026 21:00:20
 * Author : rezacm
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

const uint8_t klavesy[4][4] = {
	{1, 4, 7, 11},
	{2, 5, 8, 0},
	{3, 46, 9, 10},
	{12, 13, 14, 15}
};

uint8_t cti_klavesy(){
	for (uint8_t sloupce = 0; sloupce < 4; sloupce++)
	{
		PORTC = ~(0b00000001 << sloupce)
		for (uint8_t radky = 0; radky < 4; radky++)
		{
			if (((~PINC)&(0b00010000 << radky)) > 0)
			{
				return klavesy[sloupce][radky];
			}
		}
	}
	return 99;
}

void zapis_pulz(){
	PORTB = (PINB & 0b00111111);
	_delay_us(5);
	PORTB = (PINB | 0b10000000);
	_delay_us(50);
	PORTB = (PINB & 0b00111111);
	_delay_us(50);
}

void data_pulz(){
	PORTB = (PINB | 0b01000000);
	_delay_us(5);
	PORTB = (PINB | 0b11000000);
	_delay_us(50);
	PORTB = (PINB & 0b01000000);
	_delay_us(5);
	PORTB = (PINB & 0b00000000);
	_delay_us(50);
}

void Inicializace(){
	_delay_ms(50);
	PORTD = 0b00111100;
	zapis_pulz();
	_delay_us(1);
	zapis_pulz();
	_delay_us(1);
	zapis_pulz();
	_delay_us(1);
	PORTD = 0b00000001;
	zapis_pulz();
	_delay_ms(50);
	PORTD = 0b00001111;
	zapis_pulz();
}

void zapis_znaku(uint8_t znak){
	PORTD = znak;
	data_pulz();
}

void pozice(uint8_t radky, uint8_t sloupce){
	if (radky == 1)
	{
		PORTD = (0b10000000 | sloupce);
	}else if (radky == 2)
	{
		PORTD = (0b11000000 | sloupce);
	}
	zapis_pulz();
	_delay_ms(1);
}

void Clear(){
	PORTD = 0b00000001;
	zapis_pulz();
	_delay_ms(100);
}
uint8_t kl = 0;

uint32_t heslo1 = 1111;
uint32_t heslo2 = 2222;
uint32_t heslo3 = 3333;
uint32_t heslo4 = 4444;

uint32_t puk = 1234;

uint8_t mode = 0;		// 0-zad v n  hesel, 1-v b r hesla pro zm nu, 2-zm na hesla, 3-zp t na zad v n 
uint32_t zadavam = 0;
uint8_t vybrano = 0;

int main(void)
{
	DDRC = 0b00001111;
	DDRB = 0b11000000;
	DDRD = 0b11111111;
	
	Inicializace();
	Inicializace();
	Clear();
	pozice(1,0);
	zapis_znaku('Z');
	zapis_znaku('A');
	zapis_znaku('D');
	zapis_znaku('E');
	zapis_znaku('J');
	zapis_znaku(' ');
	zapis_znaku('H');
	zapis_znaku('E');
	zapis_znaku('S');
	zapis_znaku('L');
	zapis_znaku('O');
	zapis_znaku(':');
	pozice(2,0);
	mode = 0;
	while (1)
	{
		kl = cti_klavesy();
		if (kl != 99)
		{
			if (mode == 0)
			{
				if (kl < 10)
				{
					zadavam *= 10;
					zadavam += kl;
					zapis_znaku('0' + kl);
				}
				if (kl == 11)
				{
					if (zadavam == heslo1)
					{
						Clear();
						pozice(1, 0);
						zapis_znaku('Z');
						zapis_znaku('A');
						zapis_znaku('D');
						zapis_znaku('A');
						zapis_znaku('N');
						zapis_znaku('O');
						zapis_znaku(':');
						zapis_znaku('H');
						zapis_znaku('E');
						zapis_znaku('S');
						zapis_znaku('L');
						zapis_znaku('O');
						zapis_znaku('1');
						zadavam = 0;
						mode = 3;	
					}
					else if (zadavam == heslo2)
					{
						Clear();
						pozice(1, 0);
						zapis_znaku('Z');
						zapis_znaku('A');
						zapis_znaku('D');
						zapis_znaku('A');
						zapis_znaku('N');
						zapis_znaku('O');
						zapis_znaku(':');
						zapis_znaku('H');
						zapis_znaku('E');
						zapis_znaku('S');
						zapis_znaku('L');
						zapis_znaku('O');
						zapis_znaku('2');
						zadavam = 0;
						mode = 3;
					}
					else if (zadavam == heslo3)
					{
						Clear();
						pozice(1, 0);
						zapis_znaku('Z');
						zapis_znaku('A');
						zapis_znaku('D');
						zapis_znaku('A');
						zapis_znaku('N');
						zapis_znaku('O');
						zapis_znaku(':');
						zapis_znaku('H');
						zapis_znaku('E');
						zapis_znaku('S');
						zapis_znaku('L');
						zapis_znaku('O');
						zapis_znaku('3');
						zadavam = 0;
						mode = 3;
					}
					else if (zadavam == heslo4)
					{
						Clear();
						pozice(1, 0);
						zapis_znaku('Z');
						zapis_znaku('A');
						zapis_znaku('D');
						zapis_znaku('A');
						zapis_znaku('N');
						zapis_znaku('O');
						zapis_znaku(':');
						zapis_znaku('H');
						zapis_znaku('E');
						zapis_znaku('S');
						zapis_znaku('L');
						zapis_znaku('O');
						zapis_znaku('4');
						zadavam = 0;
						mode = 3;
					}
					else if (zadavam == puk)
					{
						Clear();
						pozice(1, 0);
						zapis_znaku('V');
						zapis_znaku('Y');
						zapis_znaku('B');
						zapis_znaku('R');
						zapis_znaku('R');
						zapis_znaku(' ');
						zapis_znaku('1');
						zapis_znaku('-');
						zapis_znaku('4');
						pozice(2, 0);
						zadavam = 0;
						mode = 1;					
					}else{
						Clear();
						pozice(1, 0);
						zapis_znaku('Z');
						zapis_znaku('A');
						zapis_znaku('D');
						zapis_znaku('A');
						zapis_znaku('N');
						zapis_znaku('A');
						zapis_znaku(':');
						zapis_znaku('C');
						zapis_znaku('H');
						zapis_znaku('Y');
						zapis_znaku('B');
						zapis_znaku('A');
						zapis_znaku('!');
						zadavam = 0;
						mode = 3;
					}
				}
			}
			else if (mode == 2)
			{
				if (kl < 10)
				{
					zadavam *= 10;
					zadavam += kl;
					zapis_znaku('0' + kl);
				}
				else if (kl == 11)
				{
					if (vybrano == 1)
					{
						heslo1 = zadavam;
						zadavam = 0;
						vybrano = 0;
						Clear();
						pozice(1, 0);
						zapis_znaku('H');
						zapis_znaku('E');
						zapis_znaku('S');
						zapis_znaku('L');
						zapis_znaku('O');
						zapis_znaku('1');
						zapis_znaku(' ');
						zapis_znaku('Z');
						zapis_znaku('M');
						zapis_znaku('E');
						zapis_znaku('N');
						zapis_znaku('E');
						zapis_znaku('N');
						zapis_znaku('O');
						pozice(2, 0);
						
					}
					else if (vybrano == 2)
					{
						heslo2 = zadavam;
						zadavam = 0;
						vybrano = 0;
						Clear();
						pozice(1, 0);
						zapis_znaku('H');
						zapis_znaku('E');
						zapis_znaku('S');
						zapis_znaku('L');
						zapis_znaku('O');
						zapis_znaku('2');
						zapis_znaku(' ');
						zapis_znaku('Z');
						zapis_znaku('M');
						zapis_znaku('E');
						zapis_znaku('N');
						zapis_znaku('E');
						zapis_znaku('N');
						zapis_znaku('O');
						pozice(2, 0);
					}
					else if (vybrano == 3)
					{
						heslo3 = zadavam;
						zadavam = 0;
						vybrano = 0;
						Clear();
						pozice(1, 0);
						zapis_znaku('H');
						zapis_znaku('E');
						zapis_znaku('S');
						zapis_znaku('L');
						zapis_znaku('O');
						zapis_znaku('3');
						zapis_znaku(' ');
						zapis_znaku('Z');
						zapis_znaku('M');
						zapis_znaku('E');
						zapis_znaku('N');
						zapis_znaku('E');
						zapis_znaku('N');
						zapis_znaku('O');
						pozice(2, 0);
					}
					else if (vybrano == 4)
					{
						heslo4 = zadavam;
						zadavam = 0;
						vybrano = 0;
						Clear();
						pozice(1, 0);
						zapis_znaku('H');
						zapis_znaku('E');
						zapis_znaku('S');
						zapis_znaku('L');
						zapis_znaku('O');
						zapis_znaku('4');
						zapis_znaku(' ');
						zapis_znaku('Z');
						zapis_znaku('M');
						zapis_znaku('E');
						zapis_znaku('N');
						zapis_znaku('E');
						zapis_znaku('N');
						zapis_znaku('O');
						pozice(2, 0);
					}
				}
			}
			else if (mode == 3)
			{
				Clear();
				zapis_znaku('Z');
				zapis_znaku('A');
				zapis_znaku('D');
				zapis_znaku('E');
				zapis_znaku('J');
				zapis_znaku(' ');
				zapis_znaku('H');
				zapis_znaku('E');
				zapis_znaku('S');
				zapis_znaku('L');
				zapis_znaku('O');
				pozice(2, 0);
				mode = 0;
			}
		}
		while(cti_klavesy() != 99);
		_delay_ms(100);
	}
}
