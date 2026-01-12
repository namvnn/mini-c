#include <stdbool.h>
#include <stdio.h>
#include <string.h>

char *ONES[] = {
    "zero",    "one",     "two",       "three",    "four",
    "five",    "six",     "seven",     "eight",    "nine",
    "ten",     "eleven",  "twelve",    "thirteen", "fourteen",
    "fifteen", "sixteen", "seventeen", "eighteen", "nineteen",
};

char *TENS[] = {
    "",      "",      "twenty",  "thirty", "forty",
    "fifty", "sixty", "seventy", "eighty", "ninety",
};

char *SCALES[] = {
    "",         "thousand",    "million",     "billion",
    "trillion", "quadrillion", "quintillion",
};

const size_t MAX_GROUP_SIZE = 3;

struct testcase {
    char *input;
    char *expected;
};

void number_wordinator(char output[], size_t output_size, char input_digits[],
                       size_t input_len) {
    size_t group_total = (input_len + MAX_GROUP_SIZE - 1) / MAX_GROUP_SIZE;
    size_t output_len = 0;

    for (size_t group_i = 0; group_i < group_total; group_i++) {
        char group[3] = { '0', '0', '0' };

        int input_offset_i = group_i * MAX_GROUP_SIZE;
        if (input_len % MAX_GROUP_SIZE != 0) {
            int missing = MAX_GROUP_SIZE - (input_len % MAX_GROUP_SIZE);
            input_offset_i -= missing;
        }
        for (size_t i = 0; i < MAX_GROUP_SIZE; i++) {
            int digit_i = input_offset_i + i;
            if (digit_i >= 0 && digit_i < (int)input_len) {
                group[i] = input_digits[digit_i];
            }
        }

        int number = (group[0] - '0') * 100 + //
                     (group[1] - '0') * 10 +  //
                     (group[2] - '0');

        if (number == 0) {
            continue;
        }

        int temp_number = number;
        if (temp_number >= 100) {
            char *format = temp_number % 100 == 0 ? "%s hundred"
                                                  : "%s hundred "
                                                    "and ";
            output_len += snprintf(output + output_len,
                                   output_size - output_len, format,
                                   ONES[temp_number / 100]);
            temp_number %= 100;
        }
        if (temp_number >= 20) {
            char *format = temp_number % 10 == 0 ? "%s" : "%s-";
            output_len += snprintf(output + output_len,
                                   output_size - output_len, format,
                                   TENS[temp_number / 10]);
            temp_number %= 10;
        }
        if (temp_number > 0) {
            char *format = "%s";
            output_len += snprintf(output + output_len,
                                   output_size - output_len, format,
                                   ONES[temp_number]);
        }

        size_t scale_index = group_total - 1 - group_i;
        if (number > 0 && scale_index > 0) {
            char *format = " %s, ";
            output_len += snprintf(output + output_len,
                                   output_size - output_len, format,
                                   SCALES[scale_index]);
        }
    }

    if (output_len >= 2 &&               //
        output[output_len - 2] == ',' && //
        output[output_len - 1] == ' '    //
    ) {
        output[output_len - 2] = '\0';
        output[output_len - 1] = '\0';
    }

    if (output_len == 0) {
        snprintf(output, output_size, "%s", ONES[0]);
    }
}

