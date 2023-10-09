/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 30, 2023
 */

#pragma once

#include "parser.h"

typedef struct Variable {
    char *identifier;
    size_t stack_location;
} Variable;

typedef struct Generator {
    NodeProgram *program;
    char *buffer;
    Variable *variables[PROGRAM_CAPACITY];
    size_t scopes[PROGRAM_CAPACITY];
    size_t variable_count;
    size_t scope_count;
    size_t stack_size;
    size_t label_count;
} Generator;

Generator *create_generator(NodeProgram *program, char *buffer);
Variable *create_variable(char *identifier, size_t stack_location);
Variable *find_variable(Generator *generator, char *identifier);
void generate_label(Generator *generator);
void generate_term(Generator *generator, NodeTerm *term);
void generate_expression(Generator *generator, NodeExpression *expression);
void generate_statement(Generator *generator, NodeStatement *statement);
void generate_program(Generator *generator);
void push(Generator *generator, char *reg);
void pop(Generator *generator, char *reg);
void begin_scope(Generator *generator);
void end_scope(Generator *generator);
void free_generator(Generator *generator);
