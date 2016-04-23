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

static void* (*real_malloc)(size_t) = NULL;

void *malloc(size_t size)
{
    if (real_malloc == NULL) {
        real_malloc = dlsym(RTLD_NEXT, "malloc");
        global_array[count].ptr = real_malloc(size);
        global_array[count].size = size;
        printf("Address: %p, Size: %u\n", global_array[count].ptr, global_array[count].size);
        count ++;

    }
    return global_array[count].ptr;
}
