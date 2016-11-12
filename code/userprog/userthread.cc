#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "userthread.h"
#include "synch.h"
#include <stdint.h>

struct funcArg { int function; int args;};
int countThread = 0;
static Semaphore *readAvail;
static Semaphore *writeDone;

//----------------------------------------------------------------------
// StartUserThread
//      Run a user thread.  
//----------------------------------------------------------------------
static void StartUserThread(void *schmurtz)
{
    DEBUG('s', "mon debug StartUserThread %d\n", schmurtz);

    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);

    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    funcArg *vars = (funcArg *) (schmurtz);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, vars->function);//ADDRESS DU FUNCTION DU STRUCTURE
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack());
    machine->Run();// Run a user program

    DEBUG ('s', "Initializing User stack register to 0x%x\n", currentThread->space->AllocateUserStack());
}
/*     Allocate memory for the stack. The default stack size is StackSize (4096) 4-byte integers.
    Place a sentinel value at the top of the allocated stack. Whenever it switches to a new thread, the scheduler verifies that the sentinel value of the thread being switched out has not changed, as might happen if a thread overflows its stack during execution.
    Initialize the program counter PC to point to the routine ThreadRoot. Instead of beginning execution at the user-supplied routine, execution actually begins in routine ThreadRoot.  ThreadRoot does the following:
 Calls an initialization routine that simply enables interrupts.
 Calls the user-supplied function, passing it the supplied argument.
Calls thread::Finish(), to terminate the thread. 
Having thread execution begin in ThreadRoot rather than in the user-supplied routine makes it straightforward to terminate the thread when it finishes. The code for ThreadRoot is written in assembly language and is found in switch.s Note: ThreadRoot isn't run by the thread that calls Fork(). The newly created thread executes the instructions in ThreadRoot when it is scheduled and starts execution. */

int do_ThreadCreate (int f, int arg){
 DEBUG ('s', "Entering do_ThreadCreate, UserThread %x\n",f,arg);
 struct funcArg myThread;

 myThread.function = f;
 myThread.args =  arg;

 Thread *t = new Thread ("User Thread");  
 t->Start (StartUserThread, (void*) &myThread); //on doit passer un pointeur
 
 countThread++;
 return 0;
};


void do_ThreadExit (){

 DEBUG ('t', "Entering do_ThreadExit\n");
 if (countThread >0){
  countThread--;
 }else {
  interrupt->Halt();//du machine
 }



if (countThread==0)
 currentThread->Finish(); 
 interrupt->Halt();
 
}




#endif //CHANGED


