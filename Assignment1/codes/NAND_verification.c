
#include <stdio.h>

//Function for 2-input NAND gate
unsigned int NAND (unsigned int A, unsigned int B) {
    return !(A & B);    
}

int main()
{
    //Binary inputs
    unsigned int A=0x00, B=0x00, C=0x00, n; 
    
    //Given truth table (Minterm and Maxterms are indices corresponding to value '1' and '0' respectively)
    unsigned int Y[8]={1,0,1,0,1,1,0,0}; 
    
    unsigned int mask = 0x01;
    unsigned int nand_output, temp, t;
    printf("\nBinary_Input  Output NAND_Output\n");
    
    for (n = 0x00; n<0x08; n++)              
    {
        //Binary inputs
        A = n>>2;    
        B = n>>1;    
        C = n>>0;
        
        //verifying the expression obtained for 2-input nand gates only.
        temp = NAND( NAND( A, B ), NAND( NAND( A, A ), C ) );
        nand_output = NAND( temp, temp );
        
        //Printing the results.
        printf("    %x %x %x       %x      %x\n", mask&A, mask&B, mask&C, Y[n], nand_output);
    }
    return 0;
}
