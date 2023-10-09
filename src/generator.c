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
Generator *create_generator(NodeProgram *program, char *buffer) {
    Generator *generator = (Generator *)malloc(sizeof(Generator));
    generator->program = program;
    generator->buffer = buffer;
    generator->variable_count = 0;
    generator->scope_count = 0;
    generator->stack_size = 0;
    generator->label_count = 0;

    return generator;
}

// create variable
Variable *create_variable(char *identifier, size_t stack_location) {
    Variable *variable = (Variable *)malloc(sizeof(Variable));
    variable->identifier = (char *)malloc(BUFFER_CAPACITY * sizeof(char));
    strcpy(variable->identifier, identifier);
    variable->stack_location = stack_location;

    return variable;
}

// find variable in array
Variable *find_variable(Generator *generator, char *identifier) {
    for (size_t i = 0; i < generator->variable_count; i++) {
        if (strncmp(generator->variables[i]->identifier, identifier, strlen(identifier)) == 0) {
            return generator->variables[i];
        }
    }

    return NULL;
}

// generate label
void generate_label(Generator *generator) {
    strcat(generator->buffer, "label");

    char count[BUFFER_CAPACITY];
    memset(count, 0, BUFFER_CAPACITY);
    snprintf(count, sizeof(count), "%zu", generator->label_count++);
    
    strcat(generator->buffer, count);
    strcat(generator->buffer, ":");
}

// generate term
void generate_term(Generator *generator, NodeTerm *term) {
    if (term->term_type == NODE_TERM_INTEGER_LITERAL) {
        strcat(generator->buffer, "    mov rax, ");

        // concatenate integer literal
        NodeTermIntegerLiteral *integer_literal = (NodeTermIntegerLiteral *)term->value;
        Token *token = (Token *)integer_literal->integer_literal;
        long value = (long)token->value;
        char value_as_string[BUFFER_CAPACITY];
        memset(value_as_string, 0, BUFFER_CAPACITY);
        snprintf(value_as_string, sizeof(value_as_string), "%ld", value);

        strcat(generator->buffer, value_as_string);
        
        free(integer_literal);

        strcat(generator->buffer, "\n");
        push(generator, "rax"); // push value in rax onto stack
        strcat(generator->buffer, "\n");
    } else if (term->term_type == NODE_TERM_IDENTIFIER) {
        Token *expression_identifier = (Token *)term->value;
        Token *identifier = (Token *)expression_identifier->value;

        Variable *variable = find_variable(generator, (char *)identifier->value);

        if (variable == NULL) {
            fprintf(stderr, "Identifier: %s not declared\n", (char *)identifier->value);
            exit(EXIT_FAILURE);
        }

        char *reg = (char *)malloc(BUFFER_CAPACITY * sizeof(char));
        memset(reg, 0, BUFFER_CAPACITY);
        char offset[BUFFER_CAPACITY];
        memset(offset, 0, BUFFER_CAPACITY);

        strcat(reg, "QWORD [rsp + ");
        snprintf(offset, sizeof(offset), "%zu", (generator->stack_size - variable->stack_location - 1) * 8); // 8 bytes -> 64 bits
        strcat(reg, offset);
        strcat(reg, "]");

        push(generator, reg);

        free(reg);
        free(expression_identifier);
        free(identifier->value);
    }
}

// generate expression
void generate_expression(Generator *generator, NodeExpression *expression) {
    if (expression->expression_type == NODE_EXPRESSION_TERM) {
        NodeTerm *term = (NodeTerm *)expression->expression;

        generate_term(generator, term);

        free(term);
    } else if (expression->expression_type == NODE_EXPRESSION_BINARY) {
        NodeBinaryExpression *binary_expression_node = (NodeBinaryExpression *)expression->expression;
        BinaryExpression *binary_expression = binary_expression_node->expression;

        generate_expression(generator, binary_expression->left_hand_side);
        generate_expression(generator, binary_expression->right_hand_side);

        pop(generator, "rax");
        pop(generator, "rbx");

        if (binary_expression_node->type == NODE_BINARY_MULTIPLICATION) {
            strcat(generator->buffer, "    mul rax, rbx\n");
            push(generator, "rax");
        } else if (binary_expression_node->type == NODE_BINARY_DIVISION) {
            // TODO: division algorithm in x86-64
            push(generator, "rax");
        } else if (binary_expression_node->type == NODE_BINARY_ADDITION) {
            strcat(generator->buffer, "    add rax, rbx\n");
            push(generator, "rax");
        } else if (binary_expression_node->type == NODE_BINARY_SUBTRACTION) {
            strcat(generator->buffer, "    sub rax, rbx\n");
            push(generator, "rax");
        }

        strcat(generator->buffer, "\n");

        free(binary_expression);
        free(binary_expression_node);
    }

    free(expression);
}

