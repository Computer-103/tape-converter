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

void translate(FILE * input_file, FILE * output_file) {
    static char num_set[10] = {NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9};
    char ch;
    while ((ch = fgetc(input_file)) != EOF) {
        if (ch == END_ARR || ch == WRITE) {
            fputc('\n', output_file);
        }
        if (ch == SET_ADD) {
            fputc('@', output_file);
        }
        if (ch == POS) {
            fputc('+', output_file);
        }
        if (ch == NEG) {
            fputc('-', output_file);
        }
        if (ch == SPACE) {
            fputc(' ', output_file);
        }
        if (ch == NUM0) {
            fputc('0', output_file);
        }
        if (ch == NUM1) {
            fputc('1', output_file);
        }
        if (ch == NUM2) {
            fputc('2', output_file);
        }
        if (ch == NUM3) {
            fputc('3', output_file);
        }
        if (ch == NUM4) {
            fputc('4', output_file);
        }
        if (ch == NUM5) {
            fputc('5', output_file);
        }
        if (ch == NUM6) {
            fputc('6', output_file);
        }
        if (ch == NUM7) {
            fputc('7', output_file);
        }
        if (ch == NUM8) {
            fputc('8', output_file);
        }
        if (ch == NUM9) {
            fputc('9', output_file);
        }
    }
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
            output_file = fopen(optarg, "w");
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

    input_file = fopen(argv[optind], "rb");
    if (!output_file) {
        output_file = stdout;
    }

    translate(input_file, output_file);

    exit(EXIT_SUCCESS);
}