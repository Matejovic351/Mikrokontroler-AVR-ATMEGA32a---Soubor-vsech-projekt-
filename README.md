# Mikrokontroler-AVR-ATMEGA32a---Soubor-v-ech-projekt-
Všechny hotové projekty na AVR ATMEGA32a psané v programovacím jazyku C++. Použitý software Microchip Studio. Konfigurace uvedena v README.


Konfigurace:

3 porty: k portC je připojena klasická klávesnice 
         ke spodnímu nibblu portuB je připojen LCD displeje
         a zbytek na portuA

Jedná se o krátký program Zámku s PINEM a PUKEM se základními funkcemi. Program pracuje s knihovnami <avr/io.h>, <avr/interrupt.h>, <util/delay.h>. 

Kód je napsán začátečníkem na procvičení základů programovacího jazyka C++. Kód je okomentovaný v potřebných částek kódu.


Jak program Spustit?

Potřebný hardware je USB na RS232. Komunikace PC-AVR probíhá přes komunikaci USART.

Součástí projektu je pouze zdrojový kód nikoli celý projekt. Pro spuštění je třeba založit projekt nový a přetáhnout zdrojový kód.
