#ifdef CHANGED
#include "syscall.h" 
int main() { 
    
	char buffer[] =  "Voici une phrase";
	char buffer2[] =  "Voici une phraseeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
	char buffer3[] =  "Il y a deux sortes d'amour: l'amour insatisfait, qui vous rend odieux, et l'amour satisfait, qui vous rend Idiot.";
	
	PutString(buffer3);	
	//PutString(buffer2);
	//PutString(buffer3);
	//si on n'ajoute pas Halt le systeme continue dans un exception et apres aura un erreur
	Halt();
}

#endif
