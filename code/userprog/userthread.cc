#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"
#include <stdint.h>

struct  funcArg{ int function; int args;};
unsigned countThread = 0;
unsigned numThread = 0;
int cntSUT = 0;
static Semaphore *SemThread = new Semaphore("SemThread", 1);


//----------------------------------------------------------------------
// StartUserThread
//      Run a user thread.  
//----------------------------------------------------------------------
static void StartUserThread(void *schmurtz)
{

    cntSUT ++;
    funcArg *vars = (struct funcArg *) (schmurtz);
	
	DEBUG('s', " Entering to StartUserThread \n");
	
    int f = vars->function;
    int arg = vars->args;
	
    for (int i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);
    
	int topAdress = currentThread->space->AllocateUserStack(cntSUT);
    
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
 t->Start (StartUserThread, myThread);  
 
 SemThread->P ();
 countThread++;
 SemThread->V ();
 
 return 0;
};

//----------------------------------------------------------------------
// do_ThreadExit
//      Finir an user thread.  
//----------------------------------------------------------------------

void do_ThreadExit (){
 DEBUG ('t', "Entering do_ThreadExit\n");
 
 SemThread->P ();
 countThread--;
 SemThread->V ();
  

 if (countThread <= 0){

  interrupt->Halt();
 }else {
 currentThread->space->~AddrSpace ();
 currentThread->Finish(); 
 }


}


#endif //CHANGED


