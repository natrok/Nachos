/* makethreads.c 
 *    Test program un mecanisme d'entré-sortie minimal dans un thread
 */
#ifdef CHANGED
#include "syscall.h"


void print (char c){
  PutChar (c); 
  PutChar ('\n');
  ThreadExit();
}


int
main ()
{
volatile int tr = 1;
volatile int j;

/*//Test UsetThread
 while (tr==1){
  ThreadCreate( print,'d');
  ThreadCreate( print,'i');
  ThreadCreate( print,'e');
  ThreadCreate( print,'g');
  ThreadCreate( print,'o');
		  
  tr=0;
 }
 PutChar ('a'); 
 ThreadExit();*/
 //UsetThread
 
//Test Verrou
/*
 while (tr==1){
  ThreadCreate( print,'d');
  tr=0;
 }
 PutChar ('a'); 
 ThreadExit();
*/
//Verrou


//Test multithreads
while (tr==1){
 for( j=0;j<5;j++){
	ThreadCreate( print,'d');	 
 }
 tr=0;
}
ThreadExit();
 
}
#endif //CHANGED

