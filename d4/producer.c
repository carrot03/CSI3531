/*-----------------------------------------------------------------
File: producer.c

Name: Hiba Tantawi
Student Number: 300250911

Description: Answer to Question 3 of Assignment 4.
	     This project creates a shared memory segmement and 
         saves in it a shared memory segment. That is going to be
         read by consumer.c program.
-----------------------------------------------------------------*/
/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/*-----------------------------------------------------------------
Function: catalan
Arguments:
    int n  - number of Catalan numbers we want to execute.
Description: This function use the Catalan formula to generate
        a Catalan number from the list.
-----------------------------------------------------------------*/
unsigned long long catalan(int n) {
    if (n <= 1) {
        return 1;
    }

    unsigned long long result = 0;
    for (int i = 0; i < n; i++) {
        result += catalan(i) * catalan(n - i - 1);
    }

    return result;
}

/*-----------------------------------------------------------------
Function: main
Arguments:
    int argc	- nombre d'arguments de la commande -
                  name of the output file.
	char **argv - tableau de pointeurs aux arguments de commande - 
                  number of Catalan numbers we want to generate.
Description: This function creates the list of Catalan numbers
        and saves it in a shared memory segment. After creating the
        segment, we detach it.
-----------------------------------------------------------------*/
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);

    // Create a shared memory segment
    int segment_id = shmget(IPC_PRIVATE, sizeof(unsigned long long) * n, IPC_CREAT | 0666);
    if (segment_id == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment
    unsigned long long *shm_ptr = (unsigned long long *)shmat(segment_id, (void *)0, 0);
    if ((void *)shm_ptr == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    // Generate the Catalan numbers and write them to shared memory
    for (int i = 0; i < n; i++) {
        shm_ptr[i] = catalan(i + 1);
    }

    printf("Producer: Catalan sequence generated and written to shared memory.\n");
    printf("Shared memory segment ID: %d\n", segment_id); // Print the segment ID

    // Detach the shared memory segment
    if (shmdt((void *)shm_ptr) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
