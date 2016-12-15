#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"
#include <stdint.h>

struct  funcArg{ int function; int args;};
static Semaphore *mutexThread = new Semaphore("mutexThread", 1);


//----------------------------------------------------------------------
// StartUserThread
//      Run a user thread.  
//----------------------------------------------------------------------
static void StartUserThread(void *schmurtz)
{

    funcArg *vars = (struct funcArg *) (schmurtz);
	
	DEBUG('s', " Entering to StartUserThread \n");
	
    int f = vars->function;
    int arg = vars->args;
	
    for (int i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);
    
	int topAdress = currentThread->space->AllocateUserStack(numUserThreads);
    
	DEBUG ('s', "Initializing User stack register to 0x%x\n", topAdress);
	
    // Initial program counter -- must be location of "function"
    machine->WriteRegister (PCReg, f);
    DEBUG ('s', "[DEBUG] Function: %d\n", f);

	machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);
    DEBUG ('s', "[DEBUG] NextPCReg: %d\n", machine->ReadRegister(PCReg) + 4);
	
    machine->WriteRegister (4, arg);
    DEBUG ('s', "[DEBUG] Arg: %d\n", arg);

    machine->WriteRegister (StackReg, topAdress);
    DEBUG ('s', "[DEBUG] Adress: %d\n", topAdress);
    
    machine->Run();// Run an user program	
    free (vars);
	free (schmurtz);
}

//----------------------------------------------------------------------
// do_ThreadCreate
//      Create a user thread.  
//----------------------------------------------------------------------

int do_ThreadCreate (int f, int arg){
 DEBUG ('t', "Entering do_ThreadCreate, UserThread %x\n",f,arg);
 funcArg *myThread = (funcArg*)malloc(sizeof(funcArg)); 

 myThread->function = f;
 myThread->args =  arg;

 Thread *t = new Thread ("NouveauUserThread");  
 
 mutexThread->P ();
 numUserThreads++;
 mutexThread->V ();
 
 t->Start (StartUserThread, myThread);  
 StartUserThread(myThread);
 

 
 return 0;
};

//----------------------------------------------------------------------
// do_ThreadExit
//      Finir an user thread.  
//----------------------------------------------------------------------

void do_ThreadExit (){
 DEBUG ('t', "Entering do_ThreadExit\n");
 
 mutexThread->P ();
 numUserThreads--;
 mutexThread->V ();
  
 if (numUserThreads <= 0){
    interrupt->Halt();
 }else {	 
   currentThread->Finish(); 
   currentThread->space->~AddrSpace();
   delete currentThread->space;
 }
}


#endif //CHANGED


