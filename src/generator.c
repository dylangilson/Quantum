/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 30, 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generator.h"

// create generator
Generator *create_generator(NodeProgram *program) {
    Generator *generator = (Generator *)malloc(sizeof(Generator));
    generator->program = program;
    generator->buffer = (char *)malloc(BUFFER_CAPACITY * sizeof(char));
    memset(generator->buffer, 0, BUFFER_CAPACITY);

    return generator;
}

// generate expression
void generate_expression(NodeExpression *expression, char *buffer) {
    if (expression->expression_type == NODE_EXPRESSION_INTEGER_LITERAL) {
        strcat(buffer, "    mov rax, ");

        // concatenate integer literal
        NodeExpressionIntegerLiteral *integer_literal = (NodeExpressionIntegerLiteral *)expression->expression;
        long value = (long)integer_literal->integer_literal->value;
        int length = snprintf(NULL, 0, "%ld", value);
        char* value_as_string = malloc(sizeof(char) * (length + 1));
        snprintf(value_as_string, length + 1, "%ld", value);
        strcat(buffer, value_as_string);
        
        free(value_as_string);
        free(integer_literal);

        strcat(buffer, "\n");
        strcat(buffer, "    push rax\n\n"); // push value in rax onto stack
        
    } else if (expression->expression_type == NODE_EXPRESSION_IDENTIFIER) {
        
    }
}

// generate statement
void generate_statement(NodeStatement *statement, char *buffer) {
    if (statement->statement_type == NODE_STATEMENT_EXIT) {
        NodeStatementExit *exit_statement = (NodeStatementExit *)statement->statement;
        NodeExpression *expression = (NodeExpression *)exit_statement->expression;

        generate_expression(expression, buffer);

        free(exit_statement);
        free(expression);

        strcat(buffer, "    mov rax, 60\n"); // 60 -> exit syscall
        strcat(buffer, "    pop rdi\n"); // pop expression value into rdi
        strcat(buffer, "    syscall\n\n");
    } else if (statement->statement_type == NODE_STATEMENT_LET) {

    } 
}

// generate x86-64 assembly from input program
void generate_program(Generator *generator, char *buffer) {
    strcat(buffer, "global _start:\n\n_start:\n"); // default x86-64 boiler plate code

    for (int i = 0; i < generator->program->program_length; i++) {
        NodeStatement *statement = generator->program->statements[i];
        generate_statement(statement, buffer);
        free(statement);
    }

    // default exit with code 0
    strcat(buffer, "    mov rax, 60\n"); // 60 -> exit syscall
    strcat(buffer, "    mov rdi, 0\n"); // exit code 0
    strcat(buffer, "    syscall\n");
}
