// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "userthread.h"
#include "fork.h"

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);

    switch (which)
      {
	case SyscallException:
          {
	    switch (type)
	      {
		case SC_Halt:
		  {
		    DEBUG ('s', "Shutdown, initiated by user program.\n");
		    interrupt->Halt ();
		    break;
		  }
		#ifdef CHANGED
		//Ici on define les differents appelles au systeme, il faut les ajoute dans le langue assambleur
		case SC_Exit:
		  {
		    DEBUG ('s', "Exit end of user function: ");
		    int returnValue = machine->ReadRegister (4);
		    interrupt->Halt ();
		    break;
		  }
		case SC_PutChar:
		  {
		    DEBUG ('s', "Synchconsole, Putchar: ");
		    char addr = machine->ReadRegister (4);
		    synchconsole->SynchPutChar (addr);
		    break;
		  }
		case SC_PutString:
		  {
		    DEBUG ('s', "Synchconsole, PutString: \n");
            	int addr = machine->ReadRegister (4); //read addrese from noyeu
	        char putString[MAX_STRING_SIZE];		    
		    copyStringFromMachine (addr,  putString, MAX_STRING_SIZE );
		    synchconsole->SynchPutString (putString);
		    break;
		  }
		case SC_GetChar:
		  {
		    DEBUG ('s', "Synchconsole, GetChar.\n");
		     machine->WriteRegister(2, (int) synchconsole->SynchGetChar());// On garde la valeur dans le registre 2 
		    break;
		  }
		case SC_GetString:
		  {
		    DEBUG ('s', "Synchconsole, GetString.\n");

			int to = machine->ReadRegister(4);
			int taille  = machine->ReadRegister(5);
			char stringCaractere[taille];	
			synchconsole->SynchGetString( stringCaractere, taille);
			int val = 0;
			while(val != -1 && val < taille)
			val = copyStringToMachine(to+val,stringCaractere,MAX_STRING_SIZE);
			
		    break;
		  }
		
		case SC_ThreadExit:
		  {
		    DEBUG ('s', "Entering to ThreadExit function, Exception.\n");
		    do_ThreadExit();
		    break;
		  }
		  
		case SC_ThreadCreate:
		  {
		  DEBUG ('s', "Entering to ThreadCreate function, Exception.\n");	
		  	int funct = machine->ReadRegister(4);	  // appelle systeme recupere la function y el parametre
		  	int args  = machine->ReadRegister(5);		
		  	do_ThreadCreate (funct,args);
		  break;
		  }

		case SC_ForkExec:
		  {
		  DEBUG ('s', "Entering to ForkExec function, Exception.\n");
		
		  int from;
		  char args[MAX_STRING_SIZE];
		  
		  from  = machine->ReadRegister(4);	
		  copyStringFromMachine (from,  args, MAX_STRING_SIZE );
		  //ThreadCreate(ForkExec,args);
		  ForkExec (args);

		  break;
		  }


		#endif
		default:
		  {
		    printf("Unimplemented system call %d\n", type);
		    ASSERT(FALSE);
		  }
	      }

	    // Do not forget to increment the pc before returning!
	    UpdatePC ();
	    break;
	  }



	case PageFaultException:
	  if (!type) {
	    printf("NULL dereference at PC %x!\n", machine->registers[PCReg]);
	    ASSERT (FALSE);
	  } else {
	    printf ("Page Fault at address %x at PC %x\n", type, machine->registers[PCReg]);
	    ASSERT (FALSE);	// For now
	  }

	default:
	  printf ("Unexpected user mode exception %d %d at PC %x\n", which, type, machine->registers[PCReg]);
	  ASSERT (FALSE);
      }
}

