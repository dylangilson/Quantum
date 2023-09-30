/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 24, 2023
 */

/*** Compilation & Running ***/
// make
// ./quantum <input.qtm>
// ./output

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"
#include "tokenizer.h"
#include "parser.h"
#include "generator.h"

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

    Tokenizer *tokenizer = create_tokenizer(file_buffer);
    LinkedList *tokens = create_list();
    tokens = tokenize(tokenizer, tokens);

    free(tokenizer);

    Parser *parser = create_parser(tokens);
    NodeExit *tree = parse_tree(parser);

    free(parser);

    if (tree == NULL) {
        fprintf(stderr, "No exit statement found\n");
        return EXIT_FAILURE;
    }

    char *assembly = (char *)malloc(sizeof(char) * BUFFER_CAPACITY);
    memset(assembly, 0, BUFFER_CAPACITY);
    Generator *generator = create_generator(*tree);
    assembly = generate(generator, assembly);

    free(tree);
    free(generator);

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
