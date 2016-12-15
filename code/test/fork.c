/* fork.c 
 *    Test de l'appelle systeme ForkExec
 */
#ifdef CHANGED
#include "syscall.h"

int main ()
{

ForkExec("../test/putchar");
ForkExec("../test/userpages1");
while(1);
}
#endif //CHANGED

