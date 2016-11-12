#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"
#include <stdint.h>

struct  funcArg{ int function; int args;};
int countThread = 0;
//static Semaphore *readAvail;
//static Semaphore *writeDone;

//----------------------------------------------------------------------
// StartUserThread
//      Run a user thread.  
//----------------------------------------------------------------------
static void StartUserThread(void *schmurtz)
{
    DEBUG('s', "StartUserThread %d\n", &schmurtz);

    funcArg *vars = (struct funcArg *) (schmurtz);
    int f = vars->function;
    int arg = vars->args;
    for (int i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);
    int top = currentThread->space->AllocateUserStack();

    // Initial program counter -- must be location of "function"
    machine->WriteRegister (PCReg, f);
    DEBUG ('s', "[DEBUG] Function: %d\n", f);

    machine->WriteRegister (4, arg);
    DEBUG ('s', "[DEBUG] Arg: %d\n", arg);

    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);
    DEBUG ('s', "[DEBUG] NextPCReg: %d\n", machine->ReadRegister(PCReg) + 4);

    machine->WriteRegister (StackReg, top);
    DEBUG ('s', "[DEBUG] Adress: %d\n", top);
    
    machine->Run();// Run an user program
    free (schmurtz);

    DEBUG ('s', "Initializing User stack register to 0x%x\n", top);
}


int do_ThreadCreate (int f, int arg){
 DEBUG ('t', "Entering do_ThreadCreate, UserThread %x\n",f,arg);

 funcArg *myThread = (funcArg*)malloc(sizeof(funcArg)); 

 myThread->function = f;
 myThread->args =  arg;

 Thread *t = new Thread ("NouveauUserThread");  
 t->Start (StartUserThread, myThread);  
 
 //StartUserThread(myThread);
 
 countThread++;
 return 0;
};


void do_ThreadExit (){
 DEBUG ('t', "Entering do_ThreadExit\n");
  countThread--;

 if (countThread <0){
  interrupt->Halt();
 }
 
 currentThread->Finish(); 

}


#endif //CHANGED