int main(void) {
    struct testcase testcases[] = {
        {
            .input = "0",
            .expected = "zero",
        },
        {
            .input = "000",
            .expected = "zero",
        },
        {
            .input = "7",
            .expected = "seven",
        },
        {
            .input = "00007",
            .expected = "seven",
        },
        {
            .input = "19",
            .expected = "nineteen",
        },
        {
            .input = "019",
            .expected = "nineteen",
        },
        {
            .input = "20",
            .expected = "twenty",
        },
        {
            .input = "21",
            .expected = "twenty-one",
        },
        {
            .input = "99",
            .expected = "ninety-nine",
        },
        {
            .input = "100",
            .expected = "one hundred",
        },
        {
            .input = "00100",
            .expected = "one hundred",
        },
        {
            .input = "101",
            .expected = "one hundred and one",
        },
        {
            .input = "110",
            .expected = "one hundred and ten",
        },
        {
            .input = "112",
            .expected = "one hundred and twelve",
        },
        {
            .input = "120",
            .expected = "one hundred and twenty",
        },
        {
            .input = "143",
            .expected = "one hundred and forty-three",
        },
        {
            .input = "999",
            .expected = "nine hundred and ninety-nine",
        },
        {
            .input = "1000",
            .expected = "one thousand",
        },
        {
            .input = "00001000",
            .expected = "one thousand",
        },
        {
            .input = "1001",
            .expected = "one thousand, one",
        },
        {
            .input = "1010",
            .expected = "one thousand, ten",
        },
        {
            .input = "1043",
            .expected = "one thousand, forty-three",
        },
        {
            .input = "0000001043",
            .expected = "one thousand, forty-three",
        },
        {
            .input = "1100",
            .expected = "one thousand, one hundred",
        },
        {
            .input = "1101",
            .expected = "one thousand, one hundred and one",
        },
        {
            .input = "1143",
            .expected = "one thousand, one hundred and forty-three",
        },
        {
            .input = "9999",
            .expected = "nine thousand, nine hundred and "
                        "ninety-nine",
        },
        {
            .input = "1000000",
            .expected = "one million",
        },
        {
            .input = "00001000000",
            .expected = "one million",
        },
        {
            .input = "1000001",
            .expected = "one million, one",
        },
        {
            .input = "1000100",
            .expected = "one million, one hundred",
        },
        {
            .input = "1001100",
            .expected = "one million, one thousand, one hundred",
        },
        {
            .input = "1147483647",
            .expected = "one billion, one hundred and forty-seven "
                        "million, four hundred and "
                        "eighty-three thousand, six hundred and "
                        "forty-seven",
        },
        {
            .input = "0001147483647",
            .expected = "one billion, one hundred and forty-seven "
                        "million, four hundred and "
                        "eighty-three thousand, six hundred and "
                        "forty-seven",
        },
        {
            .input = "123456789",
            .expected = "one hundred and twenty-three million, "
                        "four hundred and "
                        "fifty-six thousand, seven hundred and "
                        "eighty-nine",
        },
        {
            .input = "2000000000",
            .expected = "two billion",
        },
        {
            .input = "000002000000000",
            .expected = "two billion",
        },
        {
            .input = "2000001001",
            .expected = "two billion, one thousand, one",
        },
        {
            .input = "7828660194658220598",
            .expected = "seven quintillion, eight hundred and "
                        "twenty-eight quadrillion, six "
                        "hundred and sixty trillion, one hundred "
                        "and ninety-four billion, six "
                        "hundred and fifty-eight million, two "
                        "hundred and twenty thousand, "
                        "five hundred and ninety-eight",
        },
        {
            .input = "0007828660194658220598",
            .expected = "seven quintillion, eight hundred and "
                        "twenty-eight quadrillion, six "
                        "hundred and sixty trillion, one hundred "
                        "and ninety-four billion, six "
                        "hundred and fifty-eight million, two "
                        "hundred and twenty thousand, "
                        "five hundred and ninety-eight",
        },
    };

    size_t passed = 0;
    size_t total = sizeof(testcases) / sizeof(struct testcase);

    for (size_t i = 0; i < total; i++) {
        struct testcase tc = testcases[i];
        char *input = tc.input;
        size_t input_len = strlen(input);
        char output[1000] = { 0 };
        size_t output_size = sizeof(output);
        char *expected = tc.expected;

        number_wordinator(output, output_size, input, input_len);

        if (strcmp(output, expected) == 0) {
            passed++;
            printf("\033[1;42m PASS \033[0m \033[1m%s\033[0m\n", input);
        } else {
            printf("\033[1;41m FAIL \033[0m \033[1m%s\033[0m\n", input);
            printf("  \033[32mExpected\033[0m: %s\n", expected);
            printf("    \033[31mActual\033[0m: %s\n", output);
        }
    }

    size_t failed = total - passed;

    printf("\n\033[1;43m   REPORT   \033[0m\n");
    printf("Total:  \033[1m%zu\033[0m\n", total);
    if (passed > 0) {
        printf("Passed: \033[1;32m%zu\033[0m\n", passed);
    }
    if (failed > 0) {
        printf("Failed: \033[1;31m%zu\033[0m\n", failed);
    }
}
