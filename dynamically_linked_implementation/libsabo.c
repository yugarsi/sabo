#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
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

size_t isWritable( void *p ) {
    int i;
    for( i=0; i<count; i++) {
        if ( ga[i].ptr <= p && p <= ga[i].ptr + ga[i].size ) {
            return (ga[i].ptr + ga[i].size) - p;
        }
    }
    return -1;
}

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
        return NULL;
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

//--------------------------------------------------


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
