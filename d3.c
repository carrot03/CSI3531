/*------------------------------------------------------------
File: d3q1.c

Name: Hiba Tantawi
Student Number: 300250911

Description:
    Answer to Assignment 3.
    TO RUN THE PROJECT please use the following command lines:
    $ gcc d3.c -pthread
    $ ./a.out

    This program creates student and TA threads. The TA helps
    one student at a time, and up to 3 students can wait
    outside for the TA. If a student doesn't find a seat, they
    will continue programming and come back later.
------------------------------------------------------------*/

/* Libraries */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Variables */
#define NUM_CHAIRS 3
#define NUM_LOOPS 10
#define NUM_STUDENTS_MAX 100

/* Shared memory */
int NUM_STUDENTS;
pthread_mutex_t mutex;
sem_t students_sem, ta_sem;
int waiting_students = 0;
int ta_terminated = 0;
int student_ids[NUM_STUDENTS_MAX];

/* Prototype */
void* student_thread(void* arg);
void* ta_thread(void* arg);

/*------------------------------------------------------------
Function: student_thread
Arguments:
    arg - refers to the student id
Description: This thread takes care of the student. It enters
             a loop to make sure that the student is
             getting the TA's help in case they didn't find a seat.
             In this thread, we use pthread_mutex_lock/unlock
             to protect critical sections of code from concurrent
             execution by multiple threads. If the student doesn't
             find a seat, they let other students come, sleep,
             and come back later. While awake and waiting in the thread,
             the student does programming.
------------------------------------------------------------*/
void* student_thread(void* arg) {
    int student_id = *((int*)arg);

    while (1) {
        int programming_time = rand() % 3 + 1;  // Random programming time (between 1 and 3 seconds)

        printf("Student %d starts programming for %d seconds.\n", student_id, programming_time);
        sleep(programming_time);  // Student is programming

        pthread_mutex_lock(&mutex);

        if (waiting_students < NUM_CHAIRS) {
            waiting_students++;
            printf("Student %d is waiting for help. Total waiting students: %d.\n", student_id, waiting_students);
            pthread_mutex_unlock(&mutex);
            sem_wait(&students_sem);  // Student waits to be called by the TA
            printf("Student %d is getting help from the TA.\n", student_id);
            sem_post(&ta_sem);  // TA is available to help a student
            break;
        } else {
            printf("Student %d couldn't find a chair and will try again later.\n", student_id);
            pthread_mutex_unlock(&mutex);
            int retry_time = rand() % 2 + 1;  // Random retry time (between 1 and 2 seconds)
            printf("Student %d will retry in %d seconds.\n", student_id, retry_time);
            sleep(retry_time);
        }
    }

    pthread_exit(NULL);
}

/*------------------------------------------------------------
Function: ta_thread
Arguments:
    arg - not used
Description: This thread takes care of the TA's job. It enters
             a loop to make sure that all the waiting students
             get the TA's help. In this thread, we use
             pthread_mutex_lock/unlock to protect critical
             sections of code from concurrent execution by
             multiple threads. The TA sleeps if they have no students.
------------------------------------------------------------*/
void* ta_thread(void* arg) {
    int loop_count = 0;
    int student_id = -1; // Variable to store the student ID being helped

    while (loop_count < NUM_LOOPS || waiting_students > 0) {
        pthread_mutex_lock(&mutex);

        if (waiting_students > 0) {
            waiting_students--;
            student_id = student_ids[waiting_students]; // Get the student ID from the array
            printf("TA is helping student number %d in the waiting line. Remaining waiting students: %d.\n", student_id, waiting_students);
            sem_post(&students_sem);  // A student is called for help
            pthread_mutex_unlock(&mutex);
            sem_wait(&ta_sem);  // TA waits for the student to finish being helped
            printf("TA finished helping student %d.\n", student_id);
            student_id = -1; // Reset the student ID after helping
        } else if (waiting_students == 0 && loop_count < NUM_LOOPS) {
            pthread_mutex_unlock(&mutex);
            printf("TA is sleeping.\n");
            sleep(5);  // TA sleeps for 5 seconds
            loop_count++;
        } else {
            pthread_mutex_unlock(&mutex);
        }
    }

    printf("TA has finished helping students and is terminating.\n");
    ta_terminated = 1;

    pthread_exit(NULL);
}

/*------------------------------------------------------------
Function: main
Arguments:
    None
Description: This method prints a message and waits for the user
             to enter a number. Then it creates and terminates
             each student thread and the TA thread. It also creates
             and terminates the used semaphores.
------------------------------------------------------------*/
int main() {
    printf("Please provide the number of incoming students: ");
    if (scanf("%d", &NUM_STUDENTS) == 1 && NUM_STUDENTS > 0 && NUM_STUDENTS <= NUM_STUDENTS_MAX) {
        pthread_t ta, students[NUM_STUDENTS];

        pthread_mutex_init(&mutex, NULL);
        sem_init(&students_sem, 0, 0);
        sem_init(&ta_sem, 0, 1);

        pthread_create(&ta, NULL, ta_thread, NULL);

        // Creating the student threads
        for (int i = 0; i < NUM_STUDENTS; i++) {
            student_ids[i] = i + 1;
            pthread_create(&students[i], NULL, student_thread, &student_ids[i]);
        }

        // Waiting for the student threads to finish
        for (int i = 0; i < NUM_STUDENTS; i++) {
            pthread_join(students[i], NULL);
        }

        while (!ta_terminated) {
            sem_post(&ta_sem);  // Signal TA to wake up and terminate
            pthread_join(ta, NULL);
        }

        pthread_mutex_destroy(&mutex);
        sem_destroy(&students_sem);
        sem_destroy(&ta_sem);
    } else {
        fprintf(stderr, "Invalid input or maximum number of students exceeded.\n");
        return 1;
    }

    return 0;
}
