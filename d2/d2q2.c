/*------------------------------------------------------------
Fichier: d2q2.c

Nom: Hiba Tantawi
Numero d'etudiant: 300250911

Description: 
    Réponse du question 2.
    TO RUN THE PROJECT please use the following command lines:
    $ gcc d2q2.c -pthread
    $ ./a.out

    This program generates the Fibonacci sequence. The user enters
    how many numbers he wants to generate and then the parent thread
    creates a child thread that generate the numbers. Finally the 
    numbers are printed by the parent thread.
------------------------------------------------------------*/
/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Variables */
#define MAX_ARRAY_LENGTH 100

/* Shared memory */
int n, array[MAX_ARRAY_LENGTH];

/* Prototype */
void *childThread(void *param);

/*------------------------------------------------------------
Function: main
Arguments: 
	None
Description: This method prints a message and waits for the user
             to enter a number. Then it creates a child thread
             and waits for it to finish. After that it prints 
             the values saved in the array by the created
             child thread. 
------------------------------------------------------------*/
int main() {//parent thread

    printf("How many Fibonacci numbers would you like to print?: ");

    if (scanf("%d", &n) != 1) {
        printf("Invalid input. Please enter a valid number.\n");
        exit(EXIT_FAILURE);
    }

    else if (n < 0) {
        printf("The number should be higher than or equal to 0.\n");
        exit(EXIT_FAILURE);
    } 
    else if (n>=0){
        pthread_t tid;
        pthread_attr_t attr;
        /*get the default attributes*/
        pthread_attr_init(&attr);

        /* create the thread */
        pthread_create(&tid, &attr, childThread, NULL);

        /* now wait for the thread to exit */
        pthread_join(tid, NULL);

        //start printing the output
        printf("\n\t n\t\t Fibonacci Numbers\n");
        printf("\n\t---\t\t -----------------\n");

        for (int i=0; i<n; i++)
        {
	        printf("\t %d\t\t %d\n", i+1, array[i]);

        }
        printf("The parent thread finished printing the results\n");
    }
    else {
        printf("Invalid arguments\n");
    }
    return 0;
}

/*-------------------------------------------------------------
Function: *childThread
Arguments: 
	void param - NULL
Description: This is the created child thread by the parent thread
             (in the main method). It saves 2 values in the shared
             array and then starts a for loop that find the Fibonacci
             numbers and saves it in the array. After finishing storing 
             the numbers, the thread calls pthread_exit and it got
             terminated.
-------------------------------------------------------------*/
void *childThread(void *param) {//child thread
    array[0] = 0;
    array[1] = 1;

    for (int i = 2; i < n; i++) {
        array[i] = array[i - 1] + array[i - 2];
    }

    pthread_exit(NULL);
}
