#ifndef LIBSABO_H_INCLUDED
#define SABO_H_INCLUDED

typedef struct size_aware_buffer_char {
    char *p;
    unsigned int length;
}sabs;

typedef struct size_aware_buffer_double {
    double *p;
    unsigned int length;
}sabd;

typedef struct size_aware_buffer_int {
    int *p;
    unsigned int length;
}sabi;

typedef struct size_aware_buffer_float {
    float *p;
    unsigned int length;
}sabf;

sabs* s_malloc(unsigned int size);
sabi* i_malloc(unsigned int size);
sabf* f_malloc(unsigned int size);
sabd* d_malloc(unsigned int size);
sabs* sab_strcpy(sabs *dest, sabs* src);
#endif
