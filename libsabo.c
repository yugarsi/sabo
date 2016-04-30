#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#define MAX_BUFFERS 5000

//#define DEBUG

typedef struct size_aware_buffers_info {
        void *ptr;
        size_t size;
}sabi;

sabi ga[MAX_BUFFERS];
int count=0;

static void* (*real_malloc)(size_t) = NULL;
static void* (*real_free)(void *) = NULL;
static void* (*real_realloc)(void *, size_t t) = NULL;

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

/******* UTILITY FUNCTIONS ********/

/*
** Writable memory checker function returns amount of space one can write to,
** else returns -1.  Used by overflow unsafe functions defined below.
*/
size_t isWritable( void *p ) {
    int i;
    for( i=0; i<count; i++) {
        if ( ga[i].ptr <= p && p <= ga[i].ptr + ga[i].size ) {
            return (ga[i].ptr + ga[i].size) - p;
        }
    }
    return -1;
}

/*
** If a given pointer needs to be free-d, this clears
** the entry for that pointer in out global table.  If this space is
** later used by other pointers, this will make sure that conflicts
** are handled.
** Returns 0 on success and -1 if no such pointer exists in our table.
*/
int clearEntryInTable( void *p ) {
    int i;
    for( i=0; i<count; i++) {
        if ( ga[i].ptr <= p && p <= ga[i].ptr + ga[i].size ) {
            ga[i].ptr = (void *)(0);
            ga[i].size = 0;
            return 0;
        }
    }
    return -1;
}

/*
** If a given pointer needs to be reallocated to a different size, this fucntion
** updates the global table.  For handling of edge case, see 'realloc' function
** defined later.
*/
int updateEntryInTable( void *p , size_t t) {
    int i;
    for( i=0; i<count; i++) {
        if ( ga[i].ptr <= p && p <= ga[i].ptr + ga[i].size ) {
            ga[i].size = t;
            return 0;
        }
    }
    return -1;
}

/*
** Snapshot of 'ga' table at the time of call.
*/
void print_ga() {
    printf("\n-- GLOBAL VARIABLE TABLE (start) --\n");
    int i;
    for( i=0; i<count; i++) {
        printf("| %p | %u |\n", ga[i].ptr, ga[i].size);
    }
    printf("-------------------------------------\n");
}

/******* OVERFLOW SAFE FUNCTIONS ********/

/* TODO: Might want to check if length 'n' is safely passed ** by the developer.  Might have to return a different value
** than 'dest' to inform of this issue?
*/
char* strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for ( ; i < n; i++)
        dest[i] = '\0';

    // This additional line addresses the vulnerability present in 'strncpy'
    dest[n-1] = '\0';

    #ifdef DEBUG
    printf("Src: %s\n",src);
    printf("Dest: %s\n",dest);
    #endif
    return dest;
}

char* strcpy(char *dest, const char *src) {
    size_t n = isWritable(dest);
    if ( n == -1 ) {
        return dest;
    }
    return strncpy(dest, src, n);
}

char* gets(char *src) {
    size_t n = isWritable(src);
    if ( n == -1 ) {
        return NULL;
    }
    int i=0;
    char j;
    char *ch = src;
    while ((j = getchar ()) != '\n' && i<n-1) {
        if (j == EOF) {
            if (ch == src || !feof(stdin))
                return NULL;
            break;
        }
        *ch++ = j; // character is stored at address, and pointer is incremented
        i++;
    }

    *ch = '\0'; //add null terminating character

    #ifdef DEBUG
    printf("%s\n",src);
    #endif

    return src;
}

char* fgets(char* dest, int n, FILE *iop)
{
    size_t max = isWritable(dest);
    printf("size is :%d\n",(int)n);
    if ( max == -1 ) {
        return NULL;
    }
    if(max<n)
        n=max;

    register int c;
    register char* cs;
    cs = dest;

    while(--n > 0 && (c = getc(iop)) != EOF)
    {
    // put the input char into the current pointer position, then increment it
    // if a newline entered, break
    if((*cs++ = c) == '\n')
        break;
    }
    *cs = '\0';

    #ifdef DEBUG
    printf("%s\n",dest);
    #endif
    return (c == EOF && cs == dest) ? NULL : dest;

}

char *strcat(char *dest, const char *src)
{

    int max = (int)isWritable(dest);
    if ( max == -1 ) {
        return NULL;
    }
    int i=0;
    char *temp = dest;
    while (*temp!= '\0'){
         *temp++ ;
         i++;
    }
    while (*src != '\0'&& i< max-1) {
        *temp++ = *src++;
        i++;
    }
    temp[max-1] = '\0';
    #ifdef DEBUG
    printf("%s\n",dest);
    #endif
    return dest;
}

// TODO: Put in handlers for handling string format attacks.
int sprintf(char *str, const char *format, ...) {
    size_t n = isWritable(str);
    if ( n == -1 ) {
        return -1;
    }

    va_list args;
    va_start(args, format);
    int r = snprintf(str, n, format, args);
    va_end(args);
#ifdef DEBUG
    printf("%s\n",str);
#endif
    return r;
}

void free(void *ptr) {
    // Create override for 'free' symbol only at start
    if( real_free == NULL ) {
        real_free = dlsym(RTLD_NEXT, "free");
    }

    #ifdef DEBUG
    printf("Global entries in global table before free.");
    print_ga();
    #endif

    int n = clearEntryInTable(ptr);
    if ( n == -1 ) {
        #ifdef DEBUG
        printf("Bad memory address(%p).  Nothing to free.\n", ptr);
        #endif
        // If a developer wants to delete a pointer that is not allocated in code,
        // (eg. argv input), it is not possible.
        return ;

    }
    real_free(ptr);

    #ifdef DEBUG
    printf("Global entries in global table after free.");
    print_ga();
    #endif
}

void *realloc(void *ptr, size_t t) {
    #ifdef DEBUG
    printf("Global entries in global table before realloc.");
    print_ga();
    #endif

    // Create override for 'realloc' symbol only at start
    if( real_realloc == NULL ) {
        real_realloc = dlsym(RTLD_NEXT, "realloc");
    }

    // If ptr is NULL, then the call is equivalent to malloc(size).  If size is equal
    // to zero, and ptr is not NULL, then the call is equivalent to free(ptr).
    // Ref: 'man realloc'
    if (ptr == NULL) {
        return malloc(t);
    }
    if (t == 0) {
        free(ptr);
        return ptr;
    }
    int n = updateEntryInTable(ptr, t);
    if ( n == -1 ) {
        #ifdef DEBUG
        printf("Memory address(%p) was never allocated.  Nothing to reallocate.\n", ptr);
        #endif
        return ptr;
    }

    #ifdef DEBUG
    printf("Global entries in global table after realloc.");
    print_ga();
    #endif

    return real_realloc(ptr, t);
}
