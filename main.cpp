/*
 * Hodiny a Budikem.cpp
 *
 * Created: 29.08.2026 22:35:17
 * Author : rezacm
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

const uint8_t klavesy[4][4] = {
	{1, 4, 7, 11},
	{2, 5, 8, 0},
	{3, 6, 9, 10},
	{12, 13, 14, 15}
};

uint8_t cti_klavesy(){
	for(uint8_t sloupce = 0; sloupce < 4; sloupce++)
	{
		PORTC = ~(0b00000001 << sloupce);
		for (uint8_t radky = 0; radky < 4; radky++)
		{
			if (((~PINC)&(0b00010000 << radky)) > 0)
			{
				return klavesy [sloupce][radky];
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

void inicializace(){
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

void pozice(uint8_t radek, uint8_t sloupec){
	if (radek == 1)
	{
		PORTD = (0b10000000 | sloupec);
	}else if (radek == 2)
	{
		PORTD = (0b11000000 | sloupec);
	}
	zapis_pulz();
	_delay_ms(1);
}

void Clear(){
	PORTD = 0b00000001;
	zapis_pulz();
	_delay_ms(50);
}

void zobraz_hodiny(uint8_t sec, uint8_t min, uint8_t hod)
{
	pozice(1, 0);
	zapis_znaku('H');
	zapis_znaku('O');
	zapis_znaku('D');
	zapis_znaku('I');
	zapis_znaku('N');
	zapis_znaku('Y');
	zapis_znaku(':');
	pozice(1, 8);
	zapis_znaku('0'+((hod/10)%10));
	zapis_znaku('0'+(hod%10));
	zapis_znaku(':');
	zapis_znaku('0'+((min/10)%10));
	zapis_znaku('0'+(min%10));
	zapis_znaku(':');
	zapis_znaku('0'+((sec/10)%10));
	zapis_znaku('0'+(sec%10));
}

void zobraz_budik(uint8_t b_sec, uint8_t b_min, uint8_t b_hod){
		pozice(2, 0);
		zapis_znaku('B');
		zapis_znaku('U');
		zapis_znaku('D');
		zapis_znaku('I');
		zapis_znaku('K');
		zapis_znaku(':');
		pozice(2, 8);
		zapis_znaku('0'+((b_hod/10)%10));
		zapis_znaku('0'+(b_hod%10));
		zapis_znaku(':');
		zapis_znaku('0'+((b_min/10)%10));
		zapis_znaku('0'+(b_min%10));
		zapis_znaku(':');
		zapis_znaku('0'+((b_sec/10)%10));
		zapis_znaku('0'+(b_sec%10));
}

void blikej(){
	static uint8_t stav = 0;
	stav = !stav;
	
	if(stav)
	{
		pozice(1, 0);
		for (uint8_t x = 0; x < 16; x++)
		{
			zapis_znaku('-');
		}
		pozice(2, 0);
		for (uint8_t x = 0; x < 16; x++)
		{
			zapis_znaku('-');
		}
	}
	else
	{
		Clear();
	}
	_delay_ms(100);
}

uint8_t kl = 0;
uint32_t millis = 0;

uint8_t sec = 0;
uint8_t min = 0;
uint8_t hod = 0;

uint8_t b_sec = 0;
uint8_t b_min = 0;
uint8_t b_hod = 0;

uint8_t mode = 0;
uint8_t ukazatel = 0;
uint8_t zobrazuju = 0;
int main(void)
{
	DDRD = 0b11111111;
	DDRB = 0b11000000;
	DDRC = 0b00001111;
	
	TIMSK = 0b00000001;
	TCCR0 = 0b00000011;
	sei();
	
	inicializace();
	inicializace();
	Clear();
	pozice(1, 0);
	
    while (1) 
    {
		if (mode == 2)
		{
			if (hod == b_hod && min == b_min && sec == b_sec)
			{
				zobrazuju = 1;
			}
		}
		if (zobrazuju == 1)
		{
			blikej();
		}
		else
		{
			zobraz_hodiny(sec, min, hod);
			zobraz_budik(b_sec, b_min, b_hod);
		}
		
		kl = cti_klavesy();
		if (kl != 99)
		{
			if (zobrazuju == 1)
			{
				zobrazuju = 0;
			}
			else if (kl == 12)
			{
				mode = 0;
				ukazatel = 0;
			}
			else if (kl == 13)
			{
				mode = 1;
				ukazatel = 0;
			}
			else if (kl == 14)
			{
				ukazatel++;
				if (ukazatel > 2)
				{
					ukazatel = 0;
				}
			}else if (kl == 11)
			{
				mode = 2;
			}
			
			if (mode == 0)
			{
				if (kl < 11)
				{
					if (ukazatel == 0)
					{
						if (kl == 10)
						{
							sec = 0;
						}
						else
						{
							sec *= 10;
							sec += kl;	
						}
					}
					else if(ukazatel == 1)
					{
						if (kl == 10)
						{
							min = 0;	
						}
						else
						{
							min *= 10;
							min += kl;	
						}
					}
					else if(ukazatel == 2)
					{
						if (kl == 10)
						{
							hod = 0;
						}
						else
						{
							hod *= 10;
							hod += kl;
						}
					}
					
				}
			}
			else if (mode == 1)
			{
				if (kl < 11)
				{
					if (ukazatel == 0)
					{
						if (kl == 10)
						{
							b_sec = 0;
						}
						else
						{
							b_sec *= 10;
							b_sec += kl;
						}
					}
					else if (ukazatel == 1)
					{
						if (kl == 10)
						{
							b_min = 0;
						}
						else
						{
							b_min *= 10;
							b_min += kl;
						}
					}
					else if (ukazatel == 2)
					{
						if (kl == 10)
						{
							b_hod = 0;
						}
						else
						{
							b_hod *= 10;
							b_hod += kl;
						}
					}
				}
			}
			else if (mode == 3)
			{
				mode = 2;
			}
			
		}
		while(cti_klavesy() != 99);
		_delay_ms(100);
	}
}

ISR(TIMER0_OVF_vect){
	TCNT0 = 6;
	
	if (mode == 2)		//beh programu
	{
		millis++;
		if (millis > 999)
		{
			millis = 0;
			sec++;
		}
		if (sec > 59)
		{
			sec = 0;
			min++;
		}
		if (min > 59)
		{
			min = 0;
			hod++;
		}
		if (hod > 23)
		{
			hod = 0;
		}
	}	
}

