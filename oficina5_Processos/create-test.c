/* Creates an unrolled loop with the right skips to avoid testing for small
   primes
*/

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    long n = -1;
    if (argc>1) {
        char *notnumber_ptr;
        n = strtol(argv[1], &notnumber_ptr, 10);
        n = *notnumber_ptr == '\0' ? n : -1;
    }

    if (n<1 || n>5){
        fprintf(stderr, "usage: create-test <n>\n"
                        "creates a test allowing to skip the first n primes\n"
                        "in a primality test,  1 <= n <= 5.\n");
        return EXIT_FAILURE;
    }

    const int first_primes[] = { 2, 3, 5, 7, 11 };
    int primorial = 1;
    for (size_t i=0; i<n; i++) {
        primorial *= first_primes[i];
    }

    // We will build an array of residues, such that only numbers with those
    // residues modulo primorial(n) will have to be considered

    // Generate code of test
    int tests = 1;
    printf("// [[ ---------- code generated by create-test.c\n");
    printf("(candidate %% (divisor+1) == 0)");
    for (int residue=2; residue<primorial; residue++) {
        bool keep = true;
        for (size_t i=0; keep && i<n; i++) keep = residue%first_primes[i] != 0;
        if (keep) {
            tests++;
            printf(" ||\n");
            printf("(candidate %% (divisor+%d) == 0)", residue);
        }
    }
    printf("\n");
    printf("// ---------- ]]\n");

    fprintf(stderr, "tests: %d, fraction: %.4f\n",
            tests, ((float)tests)/primorial);

    return EXIT_SUCCESS;
}


