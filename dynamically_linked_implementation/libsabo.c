#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#define MAX_BUFFERS 5000

#define DEBUG

typedef struct size_aware_buffers_info {
        void *ptr;
        size_t size;        
}sabi;

sabi ga[MAX_BUFFERS];
int count=0;

static void* (*real_malloc)(size_t) = NULL;

void *malloc(size_t size)
{
    // Create override for 'malloc' symbol only at start
    if( real_malloc == NULL ) {
        real_malloc = dlsym(RTLD_NEXT, "malloc");
    }
   
    // Create pointer, store in global table
    void *p = real_malloc(size);
    ga[count].ptr = p;
    ga[count].size = size;
#ifdef DEBUG
    printf("Address: %p, Size: %u\n", ga[count].ptr, ga[count].size);
#endif
    count++;
    return p;
}

int isWritable( void *p ) {
    int i;
    for( i=0; i<count; i++) {
        if ( ga[i].ptr <= p && p <= ga[i].ptr + ga[i].size ) {
            return (ga[i].ptr + ga[i].size) - p;
        }
    }
    return -1;
}

char* strcpy(char *dest, char *src) {
    int n = isWritable(dest);
    if ( n == -1 ) {
        return NULL;
    }
    
    int i;
    for (i = 0; i < n-1 && src[i] != '\0'; i++)
        dest[i] = src[i];
    for ( ; i < n ; i++)
        dest[i] = '\0';

    // This additional line addresses the vulnerability present in 'strncpy'
    dest[n-1] = '\0';
    
#ifdef DEBUG
    printf("%s\n",src);
    printf("%s\n",dest);
#endif

    return dest;
}