// generate statement
void generate_statement(Generator *generator, NodeStatement *statement) {
    if (statement->statement_type == NODE_STATEMENT_EXIT) {
        NodeStatementExit *exit_statement = (NodeStatementExit *)statement->statement;
        NodeExpression *expression = (NodeExpression *)exit_statement->expression;

        generate_expression(generator, expression); // evaulate expression

        free(exit_statement);

        strcat(generator->buffer, "    mov rax, 60\n"); // 60 -> exit syscall
        pop(generator, "rdi"); // pop expression value into rdi
        strcat(generator->buffer, "    syscall\n\n");
    } else if (statement->statement_type == NODE_STATEMENT_LET) {
        NodeStatementLet *let_statement = (NodeStatementLet *)statement->statement;
        Token *identifier = (Token *)let_statement->identifier;
        
        Variable *temp = find_variable(generator, (char *)identifier->value);

        // identifier already declared
        if (temp != NULL) {
            fprintf(stderr, "Identifier: %s already declared\n", (char *)identifier->value);
            exit(EXIT_FAILURE);
        }

        Variable *variable = create_variable((char *)identifier->value, generator->stack_size);

        generator->variables[generator->variable_count++] = variable;

        generate_expression(generator, let_statement->expression); // evaulate expression

        free(let_statement);
        free(identifier->value);
    } 
}

// generate x86-64 assembly from input program
void generate_program(Generator *generator) {
    strcat(generator->buffer, "global _start\n\n_start:\n"); // default x86-64 boiler plate code

    for (size_t i = 0; i < generator->program->program_length; i++) {
        NodeStatement *statement = generator->program->statements[i];
        generate_statement(generator, statement);
        free(statement);
    }

    // default exit with code 0
    strcat(generator->buffer, "    mov rax, 60\n"); // 60 -> exit syscall
    strcat(generator->buffer, "    mov rdi, 0\n"); // exit code 0
    strcat(generator->buffer, "    syscall\n");
}

// push value in register onto stack
void push(Generator *generator, char *reg) {
    strcat(generator->buffer, "    push ");
    strcat(generator->buffer, reg);
    strcat(generator->buffer, "\n");

    generator->stack_size++;
}

// pop value from stack into register
void pop(Generator *generator, char *reg) {
    strcat(generator->buffer, "    pop ");
    strcat(generator->buffer, reg);
    strcat(generator->buffer, "\n");

    generator->stack_size--;
}

// begin scope
void begin_scope(Generator *generator) {
    generator->scopes[generator->scope_count++] = generator->variable_count;
}

// end scope
void end_scope(Generator *generator) {
    size_t pop_count = generator->variable_count - generator->scope_count;

    char count[BUFFER_CAPACITY];
    memset(count, 0, BUFFER_CAPACITY);
    snprintf(count, sizeof(count), "%zu", pop_count * 8); // 8 bytes -> 64 bits

    strcat(generator->buffer, "    add rsp, ");
    strcat(generator->buffer, count);
    strcat(generator->buffer, "\n");

    for (size_t i = 0; i < pop_count; i++) {
        free(generator->variables[generator->variable_count--]->identifier);
    }

    generator->scope_count--;
}

void free_generator(Generator *generator) {
    for (size_t i = 0; i < generator->variable_count; i++) {
        free(generator->variables[i]->identifier);
        free(generator->variables[i]);
    }

    free(generator);
}
