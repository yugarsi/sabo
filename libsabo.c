#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef struct size_aware_buffer_char {
    char *p;
    int length;
}sabs;

typedef struct size_aware_buffer_double {
    double *p;
    int length;
}sabd;

typedef struct size_aware_buffer_int {
    int *p;
    int length;
}sabi;

typedef struct size_aware_buffer_float {
    float *p;
    int length;
}sabf;

sabs* s_malloc(int size) {
    sabs *s = (sabs *)malloc(sizeof(sabs));
    s->p = malloc(size*sizeof(char));
    s->length = size;
    return s;
}

sabi* i_malloc(int size) {
    sabi *s = (sabi *)malloc(sizeof(sabi));
    s->p = malloc(size*sizeof(int));
    s->length = size;
    return s;
}

sabf* f_malloc(int size) {
    sabf *s = (sabf *)malloc(sizeof(sabf));
    s->p = malloc(size*sizeof(float));
    s->length = size;
    return s;
}

sabd* d_malloc(int size) {
    sabd *s = (sabd *)malloc(sizeof(sabd));
    s->p = malloc(size*sizeof(double));
    s->length = size;
    return s;
}

sabs* sab_strcpy(sabs *dest, sabs* src) {
    int n = dest->length;
    int i;
    for (i = 0; i < n-1 && src->p[i] != '\0'; i++)
        dest -> p[i] = src -> p[i];
    for ( ; i < n ; i++)
        dest -> p[i] = '\0';

    // This additional line addresses the vulnerability present in 'strncpy'
    dest -> p[n-1] = '\0';

    return dest;
}
