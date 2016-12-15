#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lexer.h"

int main(int argc, char **argv) {
    FILE *stream;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    struct Lexer lexer;
    struct Token token;

    stream = fdopen(STDIN_FILENO, "rb");
    if (!stream) {
        exit(EXIT_FAILURE);
    }

    setbuf(stdout, 0);
    while(1) {
        printf(">> ");
        read = getline(&line, &len, stream);
        if (read > 0) {
            if (lexer_create(&lexer, line, line+read) != 0) {
                fprintf(stderr, "Failed to create lexer!\n");
                exit(EXIT_FAILURE);
            }

            // printf("Retrieved line of length %zu :\n", read);
            // printf("%s", line);

            do {
                if (lexer_next_token(&lexer, &token) != 0) {
                    fprintf(stderr, "Lexer failed to get next Token!\n");
                    break;
                }
                printf("%s\n", token_to_string(token.type));
            } while (token.type != EndOfFile);

            if (lexer_destroy(&lexer) != 0) {
                fprintf(stderr, "Failed to destroy lexer!\n");
                exit(EXIT_FAILURE);
            }
        } else {
            break;
        }
    }

    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);

    return 0;
}
