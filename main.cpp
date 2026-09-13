/*
 * Stopky.cpp
 *
 * Created: 03.04.2026 10:43:15
 * Author : rezacm
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

const uint8_t znaky[] = {
	0b00111111, //0
	0b00000110, //1
	0b01011011, //2
	0b01001111, //3
	0b01100110, //4
	0b01101101, //5
	0b01111101, //6
	0b00000111, //7
	0b01111111, //8
	0b01101111,	//9
	0b01000000, //-
	0b00000000 //nic
	
};
	
const uint8_t klavesy[4][4] = {
	{0,1,2,3},
	{4,5,6,7},
	{8,9,10,11},
	{12,13,14,15}
};

void zobraz_znak(uint8_t pozice, uint8_t znak);
uint8_t cti_klavesy();

uint32_t milis = 0;
uint8_t bezi = 0;
uint8_t pozice;
uint8_t hod = 0;
uint8_t min = 0;
uint8_t sec = 0;
uint8_t mezicas = 0;
uint8_t c_hod = 0;
uint8_t c_min = 0;
uint8_t c_sec = 0;
int main(void)
{
	DDRA = 0b11111111;
	DDRB = 0b00001111;
	DDRC = 0b00001111;
	
	TIMSK = 0b00000001;
	TCCR0 = 0b00000011;
	sei();
	
    while (1) 
    {
		
		uint8_t kl = cti_klavesy();
		if (kl < 20)
		{
			if (kl == 11)
			{
				if (bezi == 0)
				{
					bezi = 1;
					
				}
				else if (bezi == 1)
				{
					bezi = 0;
				}
			}
			if (kl == 12)
			{
				bezi = 1;
			}
			if (kl == 13)
			{
				bezi = 0;
				milis = 0;
			}
			if (kl == 14)
			{
				bezi = 2;
				mezicas = 1;
			}
			if (kl == 15)
			{
				mezicas = 0;
			}
		}
		while (cti_klavesy() != 99);
		_delay_ms(5);
		
		
	}
}

uint8_t cti_klavesy(){
	for (uint8_t x = 0; x < 4; x++)
	{
		PORTC = ~(0b00000001 << x);
		for (uint8_t y = 0; y < 4; y++)
		{
			if ((~(PINC)&(0b00010000 << y)) > 0)
			{
				return klavesy[x][y];
			}
		}
	}
	return 99;
}

void zobraz_znak(uint8_t pozice, uint8_t znak){
	PORTA = 0b11111111;
	PORTB = ~(0b00000001 << pozice);
	PORTA = ~znaky[znak];
}

ISR(TIMER0_OVF_vect){
	TCNT0 = 6;
	sec = (milis/1000)%60;
	min = (milis/60000)%60;
	hod = (milis/3600000)%24;
	
	
	if (bezi == 1)
	{
		milis++;
		if (mezicas == 0)
		{
			if(hod == 0){
				switch (pozice)
				{
					case 0:
						zobraz_znak(0, sec%10);	
					break;
					case 1:
						zobraz_znak(1, (sec/10)%10);
					break;
					case 2:
						zobraz_znak(2, min%10);
					break;
					case 3:
						zobraz_znak(3, (min/10)%10);
					break;
				}
			}
			if (hod > 0)
			{
				switch(pozice){
					case 0:
						zobraz_znak(0, min%10);
					break;
					case 1:
						zobraz_znak(1, (min/10)%10);
					break;
					case 2:
						zobraz_znak(2, hod%10);
					break;
					case 3:
						zobraz_znak(3, (hod/10)%10);
					break;
				}
			}
		}
		else if(mezicas == 1){
			if(hod == 0){
				switch (pozice)
				{
					case 0:
						zobraz_znak(0, c_sec%10);
					break;
					case 1:
						zobraz_znak(1, (c_sec/10)%10);
					break;
					case 2:
						zobraz_znak(2, c_min%10);
					break;
					case 3:
						zobraz_znak(3, (c_min/10)%10);
					break;
				}
			}
			if (hod > 0)
			{
				switch (pozice)
				{
					case 0:
						zobraz_znak(0, c_min%10);
					break;
					case 1:
						zobraz_znak(1, (c_min/10)%10);
					break;
					case 2:
						zobraz_znak(2, c_hod%10);
					break;
					case 3:
						zobraz_znak(3, (c_hod/10)%10);
					break;
				}
			}
		}
	}
	if (bezi == 0)
	{
		switch (pozice)
		{
			case 0:
			zobraz_znak(0, sec%10);
			break;
			case 1:
			zobraz_znak(1, (sec/10)%10);
			break;
			case 2:
			zobraz_znak(2, min%10);
			break;
			case 3:
			zobraz_znak(3, (min/10)%10);
			break;
		}
	}
	if (bezi == 2)
	{ 
		c_sec = sec;
		c_min = min;
		mezicas = 1;
		bezi = 1;
	}
	pozice++;
	if (pozice > 3)
	{
		pozice = 0;
	}
}

