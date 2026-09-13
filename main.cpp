/*
 * Kalkulacka.cpp
 *
 * Created: 26.08.2026 21:49:03
 * Author : rezacm
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

const uint8_t klavesy[4][4] = {
	{1,4,7,11},
	{2,5,8,0},
	{3,6,9,10},
	{12,13,14,15} //+,-,*,/
};

uint8_t cti_klavesy(){
	for(uint8_t sloupec = 0 ; sloupec < 4 ; sloupec++){
		PORTC = ~(0b00000001 << sloupec);
		for(uint8_t radky = 0; radky < 4 ; radky++){
			if(((~PINC)&(0b00010000<<radky))>0){
				return klavesy[sloupec][radky];
			}
		}
	}
	return 99;
}

void Zapis_pulz(){
	PORTB = (PINB & 0b00111111);
	_delay_us(5);
	PORTB = (PINB | 0b10000000);
	_delay_us(50);
	PORTB = (PINB & 0b00111111);
	_delay_us(50);
}

void Data_pulz(){
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
	Zapis_pulz();
	_delay_us(1);
	Zapis_pulz();
	_delay_us(1);
	Zapis_pulz();
	_delay_us(1);
	PORTD = 0b00000001;
	Zapis_pulz();
	_delay_ms(50);
	PORTD = 0b00001111;
	Zapis_pulz();
}

void zapis_znaku(uint8_t znak){
	PORTD = znak;
	Data_pulz();
}


void pozice(uint8_t radek, uint8_t sloupec){
	if(radek == 1)
	{
		PORTD = 0b10000000 | sloupec;
		}
		else if (radek == 2)
		{
		PORTD = 0b11000000 | sloupec;
	}
	Zapis_pulz();
	_delay_us(1);
}

void Clear(){
	PORTD = 0b00000001;
	Zapis_pulz();
	_delay_ms(50);
}

uint8_t kl = 0;
uint8_t vypocet = 0;
uint32_t cislo1 = 0;
uint32_t cislo2 = 0;
uint32_t vysledek = 0;

void cely_vysledek(uint32_t vysledek){		//vysledek nap . 12345
	uint32_t vypis = vysledek;
	uint32_t rad = 1;
	
	while (vypis > 9)
	{
		rad *= 10;
		vypis = vypis/10;
	}
	vypis = vysledek;
	while(rad > 0)
	{
		uint32_t zbytek = vypis/rad;
		zapis_znaku(zbytek + 48);
		vypis = vypis%rad;
		rad = rad/10;
	}
}


void proved_vypocet(uint32_t cislo1, uint8_t vypocet, uint32_t cislo2){
	Clear();
	pozice(1, 0);
	zapis_znaku('V');
	zapis_znaku('Y');
	zapis_znaku('S');
	zapis_znaku('L');
	zapis_znaku('E');
	zapis_znaku('D');
	zapis_znaku('E');
	zapis_znaku('K');
	zapis_znaku(':');
	pozice(2, 0);
	
	if (vypocet == 1)
	{
		vysledek = (cislo1 + cislo2);
		cely_vysledek(vysledek);
	}else if (vypocet == 2)
	{
		vysledek = (cislo1 - cislo2);
		cely_vysledek(vysledek);
	}
	else if (vypocet == 3)
	{
		vysledek = (cislo1 * cislo2);
		cely_vysledek(vysledek);
	}
	else if (vypocet == 4)
	{
		if (cislo2 != 0)
		{
			vysledek = (cislo1/cislo2);
			cely_vysledek(vysledek);
		}
		else
		{
			zapis_znaku('N');
			zapis_znaku('E');
			zapis_znaku('L');
			zapis_znaku('Z');
			zapis_znaku('E');	
			zapis_znaku('!');
		}
	}
}

void proved_vypocet(uint32_t cislo1, uint8_t vypocet, uint32_t cislo2){
	Clear();
	pozice(1, 0);
	zapis_znaku('V');
	zapis_znaku('Y');
	zapis_znaku('S');
	zapis_znaku('L');
	zapis_znaku('E');
	zapis_znaku('D');
	zapis_znaku('E');
	zapis_znaku('K');
	zapis_znaku(':');
	pozice(2, 0);
	
	if (vypocet == 1)
	{
		vysledek = (cislo1 + cislo2);
		cely_vysledek(vysledek);
	}
	else if (vypocet == 2)
	{
		vysledek = (cislo1 - cislo2);
		cely_vysledek(vysledek);
	}
	else if (vypocet == 3)
	{
		vysledek = (cislo1 * cislo2);
		cely_vysledek(vysledek);
	}
	else if (vypocet == 4)
	{
		if (cislo2 != 0)
		{
			vysledek = (cislo1 / cislo2);
			cely_vysledek(vysledek);
		}
		else
		{
			zapis_znaku('N');
			zapis_znaku('E');
			zapis_znaku('L');
			zapis_znaku('Z');
			zapis_znaku('E');
			zapis_znaku('!');
		}
		
	}
}


int main(void)
{
	DDRC = 0b00001111;
	DDRD = 0b11111111;
	DDRB = 0b11000000;
	
	Inicializace();
	Inicializace();
	Clear();
	pozice(1,0);
	
	while (1)
	{
		kl = cti_klavesy();
		
		if (kl != 99)
		{
			if (vypocet == 0)
			{
				if (kl < 10)
				{
					cislo1 *= 10;
					cislo1 += kl;
					zapis_znaku('0' + kl);
				}
				else if (kl == 11)
				{
					cislo1 = 0;
					cislo2 = 0;
					vypocet = 0;
					Clear();
				}
				else if (kl == 12)
				{
					vypocet = 1;
					zapis_znaku('+');
				}
				else if (kl == 13)
				{
					vypocet = 2;
					zapis_znaku('-');
				}
				else if (kl == 14)
				{
					vypocet = 3;
					zapis_znaku('*');
				}
				else if (kl == 15)
				{
					vypocet = 4;
					zapis_znaku('/');
				}
				if (kl < 10)
				{
					cislo2 *= 10;
					cislo2 += kl;
					zapis_znaku('0' + kl);
				}
				else if (kl == 11)
				{
					cislo1 = 0;
					cislo2 = 0;
					vypocet = 0;
					Clear();
				}
				else if (kl == 10)
				{
					proved_vypocet(cislo1, vysledek, cislo2);
					cislo1 = 0;
					cislo2 = 0;
					vysledek = 0;
				}	
			}
		}
		while(cti_klavesy() != 99);
		_delay_ms(100);
	}
