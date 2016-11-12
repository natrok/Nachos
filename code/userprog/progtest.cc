// progtest.cc 
//      Test routines for demonstrating that Nachos can load
//      a user program and execute it.  
//
//      Also, routines for testing the Console hardware device.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "console.h"
#include "synchconsole.h"
#include "addrspace.h"
#include "synch.h"

char caracteres[1024];
//----------------------------------------------------------------------
// StartProcess
//      Run a user program.  Open the executable, load it into
//      memory, and jump to it.
//----------------------------------------------------------------------

void
StartProcess (char *filename)
{
    OpenFile *executable = fileSystem->Open (filename);
    AddrSpace *space;

    if (executable == NULL)
      {
	  printf ("Unable to open file %s\n", filename);
	  return;
      }
    space = new AddrSpace (executable);
    currentThread->space = space;

    delete executable;		// close file

    space->InitRegisters ();	// set the initial register values
    space->RestoreState ();	// load page table register

    machine->Run ();		// jump to the user progam
    ASSERT (FALSE);		// machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
}

// Data structures needed for the console test.  Threads making
// I/O requests wait on a Semaphore to delay until the I/O completes.

static Console *console;
static Semaphore *readAvail;
static Semaphore *writeDone;
static Semaphore *mutex;
static Semaphore *sleep;

//----------------------------------------------------------------------
// ConsoleInterruptHandlers
//      Wake up the thread that requested the I/O.
//----------------------------------------------------------------------

static void
ReadAvailHandler (void *arg)
{
    (void) arg;
    readAvail->V ();
}
static void
WriteDoneHandler (void *arg)
{
    (void) arg;
    writeDone->V ();
}


//----------------------------------------------------------------------
// ConsoleTest
//      Test the console by echoing characters typed at the input onto
//      the output.  Stop when the user types a 'q'.
//----------------------------------------------------------------------
#ifdef CHANGED
void
ConsoleTest (const char *in, const char *out)
{
    char ch;

    readAvail = new Semaphore ("read avail", 0);
    writeDone = new Semaphore ("write done", 0);
    mutex = new Semaphore ("read avail",1);
    sleep = new Semaphore ("write done",0);
    console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, 0);

    for (;;)
      {


	  
	  readAvail->P ();	// wait for character to arrive
	  ch = console->GetChar ();
          
	  console->PutChar('<');
	  writeDone->P ();

	  console->PutChar(ch);	// echo it!
	  writeDone->P ();	// wait for write to finish

	  console->PutChar('>');
	  writeDone->P ();


	  if (ch == 'q') {
	      //printf ("Nothing more\n");
	      printf ("Au revoir!\n");
	      break;		// if q, quit
	  }
      }
    delete console;
    delete readAvail;
    delete writeDone;
}


void
SynchConsoleTest (const char *in, const char *out)
{
 char ch;
 SynchConsole * test_synchconsole = new SynchConsole(in, out);

 while ((ch = test_synchconsole->SynchGetChar()) != EOF)
 {
  	if (ch == 'q') {
	      //printf ("Nothing more\n");
	      printf ("Au revoir!\n");
	      break;		// if q, quit
 	 }
  
 test_synchconsole->SynchPutChar(ch);
 test_synchconsole->SynchPutChar('<');
 test_synchconsole->SynchPutChar(ch);
 test_synchconsole->SynchPutChar('>');
 }
 fprintf(stderr, "EOF detected in SynchConsole!\n");

}

int copyStringFromMachine(int from, char *to, unsigned size) {   

	int caracter;
	int read = 0;
	//on utilise l'objet machine afin de lire le memoire et on garde dans to

	//while ((unsigned int)read < size-1 && caracter!= '\0' )
	
	for (read = 0; read < size -1;read++  )	
	{
		to[read] = (char)caracter;		
		//if (!machine ->ReadMem(from+read,1,&caracter))
		if (to[read]=='\0')
		break;	
	}

	to[read] = '\0';

	return read;	
}

int copyStringToMachine(int to, char *from, unsigned size) {   
	
	unsigned int i = 0;
	
	//on utilise l'objet machine afin d'ecrire sur la machine le parametre 1 c'est a dire que c'est un char
	while( machine-> WriteMem(to+i, 1, (int)from[i]) && i < size)
		i++; //recorro las variables

	if(from[i]== '\0')
		return -1; //C'est fini

	if(i == size && from[i]!= '\0')
		return i;

	sleep->P();
	return i;
}
#endif //CHANGED


