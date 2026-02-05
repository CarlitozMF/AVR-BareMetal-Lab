#define F_CPU 16000000UL 

// ------- Preambulo-------- //
#include <avr/io.h>                        /* Define pins, ports, etc */
#include <util/delay.h>                     /* Funciones de tiempo */


int main(void) {

  // -------- Inicio --------- //
  DDRB |= ((1<<PB0)|(1<<PB3)); //Aqui solo el PB0 y PB3 son salidas

  // ------ Event loop ------ //
  while (1) {

    PORTB |= ((1<<PB0)|(1<<PB3));          /* Enciendo el puerto*/
    _delay_ms(500);                                           /* delay*/

    PORTB &= ~((1<<PB0)|(1<<PB3));          /* Apago el puerto*/
    _delay_ms(500);                                           /* delay*/

  }                                                  /* fin del loop*/
  return 0;                            
}
