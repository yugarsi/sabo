#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define _GNU_SOURCE
#include <dlfcn.h>

void *global_array[5000][3];
int count=0;

void *malloc(size_t size)
{
   
    static void* (*real_malloc)(size_t) = NULL;
    if (!real_malloc){
        
        real_malloc = dlsym(RTLD_NEXT, "malloc");
        global_array[count][0] = real_malloc;
        global_array[count][1] = size;
        global_array[count][2] = 4;
        printf(stdout, "%s %p", "I am", global_array[count][0]);
        printf(stdout, "%s %d \n", "Count:", count);
        count ++;

    }
    void *p = real_malloc(size);
    return p;
}