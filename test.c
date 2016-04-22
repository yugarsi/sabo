#include <stdio.h>
#include "libsabo.h"


/*
** Code to test strcpy is handled safely using the libsabo library
** TODO: Might parameterize with source string (char *) input from main.  Optional.
*/
void test_strcpy() {

    sabs *source, *dest;

    // Define source as a 10 length char array
    source = s_malloc(10);
    source -> p = "hell0baby\0";

    // Define destination as a 5 length char array
    dest = s_malloc(5);
    sab_strcpy(dest, source); // This should not cause overflow

    printf("%s\n",source->p);
    printf("%s\n",dest->p);
}

/*
** Main function that tests all functionality
** TODO: Might shift testing to 'CUnit'- framework depending on time left for project.
*/
int main(void) {
    test_strcpy();
    return 0;
}
