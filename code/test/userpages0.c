/* putchar.c 
 *    Test program un mecanisme d'entré-sortie minimal 
 */

#ifdef CHANGED
#include "syscall.h" 


int main() { 
    
	char buffer[] =  "Voici une phrase\n";
	PutString(buffer);	
	Halt();
}

#endif
