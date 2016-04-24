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

/*
 * Read at most n-1 characters from the given file.
 * Stop when a newline has been read, or the count runs out.
 * Return first argument, or NULL if no characters were read.
 */
char *fgets(char *buf, size_t n, FILE *fp) {   
    size_t max = isWritable(buf);
    if ( max == -1 ) {
        return NULL;
    }
    if(max<n)
        n=max;

    register size_t len;
    register char *s;
    register unsigned char *p, *t;

    if (n == 0)     /* sanity check */
        return (NULL);

    s = buf;
    n--;            /* leave space for NUL */
    while (n != 0) {
        /*
         * If the buffer is empty, refill it.
         */
        if ((len = fp->_r) <= 0) {
            if (__srefill(fp)) {
                /* EOF/error: stop with partial or no line */
                if (s == buf)
                    return (NULL);
                break;
            }
            len = fp->_r;
        }
        p = fp->_p;

        /*
         * Scan through at most n bytes of the current buffer,
         * looking for '\n'.  If found, copy up to and including
         * newline, and stop.  Otherwise, copy entire chunk
         * and loop.
         */
        if (len > n)
            len = n;
        t = memchr((void *)p, '\n', len);
        if (t != NULL) {
            len = ++t - p;
            fp->_r -= len;
            fp->_p = t;
            (void)memcpy((void *)s, (void *)p, len);
            s[len] = 0;
            return (buf);
        }
        fp->_r -= len;
        fp->_p += len;
        (void)memcpy((void *)s, (void *)p, len);
        s += len;
        n -= len;
    }
    *s = 0;

#ifdef DEBUG
    printf("%s\n",buf);
#endif
    return (buf);


}
