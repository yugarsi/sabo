#include <stdio.h>
#include <stdlib.h>

/*
** Code to test strcpy is handled safely using the libsabo library
** TODO: Might parameterize with source string (char *) input from main.  Optional.
*/

void test_malloc(void) {
    int *a = (int *)malloc(1);
    int *b = (int *)malloc(4);
}



/*
** Main function that tests all functionality
** TODO: Might shift testing to 'CUnit'- framework depending on time left for project.
*/
int main(void) {
    test_malloc();
    return 0;

}
