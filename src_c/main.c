#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <err.h>
#include <ctype.h>

#include "tape.h"

static char short_options[] = "o:";
static struct option long_options[] = {
    {"output",  required_argument,  0, 'o'},
    {0,         0,                  0,  0 },
};

void punch_tape(char ch) {
    printf("|");
    printf("%c", (ch & (1 << 4)) ? '@' : ' ');
    printf("%c", (ch & (1 << 3)) ? '@' : ' ');
    printf("o");
    printf("%c", (ch & (1 << 2)) ? '@' : ' ');
    printf("%c", (ch & (1 << 1)) ? '@' : ' ');
    printf("%c", (ch & (1 << 0)) ? '@' : ' ');
    printf("|\n");
}

void translate(FILE * input_file, FILE * output_file) {
    static char num_set[10] = {NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9};
    static char line[32];
    while (fgets(line, 32, input_file) != NULL) {
        int line_i = 0;
        char ch;

        for (int i = 0; i < 4; i++) {
            do {
                ch = line[line_i++];
            } while (isspace(ch));

            if (!isdigit(ch)) {
                fprintf(stderr, "wrong format\n");
            }

            fputc(num_set[ch - '0'], output_file);
            punch_tape(num_set[ch - '0']);
        }

        fputc(SET_ADD, output_file);
        punch_tape(SET_ADD);

        do {
            ch = line[line_i++];
        } while (isspace(ch));

        if (ch == '+') {
            fputc(POS, output_file);
            punch_tape(POS);
        } else if (ch == '-') {
            fputc(NEG, output_file);
            punch_tape(NEG);
        } else {
            fprintf(stderr, "wrong format\n");
        }
        
        for (int i = 0; i < 10; i++) {
            do {
                ch = line[line_i++];
            } while (isspace(ch));
            
            if (!isdigit(ch)) {
                fprintf(stderr, "wrong format\n");
            }

            fputc(num_set[ch - '0'], output_file);
            punch_tape(num_set[ch - '0']);
        }

        fputc(WRITE, output_file);
        punch_tape(WRITE);
        punch_tape(SPACE);
    }
    fputc(END_ARR, output_file);
    punch_tape(END_ARR);
}

int main(int argc, char ** argv) {
    static FILE * output_file = NULL;
    static FILE * input_file = NULL;

    while (1) {
        int c = getopt_long(argc, argv, short_options, long_options, NULL);
        if (c == -1) {
            break;
        }

        if (c == 0) {
            // nothing to do
        } else if (c == 'o') {
            output_file = fopen(optarg, "wb");
            if (!output_file) {
                err(EXIT_FAILURE, "%s", optarg);
            }

        } else {
            fprintf(stderr, "Usage: %s [-o <output_filename>] <input_filename>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Usage: %s [-o <output_filename>] <input_filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    input_file = fopen(argv[optind], "r");
    if (!output_file) {
        output_file = stdout;
    }

    translate(input_file, output_file);

    exit(EXIT_SUCCESS);
}