#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
** Code to test malloc in libsabo library
*/
void test_malloc(void) {
    printf("\n=== Testing malloc ===\n");
    int *i = (int *)malloc(60*sizeof(int));
    char *c = (char *)malloc(60*sizeof(char));
    double *d = (double *)malloc(60*sizeof(double));
}

/*
** Code to test strcpy and strncpy is handled safely using the sabo library
*/
void test_strcpy() {
    printf("\n=== Testing strcpy ===\n");
    char *s, *d;

    // Define source as a 10 length char array
    s = (char *)malloc(10*sizeof(char));
    s = "hell0baby\0";

    // Define destination as a 5 length char array
    d = (char *)malloc(5*sizeof(char));
    strcpy(d, s); // This should not cause overflow
    strcpy(&d[2], s);
    
    printf("\n=== Testing strncpy ===\n");
    strncpy(d, s, 5);
}

/*
** Code to test gets is handled safely using the sabo library
*/
void test_gets() {
    printf("\n=== Testing gets ===\n");
    char *s;
    // Define source as a 10 length char array
    s = (char *)malloc(5*sizeof(char));
    gets(s); // This should not cause overflow
}

/*
** Code to test fgets is handled safely using the sabo library
*/




void test_fgets() {
    printf("\n=== Testing fgets ===\n");
    char *s;
    // Define source as a 10 length char array
    s = (char *)malloc(5*sizeof(char));
    fgets(s, 10, stdin); // This should not cause overflow
    printf("%s",s);
}



void test_strcat() {
    printf("\n=== Testing strcat ===\n");
    char *s, *d;
    // Define source as a 10 length char array
    s = (char *)malloc(10*sizeof(char));
    s = "56789asdsdsdsdsdsd\0";

    d = (char *)malloc(5*sizeof(char));
    strcpy(d, "1234");

    my_strcat(d,s); // This should not cause overflow

}



/*
** Main function that tests all functionality
** TODO: Might shift testing to 'CUnit'- framework depending on time left for project.
*/
int main(void) {
    // test_malloc();
    // test_strcpy();
    // test_gets();
    // test_fgets();
    test_strcat();
    return 0;
}