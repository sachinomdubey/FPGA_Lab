#include <Arduino.h>

#define A 2
#define B 3
#define C 4
#define NAND_OUTPUT 12

//variables
int a, b, c, kmap_output, NAND_output, temp;

//Function for 2input NAND gate
int NAND(int i1, int i2) {
    return !(i1 && i2);
}	


void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(NAND_OUTPUT, OUTPUT);
	pinMode(A, INPUT);
	pinMode(B, INPUT);
	pinMode(C,INPUT);
}

void loop() {
    a = digitalRead(A);
    b = digitalRead(B);
    c = digitalRead(C);

    kmap_output = (a|| !c) && (!a || !b);

    temp = NAND ( NAND( a,b), NAND ( NAND(a,a),c));
    NAND_output = NAND( temp,temp);

    if(kmap_output ==1)
	    digitalWrite(LED_BUILTIN,HIGH);
    else
	    digitalWrite(LED_BUILTIN,LOW);

    if(NAND_output == 1)
	    digitalWrite(NAND_OUTPUT, HIGH);
    else
	    digitalWrite(NAND_OUTPUT, LOW);
}
