/* putchar.c 
 *    Test program un mecanisme d'entré-sortie minimal 
 */

#include "syscall.h"

void print (char c, int n){
int i;

#if 1
 for (i=0; i< n; i++){
  PutChar (c+i); 
 }
 PutChar ('\n'); 
#endif
}


int
main ()
{
 print('a',6);
 return 0;
}


