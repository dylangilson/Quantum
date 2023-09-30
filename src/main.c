/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 24, 2023
 */

/*** Compilation & Running ***/
// make
// ./quantum <input.qtm>
// ./output


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"

#define BUFFER_CAPACITY 4096

// convert string into list of tokens
LinkedList *tokenize(LinkedList *tokens, const char *str) {
    char buffer[BUFFER_CAPACITY]; // file contents
    memset(buffer, 0, sizeof(buffer)); // clear buffer

    for (int i = 0; str[i] != 0; i++) {
        char c = str[i]; // get character
        
        if (isalpha(c)) {
            strncat(buffer, &c, 1); // add character to buffer
            i++;

            // while character is alphanumeric append to buffer
            while (isalnum(str[i])) {
                strncat(buffer, &str[i], 1);
                i++;
            }

            i--;

            // push token to token list
            if (strncmp(buffer, "exit", strlen(buffer)) == 0) {
                push_tail(tokens, NULL, TOKEN, EXIT); // append token
                memset(buffer, 0, sizeof(buffer)); // clear buffer
            } else {
                fprintf(stderr, "Invalid expression\n");
                exit(EXIT_FAILURE);
            }
        } else if (isdigit(c)) {
            strncat(buffer, &c, 1); // add character to buffer
            i++;

            // while character is digit append to buffer
            while (isdigit(str[i])) {
                strncat(buffer, &str[i], 1);
                i++;
            }

            i--;

            long temp = atoi(buffer);
            push_tail(tokens, (int *)temp, INTEGER, INTEGER_LITERAL); // append number
            memset(buffer, 0, sizeof(buffer)); // clear buffer
        } else if (c == ';') {
            push_tail(tokens, NULL, TOKEN, SEMICOLON); // append semicolon
        } else if (isspace(c)) {
            continue; // ignore whitespace        
        } else {
            fprintf(stderr, "Invalid expression\n");
            exit(EXIT_FAILURE);
        }
    }

    return tokens;
}

char *tokens_to_asm(LinkedList *tokens, char *buffer) {
    strcat(buffer, "global _start:\n\n_start:\n"); // default x86-64 boiler plate code

    for (int i = 0; i < tokens->count; i++) {
        Node *node = at(*tokens, i);

        if (node->token.token_type == EXIT) {
            if (i + 1 < tokens->count && peek_at(*tokens, i + 1)->token.token_type == INTEGER_LITERAL) {
                if (i + 2 < tokens->count && peek_at(*tokens, i + 2)->token.token_type == SEMICOLON) {
                    strcat(buffer, "    mov rax, 60\n"); // 60 -> exit syscall
                    strcat(buffer, "    mov rdi, ");

                    // concatenate exit value
                    int length = snprintf(NULL, 0, "%ld", (long)peek_at(*tokens, i + 1)->token.value);
                    char* value_as_string = malloc(sizeof(char) * (length + 1));
                    snprintf(value_as_string, length + 1, "%ld", (long)peek_at(*tokens, i + 1)->token.value);
                    strcat(buffer, value_as_string);
                    free(value_as_string);

                    strcat(buffer, "\n    syscall\n");
                }
            }
        }
    }

    return buffer;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect usage. Correct usage is: 'quantum <input.qtm>'\n");
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r"); // open file and return pointer

    if (file == NULL) {
        fprintf(stderr, "Unable to open file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    char file_buffer[BUFFER_CAPACITY]; // file contents
    memset(file_buffer, 0, sizeof(file_buffer)); // clear buffer

    char temp_file_buffer[BUFFER_CAPACITY]; // individual line buffer
    memset(temp_file_buffer, 0, sizeof(temp_file_buffer)); // clear buffer

    // read file into buffer
    while(fgets(temp_file_buffer, BUFFER_CAPACITY, file)) {
        strcat(file_buffer, temp_file_buffer);
    }

    fclose(file); // close file

    LinkedList *tokens = create_list();
    tokens = tokenize(tokens, file_buffer);

    char *assembly = (char *)malloc(sizeof(char) * BUFFER_CAPACITY);
    memset(assembly, 0, BUFFER_CAPACITY);
    tokens_to_asm(tokens, assembly);

    FILE *output = fopen("output.asm", "w");

    if (output == NULL) {
        fprintf(stderr, "Unable to create output file\n");
        return EXIT_FAILURE;
    }

    fputs(assembly, output);
    fclose(output);

    free_list(tokens);
    free(assembly);

    system("nasm -felf64 output.asm && ld output.o -o output");

    return EXIT_SUCCESS;
}
