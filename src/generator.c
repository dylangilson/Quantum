/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 30, 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generator.h"

Generator *create_generator(NodeExit root) {
    Generator *generator = (Generator *)malloc(sizeof(Generator));
    generator->root = root;

    return generator;
}

char *generate(Generator *generator, char *buffer) {
    strcat(buffer, "global _start:\n\n_start:\n"); // default x86-64 boiler plate code

    strcat(buffer, "    mov rax, 60\n"); // 60 -> exit syscall
    strcat(buffer, "    mov rdi, ");

    // concatenate exit value
    long value = (long)generator->root.expression.integer_literal.value;
    int length = snprintf(NULL, 0, "%ld", value);
    char* value_as_string = malloc(sizeof(char) * (length + 1));
    snprintf(value_as_string, length + 1, "%ld", value);
    strcat(buffer, value_as_string);
    free(value_as_string);

    strcat(buffer, "\n    syscall\n");

    return buffer;
}
