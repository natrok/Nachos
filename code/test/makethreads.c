/* makethreads.c 
 *    Test program un mecanisme d'entré-sortie minimal dans un thread
 */

#include "syscall.h"

int
main ()
{
volatile int tr = 1;

 while (tr==1){
 ThreadCreate( (void *) PutChar,(void *)'d');
 tr = 0;
 ThreadExit();
// while(1); 
 } 


// Halt();
}


