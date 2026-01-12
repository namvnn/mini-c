#include <stdio.h>

void fizzbuzz(char buf[], size_t buf_size, size_t up_to) {
    size_t total_written_size = 0;

    for (size_t i = 1; i <= up_to; i++) {
        int remaining_size = buf_size - total_written_size;

        if (remaining_size <= 0) {
            break;
        }

        char *curr_pos = buf + total_written_size;
        size_t written_size = 0;
        const char *suffix = i == up_to ? "." : ", ";

        if (i % 15 == 0) {
            written_size = snprintf(curr_pos, remaining_size, "FizzBuzz%s",
                                    suffix);
        } else if (i % 3 == 0) {
            written_size = snprintf(curr_pos, remaining_size, "Fizz%s", suffix);
        } else if (i % 5 == 0) {
            written_size = snprintf(curr_pos, remaining_size, "Buzz%s", suffix);
        } else {
            written_size = snprintf(curr_pos, remaining_size, "%zu%s", i,
                                    suffix);
        }

        if (written_size < 0) {
            break;
        }

        total_written_size += written_size;
    }
}

int main(void) {
    char buf[1000];
    fizzbuzz(buf, sizeof(buf), 100);
    printf("%s\n", buf);
}
