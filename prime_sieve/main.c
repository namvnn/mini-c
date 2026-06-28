#include <stdio.h>

int main(void) {
    const size_t MAX_NUMBER = 1000;
    const size_t FIRST_PRIME = 2;
    const size_t NUM_ELEMENTS = MAX_NUMBER - FIRST_PRIME + 1;
    unsigned int primes[NUM_ELEMENTS];

    for (size_t i = 0; i < NUM_ELEMENTS; i++) {
        if (i == 0) {
            primes[i] = FIRST_PRIME;
        } else {
            size_t num = FIRST_PRIME + i;
            primes[i] = num % 2 == 0 ? 0 : num;
        }
    }

    for (size_t i = 1; i < NUM_ELEMENTS; i++) {
        unsigned int smallest = primes[i];
        if (smallest == 0) {
            continue;
        }

        for (size_t j = i + 1; j < NUM_ELEMENTS; j++) {
            if (primes[j] % smallest == 0) {
                primes[j] = 0;
            }
        }
    }

    size_t prime_count = 0;
    for (size_t i = 0; i < NUM_ELEMENTS; i++) {
        if (primes[i] != 0) {
            prime_count++;
            printf("%d ", primes[i]);
        }
    }

    printf("\n\nThere are %zu prime numbers up to the number %zu.\n",
           prime_count, MAX_NUMBER);
}
