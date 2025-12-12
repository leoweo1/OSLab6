#define _GNU_SOURCE
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>

#include "util.h"
#include "process.h"

/**
 * Returns an array of process that are parsed from
 * the input file descriptor passed as argument
 * CAUTION: You need to free up the space that is allocated
 * by this function
 */
ProcessType *parse_file(FILE * f, int *P_SIZE)
{
    int i = 0;
    int capacity = 10;
    ProcessType *pptr = (ProcessType *)malloc(capacity * sizeof(ProcessType));
    
    if (!pptr) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    // Read all the data
    while (!feof(f)) {
        if (i >= capacity) {
            capacity *= 2;
            ProcessType *temp = realloc(pptr, capacity * sizeof(ProcessType));
            if (!temp) {
                free(pptr);
                fprintf(stderr, "Memory reallocation failed\n");
                exit(EXIT_FAILURE);
            }
            pptr = temp;
        }
        
        if (fscanf(f, "%d %d %d %d %d %d", 
                   &pptr[i].pid, &pptr[i].bt, &pptr[i].art, 
                   &pptr[i].wt, &pptr[i].tat, &pptr[i].pri) == 6) {
            i++;
        }
    }
    
    *P_SIZE = i;
    
    // Resize to exact number of processes
    ProcessType *exact_ptr = realloc(pptr, *P_SIZE * sizeof(ProcessType));
    if (exact_ptr) {
        return exact_ptr;
    } else {
        return pptr; // Return original if realloc fails
    }
}
