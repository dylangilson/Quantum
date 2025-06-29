/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 30, 2023
 */

#pragma once

#include "parser.h"

typedef struct Variable {
    char *identifier;
    NodeStatementDeclarationType declaration_type;
    size_t stack_location;
} Variable;

typedef struct Generator {
    NodeProgram *program;
    char *buffer;
    Variable *variables[PROGRAM_CAPACITY];
    Variable *global_variables[PROGRAM_CAPACITY];
    size_t scopes[PROGRAM_CAPACITY];
    size_t variable_count;
    size_t global_variable_count;
    size_t scope_count;
    size_t stack_size;
    size_t label_count;
} Generator;

Generator *create_generator(NodeProgram *program, char *buffer);
Variable *create_variable(char *identifier, NodeStatementDeclarationType declaration_type, size_t stack_location);
char *create_label(Generator *generator);
Variable *find_variable(Generator *generator, const char *identifier);
long evaluate_term(Generator *generator, NodeTerm *term, const char *global_identifier);
long evaluate_expression(Generator *generator, NodeExpression *expression, const char *global_identifier);
void generate_global(Generator *generator, NodeStatementDeclaration *declaration_statement);
void generate_label(Generator *generator);
void generate_term(Generator *generator, NodeTerm *term);
void generate_expression(Generator *generator, NodeExpression *expression);
void generate_scope(Generator *generator, NodeScope *scope);
void generate_if_predicate(Generator *generator, NodeIfPredicate *if_predicate, char *end_label);
void generate_statement(Generator *generator, NodeStatement *statement);
void generate_program(Generator *generator);
void push(Generator *generator, const char *reg);
void pop(Generator *generator, const char *reg);
void begin_scope(Generator *generator);
void end_scope(Generator *generator);
void free_generator(Generator *generator);
