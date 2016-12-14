/* fork.c 
 *    Test de l'appelle systeme ForkExec
 */
#ifdef CHANGED
#include "syscall.h"

int main ()
{
//ThreadCreate( ForkExec,"../test/putchar");
ForkExec("../test/putchar");
ForkExec("../test/putchar");
//while(1);
}
#endif //CHANGED

