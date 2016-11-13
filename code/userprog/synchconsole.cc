#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"


static Semaphore *readAvail;
static Semaphore *writeDone;
static Semaphore *mutexWrite;
static Semaphore *mutexRead;

static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }

SynchConsole::SynchConsole(const char *in, const char *out)
{
readAvail = new Semaphore("read avail", 0);
writeDone = new Semaphore("write done", 0);
mutexWrite = new Semaphore("mutexWrite", 1);
mutexRead = new Semaphore("mutexRead", 1);
console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, 0);
}

SynchConsole::~SynchConsole()
{
delete console;
delete writeDone;
delete readAvail;
delete mutexWrite;
delete mutexRead;
}
void SynchConsole::SynchPutChar(int ch)
{
  /*Il faut protéger les fonctions noyau correspondantes par un verrou s'il y a 2 ou plus threads qui l'appelle*/	
  mutexWrite->P ();	
  console->PutChar(ch);
  writeDone->P ();
  mutexWrite->V ();	
}

int SynchConsole::SynchGetChar()
{
  char ch;
  mutexRead->P ();	
  readAvail->P ();
  ch = console->GetChar ();
  return ch;
  mutexRead->V ();	

}
 

void SynchConsole::SynchPutString(const char s[])
{

char c = s [0];
  for( int i = 0; i<MAX_STRING_SIZE-1 && c != '\0'; i++){
   synchconsole->SynchPutChar((int)s[i]);	
   c = s[i];
  }
   writeDone->P ();   
}

void SynchConsole::SynchGetString(char *s, int n)
{
    char caracter;
	int i;
        for(i=0;i<n-1;i++) {
                caracter=synchconsole->SynchGetChar();
                if(caracter=='\0') break;
                else s[i] = caracter;
        }
	writeDone->P();
        s[i] = '\0';
}


#endif // CHANGED
