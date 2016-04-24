#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#define MAX_BUFFERS 5000

typedef struct size_aware_buffers_info {
        void *ptr;
        size_t size;
        
}sabi;

sabi global_array[MAX_BUFFERS];
int count=0;



void *malloc(size_t size)
{   
    static void* (*real_malloc)(size_t) = NULL;
    if (real_malloc == NULL) {
        real_malloc = dlsym(RTLD_NEXT, "malloc");
        global_array[count].ptr = real_malloc;
        global_array[count].size = size;
        printf("Address: %p, Size: %u\n", global_array[count].ptr, global_array[count].size);
        count ++;

    }
    void *p = real_malloc(size);
    global_array[count].ptr = p;
    global_array[count].size = size;
    if((int)size == 60)
        printf("Address: %p, Size: %u\n", global_array[count].ptr, global_array[count].size);
    count ++;
    return p;
}
