/*-----------------------------------------------------------------
File: consumer.c

Name: Hiba Tantawi
Student Number: 300250911

Description: Answer to Question 3 of Assignment 4.
	     This project reads the segment created by producer program
         using its id. And then it reads the segment (numbers of the
         generated Catalan list).
-----------------------------------------------------------------*/
/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/* Variables */
#define SENTINEL_VALUE 0ULL

/*-----------------------------------------------------------------
Function: main
Arguments:
    int argc	- nombre d'arguments de la commande -
                  name of the output file.
	char **argv - tableau de pointeurs aux arguments de commande -
                  the id of the created shared memory segment.
Description: This function is where the program execution starts. 
	     It reads the Catalan list saved in a shared memory 
         segment created by producer.c, after reading and displaying
         its content, we detach it.
-----------------------------------------------------------------*/
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <segment_id>\n", argv[0]);
        exit(1);
    }

    int segment_id = atoi(argv[1]); // Retrieve the segment ID from command-line argument

    // Attach the shared memory segment
    unsigned long long *shm_ptr = (unsigned long long *)shmat(segment_id, (void *)0, 0);
    if ((void *)shm_ptr == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    printf("Consumer: Catalan sequence read from shared memory:\n");

    // Read and display the Catalan numbers from shared memory
    int i = 0;
    while (shm_ptr[i] != 0) {
        printf("%llu ", shm_ptr[i]);
        i++;
    }
    printf("\n");

    // Detach the shared memory segment
    if (shmdt((void *)shm_ptr) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
