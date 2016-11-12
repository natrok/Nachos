#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"


static Semaphore *readAvail;
static Semaphore *writeDone;
static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }

SynchConsole::SynchConsole(const char *in, const char *out)
{
readAvail = new Semaphore("read avail", 0);
writeDone = new Semaphore("write done", 0);
console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, 0);
}

SynchConsole::~SynchConsole()
{
delete console;
delete writeDone;
delete readAvail;
}
void SynchConsole::SynchPutChar(int ch)
{
  	
  console->PutChar(ch);
  writeDone->P ();
}

int SynchConsole::SynchGetChar()
{
  char ch;

  readAvail->P ();
  ch = console->GetChar ();
  return ch;

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
