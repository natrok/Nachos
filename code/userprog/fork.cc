// fork.cc 
//      Test routines for demonstrating that Nachos can load
//      a user program and execute it.  


#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "synch.h"
#include "fork.h"

#ifdef CHANGED
//----------------------------------------------------------------------
// StartProcess
//      Run a user program.  Open the executable, load it into
//      memory, and jump to it.
//----------------------------------------------------------------------

int ForkExec (const char *s)
{
    OpenFile *executable = fileSystem->Open (s);
    AddrSpace *NewSpace;

    if (executable == NULL)
      {
	  printf ("Unable to open file %s\n", s);
	  return -1;
      }
    NewSpace = new AddrSpace (executable);



    currentThread->space = NewSpace;

    delete executable;		// close file

    NewSpace->InitRegisters ();	// set the initial register values
    NewSpace->RestoreState ();	// load page table register

    machine->Run ();		// jump to the user progam
    ASSERT (FALSE);		// machine->Run never returns;
    return 0;
}

#endif //CHANGED


