//Turns LED on and off
#include <avr/io.h>
#include <util/delay.h>

 
int main (void)
{
  //Declaring the input ports	
  DDRD |= 0b11100011;		
  /* Arduino boards have a LED at PB5 */
  //set PB5, pin 13 of arduino as output
  DDRB    |= ((1 << DDB5));
  
  //Taking variables for input and output
  int input, a, b, c, kmap_output;

  while (1) {
    input = PIND;

    a = (input & 0b00000100)>>2;
    b = (input & 0b00001000)>>3;
    c = (input & 0b00010000)>>4;

    kmap_output = (a|| !c) && (!a || !b);

    if(kmap_output == 0) {
      //turn led off    
      PORTB = ((0 <<  PB5));
    }
    else {
      //turn led on
      PORTB = ((1 <<  PB5));
    }
  }
  /* . */
  return 0;
}
