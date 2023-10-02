/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 30, 2023
 */

#pragma once

#include "parser.h"

typedef struct Generator {
    NodeProgram *program;
    char *buffer;
} Generator;

Generator *create_generator(NodeProgram *program);
void generate_expression(NodeExpression *expression, char *buffer);
void generate_statement(NodeStatement *statement, char *buffer);
void generate_program(Generator *generator, char *buffer);
