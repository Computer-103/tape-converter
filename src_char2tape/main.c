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
    int is_lf = 0;
    while ((ch = fgetc(input_file)) != EOF) {
        if (ch == '\n') {
            if (is_lf) {
                fputc(END_ARR, output_file);
            } else {
                fputc(WRITE, output_file);
                is_lf = 1;
            }
        } else {
            is_lf = 0;
        } 
        if (ch == '@') {
            fputc(SET_ADD, output_file);
        }
        if (ch == '+') {
            fputc(POS, output_file);
        }
        if (ch == '-') {
            fputc(NEG, output_file);
        }
        if (ch == ' ') {
            fputc(SPACE, output_file);
        }
        if (ch >= '0' && ch <= '9') {
            fputc(num_set[ch - '0'], output_file);
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