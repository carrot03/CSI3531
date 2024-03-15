/*--------------------------------------------------------------------
Fichier: d2q1.c

Nom: Hiba Tantawi
Numero d'etudiant: 300250911

Description: 
	Réponse du question 1.
	TO RUN THE PROJECT please use the following command lines:
	$ gcc d2q1.c -pthread
	$ ./a.out write_a_number

	This program generates the odd numbers of an interval. The user 
	enters a number that determines the right limit of the interval,
	the main method creates a thread (runner) and shares an array
	with it. Once the thread finishes generating and saving the 
	numbers in the array, the main method is going to print the 
	results.
--------------------------------------------------------------------*/

/* Libraries */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Shared memory */
int sum=5;

/* Prototype */
void *runner(void *param); // the thread 

/*--------------------------------------------------------------------
Function: main
Arguments: 
	int ac	- nombre d'arguments de la commande
	char **av - tableau de pointeurs aux arguments de commande
Description: This function reads the command passed by the user 
	     	 and then saves the second argument (the number).
	     	 It adds the number to an array whcich will be passed 
	    	 to the created thread (that runs the method runner). 
	    	 If an error occured, messages will be displayed on 
	    	 the command. Once the thread finishes storing the 
	    	 numbers in the array, the main method is going to
	    	 print the results.
--------------------------------------------------------------------*/
int main (int ac, char **av)
{
    
    int numeroDonne; 
    
    if(ac == 2)	
    {  
       if(sscanf(av[1],"%d",&numeroDonne) == 1)
       {

		int end = numeroDonne;
		int start = 0;
		int range = end-start+1;
		int count=(range/2)+(range%2);//found how many odd numbers I have = capacity of the array
		
		
           	pthread_t tid;
		pthread_attr_t attr;

		int array[count];

		array[0] = numeroDonne;



		/*get the default attributes*/
		pthread_attr_init(&attr);

		/* create the thread */
		pthread_create(&tid, &attr, runner, array);

		/* now wait for the thread to exit */
		pthread_join(tid, NULL);

		printf("The odd numbers between 0 and %d are: ", numeroDonne);
		
		
		if (numeroDonne%2==0){
			for (int i=1; i<count; i++){
				printf("%d, ",  array[i]);
			}
		}
		if (numeroDonne%2!=0){
			for (int i=1; i<count+1; i++){
				printf("%d, ",  array[i]);
			}
		}
		printf("\n");
       }

       else fprintf(stderr,"Ne peut pas traduire argument\n");
    }
    else fprintf(stderr,"Arguments pas valide\n");
    return(0);
}


/*--------------------------------------------------------------------
Function: *runner
Arguments: 
	void param - Array
Description: This is the created thread's function. It is going to 
		 	 find the odd numbers till the number provided by the
			 user. The number is found in the first number in the 
			 array, and saved in the variable num. After that, 
			 in a for loop the thread finds the odd numbers and saves
		 	 them in the array. After finishing storing the numbers,
			 the thread calls pthread_exit and it got terminated.
--------------------------------------------------------------------*/
void *runner (void * param){

	int* array = (int*)param;
	int num = array[0];
	int pos = 1;
	int limit = num;
	
	
	for (int i = 1; i <= num; i += 2) {
		array[pos]=i;
		pos++;
//printf("limit = %d, num= %d", limit, num);
		
    	}

	pthread_exit(NULL);
}