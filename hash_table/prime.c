#include "prime.h"
#include <math.h>

/**
 * Determine if the given number is prime or not.
 *
 * @param num a number.
 * @erturn 1 if prime, 0 if not prime, or -1 if undefined (i.e. x < 2).
 */
int is_prime(const int num) {
    if (num < 2) {
        return -1;
    }
    if (num < 4) {
        return 1;
    }
    if ((num % 2) == 0) {
        return 0;
    }
    for (int i = 3; i <= floor(sqrt((double)num)); i += 2) {
        if ((num % i) == 0) {
            return 0;
        }
    }
    return 1;
}

/**
 * Get the next prime after the given number,
 * or the given number if it is prime.
 *
 * @param num a number.
 * @return the next prime after `num`, or `num` if it is prime.
 */
int next_prime(const int num) {
    int next = num;
    while (is_prime(next) != 1) {
        next++;
    }
    return next;
}
