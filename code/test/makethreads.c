/* makethreads.c 
 *    Test program un mecanisme d'entré-sortie minimal dans un thread
 */

#include "syscall.h"


void print (char c){
  PutChar (c); 
  ThreadExit();
}


int
main ()
{
volatile int tr = 1;

 while (tr==1){
  ThreadCreate( print,'d');
  ThreadExit();
  tr=0;
 }
}


