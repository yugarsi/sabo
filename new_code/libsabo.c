#define _GNU_SOURCE

#include <stdio.h>
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
        fprintf(stderr, "%s %p", "I am", global_array[count][0]);
        fprintf(stderr, "%s %d \n", "Count:", count);
        count ++;

    }
    void *p = real_malloc(size);
    return p;
}
