/*-----------------------------------------------------------------
File: d4q2.c

Name: Hiba Tantawi
Student Number: 300250911

Description: Answer to Question 2 of Assignment 4.
             TO RUN THE PROJECT please use the following command lines:
    	     $ gcc d4q2.c
    	     $ ./a.out
	     This project generates random number of frames between
	     1 and 7. And then we find the number of page faults
	     using FIFO and LRU algorithms.
-----------------------------------------------------------------*/
/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Prototype */
int* generate_reference_string(int length, int num_pages);
int fifo(int* reference_string, int length, int num_frames);
int lru(int* reference_string, int length, int num_frames);

/*-----------------------------------------------------------------
Function: generate_reference_string
Arguments:
    length - length of the reference string to be generated
    num_pages - number of pages in the memory
Description: Function to generate a random reference string of pages
-----------------------------------------------------------------*/
int* generate_reference_string(int length, int num_pages) {
    int* reference_string = (int*)malloc(length * sizeof(int));

    for (int i = 0; i < length; i++) {
        reference_string[i] = rand() % num_pages;
    }

    return reference_string;
}

/*-----------------------------------------------------------------
Function: fifo
Arguments:
    reference_string - A pointer to an integer array that represents 
                       the reference string of page numbers
    length - It specifies the number of elements in the reference_string 
             array
    num_frames - It determines the capacity of the frame buffer for 
                 the FIFO algorithm
Description: This function implements the FIFO algorithm. It 
             maintains an array of frames and checks if a page is 
             already in the frames. If not, it replaces the oldest 
             page in the frames with the current page and increments 
             the page_faults count
-----------------------------------------------------------------*/
int fifo(int* reference_string, int length, int num_frames) {
    int* frames = (int*)malloc(num_frames * sizeof(int));
    int page_faults = 0;
    int frame_index = 0;

    for (int i = 0; i < length; i++) {
        int page = reference_string[i];
        int page_found = 0;

        // Check if page is already in frames
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                page_found = 1;
                break;
            }
        }

        if (!page_found) {
            frames[frame_index] = page;
            frame_index = (frame_index + 1) % num_frames;
            page_faults++;
        }
    }

    free(frames);
    return page_faults;
}

/*-----------------------------------------------------------------
Function: lru
Arguments:
    reference_string - A pointer to an integer array that represents 
                       the reference string of page numbers
    length - It specifies the number of elements in the reference_string 
             array
    num_frames - It determines the capacity of the frame buffer for 
                 the LRU algorithm
Description: This function implements the LRU (Least Recently Used) 
	     algorithm. It maintains two arrays: frames to store 
	     the page numbers in the frames and used to store the 
	     last usage time of each frame. It checks if a page 
	     is already in the frames and updates the corresponding 
	     entry in the used array. If not, it finds the least 
	     recently used page and replaces it with the current 
	     page, updating the used array accordingly. The 
	     page_faults count is incremented whenever a page is 
	     replaced.
-----------------------------------------------------------------*/
int lru(int* reference_string, int length, int num_frames) {
    int* frames = (int*)malloc(num_frames * sizeof(int));
    int* used = (int*)malloc(num_frames * sizeof(int));
    int page_faults = 0;

    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;
        used[i] = 0;
    }

    for (int i = 0; i < length; i++) {
        int page = reference_string[i];
        int page_found = 0;

        // Check if page is already in frames
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                page_found = 1;
                used[j] = 1;
                break;
            }
        }

        if (!page_found) {
            int min_used = used[0];
            int min_used_index = 0;

            // Find the least recently used page
            for (int j = 1; j < num_frames; j++) {
                if (used[j] < min_used) {
                    min_used = used[j];
                    min_used_index = j;
                }
            }

            frames[min_used_index] = page;
            used[min_used_index] = i + 1;
            page_faults++;
        }
    }

    free(frames);
    free(used);
    return page_faults;
}

/*-----------------------------------------------------------------
Function: main
Arguments:
    none
Description: This function is where the program execution starts. 
	     It seeds the random number generator and generates 
	     a random reference string using the 
	     generate_reference_string function.
-----------------------------------------------------------------*/
int main() {
    // Seed the random number generator
    srand(time(NULL));

    // Generate a random reference string
    int length = 100;
    int num_pages = 10;
    int* reference_string = generate_reference_string(length, num_pages);

    // Test the page replacement algorithms
    for (int num_frames = 1; num_frames <= 7; num_frames++) {
        int fifo_faults = fifo(reference_string, length, num_frames);
        int lru_faults = lru(reference_string, length, num_frames);

        printf("Number of frames: %d\n", num_frames);
        printf("Number of page faults (FIFO): %d\n", fifo_faults);
        printf("Number of page faults (LRU): %d\n", lru_faults);
        printf("-----------------------\n");
    }

    free(reference_string);
    return 0;
}
