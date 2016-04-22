#ifndef LIBSABO_H_INCLUDED
#define SABO_H_INCLUDED

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

sabs* s_malloc(int size);
sabi* i_malloc(int size);
sabf* f_malloc(int size);
sabd* d_malloc(int size);
sabs* sab_strcpy(sabs *dest, sabs* src);
#endif
