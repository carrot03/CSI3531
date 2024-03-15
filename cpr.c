/*------------------------------------------------------------
Fichier: cpr.c

Nom: Hiba Tantawi
Numero d'etudiant: 300250911

Description: Ce programme contient le code pour la creation
             d'un processus enfant et y attacher un tuyau.
	     L'enfant envoyera des messages par le tuyau
	     qui seront ensuite envoyes a la sortie standard.

Explication du processus zombie
(point 5 de "A completer" dans le devoir):

I was detecting the zombie process in my code using the command <ps -u usename> in the "/proc" folder, where I saw <defunct> attached to the process.
Apparently only the child process can be a zombie, because it is the parent's role to kill the zombie using the wait function. 
The zombie is a process that finished executing but waiting for his parent to kill it.

So for example, in my program when the last child printed both of its messages, it entered the zombie state due to the use of the sleep function and it stays like this till the parent wakes up and calls wait. After that this child dies and its parent becomes a zombie and so on till we reach the very first child created to become a zombie and then killed by its parent and after that we reach the end of the program.
Also, it is crucial for the parent process to handle the collection of exit status using the appropriate wait functions to prevent zombie processes from accumulating and consuming system resources.

Here is an example:
  111 pts/1    00:00:00 bash
  469 pts/0    00:00:00 bash
  573 pts/2    00:00:00 bash
  609 pts/0    00:00:00 a.out
  610 pts/0    00:00:00 a.out <defunct>
  633 pts/2    00:00:00 ps
-------------------------------------------------------------*/
#include <stdio.h>
#include <sys/select.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Prototype */
void creerEnfantEtLire(int );

/*-------------------------------------------------------------
Function: main
Arguments: 
	int ac	- nombre d'arguments de la commande
	char **av - tableau de pointeurs aux arguments de commande
Description:
	Extrait le nombre de processus a creer de la ligne de
	commande. Si une erreur a lieu, le processus termine.
	Appel creerEnfantEtLire pour creer un enfant, et lire
	les donnees de l'enfant.
	
	TO RUN THE CODE PROPERLY, PLEASE USE THE FOLLOWING COMMAND LINES: 
	"gcc cpr.c" TO COMPILE THE PROJECT AND THEN WRITE "./a.out <prcNum>", 	
	YOU CAN REPLACE THE prcNum with the numer of processes you wish to create.
-------------------------------------------------------------*/

int main(int ac, char **av)
{
    
    int numeroProcessus; 
    
    if(ac == 2)	
    {  
       if(sscanf(av[1],"%d",&numeroProcessus) == 1)
       {
           creerEnfantEtLire(numeroProcessus);
       }
       else fprintf(stderr,"Ne peut pas traduire argument\n");
    }
    else fprintf(stderr,"Arguments pas valide\n");
    return(0);
}


/*-------------------------------------------------------------
Function: creerEnfantEtLire
Arguments: 
	int prcNum - le numero de processus
Description:
	Cree l'enfant, en y passant prcNum-1. Utilise prcNum
	comme identificateur de ce processus. Aussi, lit les
	messages du bout de lecture du tuyau et l'envoie a 
	la sortie standard (df 1). Lorsqu'aucune donnee peut
	etre lue du tuyau, termine.
-------------------------------------------------------------*/


void creerEnfantEtLire(int prcNum)
{
    int pipefd[2]; // File descriptors for the pipe

    if (prcNum == 1) {// in case the input num provided by the user is = 1
				  // not executed if prcNum>1
        fprintf(stderr, "Processus 1 commence \n");
        sleep(5);
        fprintf(stderr, "Processus 1 termine \n");
        exit(0);
    }

    if (pipe(pipefd) == -1) {// pipe failed
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork(); //first child
    if (pid < 0) {//Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        close(pipefd[0]); // Close the read end of the pipe

        dup2(pipefd[1], STDOUT_FILENO); // Redirect standard output to the pipe

        char numArg[10];
        sprintf(numArg, "%d", prcNum - 1); //saving in numArg the number prcNum-1 as a char
        execlp("./a.out", "a.out", numArg, NULL); // Create child process with the new prcNum value as an argument in the file "a.out"

        perror("execlp"); // This line will be reached only if execlp fails
        exit(EXIT_FAILURE);
    } else {
        // Parent process
	 // used to print all of the info to the command line
        close(pipefd[1]); // Close the write end of the pipe

        char buffer[1024];
        ssize_t bytesRead;

        fprintf(stderr, "Processus %d commence \n", prcNum);// printed sequentially
		
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytesRead); // Write the read data to standard output
        }
	  //sleep(10);
        fprintf(stderr, "Processus %d termine \n", prcNum);
        wait(NULL); // Wait for the child process to terminate
        exit(0);
    }
}
