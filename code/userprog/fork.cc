#ifdef CHANGED

// fork.cc 
//      Test routines for demonstrating that Nachos can load
//      a user program and execute it in a user thread.  

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "synch.h"
#include "fork.h"

struct  funcArg{ const char *file; AddrSpace *NewSpace;};
static  Semaphore *mutexThread = new Semaphore("SemThread", 1);

//----------------------------------------------------------------------
// StartProcess 
//      Run a user program.  Open the executable, load it into
//      memory, and jump to it.
//----------------------------------------------------------------------
//int numUserThreads = 0;
int ForkExec (const char *s)
{
	funcArg *myFile= (funcArg*)malloc(sizeof(funcArg)); 
	OpenFile *executable = fileSystem->Open (s);
    AddrSpace *NewSpace;	
	
	if (executable == NULL)
      {
	  printf ("Unable to open file %s\n", s);
	  return -1;
      }
    NewSpace = new AddrSpace (executable);
	
	delete executable;
	
	Thread *my = new Thread ("ForkUserThread");
	myFile->file = s;
    myFile->NewSpace = NewSpace;
	
    my->Start (startForkThread,myFile);
	startForkThread(myFile);
	
	return 0;
}

void startForkThread(void *arg) {
	DEBUG ('s', "Entering Start Fork Thread Noyeau\n");
	funcArg *vars = (struct funcArg *) (arg);
	
	mutexThread->P ();
	numUserThreads++;
	mutexThread->V ();
	
	currentThread->space = vars->NewSpace;
	vars->NewSpace->InitRegisters ();	// set the initial register values
    vars->NewSpace->RestoreState ();	// load page table register

	machine->Run ();	
	
	ASSERT (FALSE);		// machine->Run never returns;
	DEBUG ('s', "Entering Exit Fork Thread Noyeau\n");
}

#endif //CHANGED