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
Variable *create_variable(char *identifier, NodeStatementDeclarationType declaration_type, size_t stack_location) {
    Variable *variable = (Variable *)malloc(sizeof(Variable));
    variable->identifier = (char *)malloc(BUFFER_CAPACITY * sizeof(char));
    strcpy(variable->identifier, identifier);
    variable->declaration_type = declaration_type;
    variable->stack_location = stack_location;

    return variable;
}

// create label
char *create_label(Generator *generator) {
    char *label = (char *)malloc(BUFFER_CAPACITY);
    memset(label, 0, BUFFER_CAPACITY);
    strcat(label, "label");

    char index[BUFFER_CAPACITY];
    memset(index, 0, BUFFER_CAPACITY);

    snprintf(index, sizeof(index), "%ld", generator->label_count++);

    strcat(label, index);

    return label;
}

// find variable in array
Variable *find_variable(Generator *generator, char *identifier) {
    for (size_t i = 0; i < generator->variable_count; i++) {
        if (strcmp(generator->variables[i]->identifier, identifier) == 0) {
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
        Token *integer_literal = term->value;
        long value = (long)integer_literal->value;
        char value_as_string[BUFFER_CAPACITY];
        memset(value_as_string, 0, BUFFER_CAPACITY);
        snprintf(value_as_string, sizeof(value_as_string), "%ld", value);

        strcat(generator->buffer, value_as_string);
        strcat(generator->buffer, "\n");
        push(generator, "rax"); // push value in rax onto stack
        strcat(generator->buffer, "\n");
    } else if (term->term_type == NODE_TERM_IDENTIFIER) {
        Token *identifier = term->value;

        Variable *variable = find_variable(generator, (char *)identifier->value);

        if (variable == NULL) {
            fprintf(stderr, "Identifier: %s not declared before attempted access on line %zu\n", (char *)identifier->value, (size_t)identifier->line_number);
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
        free(identifier->value);
    } else if (term->term_type == NODE_TERM_PARENTHESIS) {
        NodeTermParenthesis *parenthesis_term = (NodeTermParenthesis *)term->value;

        generate_expression(generator, parenthesis_term->expression);

        free(parenthesis_term);
    } else {
        fprintf(stderr, "Invalid NodeTermType\n");
        exit(EXIT_FAILURE);
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

        generate_expression(generator, binary_expression->right_hand_side);
        generate_expression(generator, binary_expression->left_hand_side);
        
        pop(generator, "rax");
        pop(generator, "rbx");

        if (binary_expression_node->binary_expression_type == NODE_BINARY_MULTIPLICATION) {
            strcat(generator->buffer, "    mul rbx\n");
        } else if (binary_expression_node->binary_expression_type == NODE_BINARY_DIVISION) {
            strcat(generator->buffer, "    div rbx\n");
        } else if (binary_expression_node->binary_expression_type == NODE_BINARY_ADDITION) {
            strcat(generator->buffer, "    add rax, rbx\n");
        } else if (binary_expression_node->binary_expression_type == NODE_BINARY_SUBTRACTION) {
            strcat(generator->buffer, "    sub rax, rbx\n");
        }

        push(generator, "rax");
        strcat(generator->buffer, "\n");

        free(binary_expression);
        free(binary_expression_node);
    } else {
        fprintf(stderr, "Invalid NodeExpressionType\n");
        exit(EXIT_FAILURE);
    }

    free(expression);
}

// generate scope
void generate_scope(Generator *generator, NodeScope *scope) {
    begin_scope(generator);
    size_t index = 0;

    while (TRUE) {
        if (index >= scope->scope_size) {
            break;
        }

        NodeStatement *statement = scope->statements[index++];

        generate_statement(generator, statement);

        free(statement);
    }

    end_scope(generator);
}

// generate if predicate
void generate_if_predicate(Generator *generator, NodeIfPredicate *if_predicate, char *end_label) {
    if (if_predicate->predicate_type == NODE_IF_PREDICATE_ELIF) {
        strcat(generator->buffer, "    ; elif\n");

        NodeIfPredicateElif *predicate = (NodeIfPredicateElif *)if_predicate->if_predicate;

        generate_expression(generator, predicate->expression);

        pop(generator, "rax");

        char *label = create_label(generator);

        strcat(generator->buffer, "    test rax, rax\n");
        strcat(generator->buffer, "    jz ");
        if (predicate->if_predicate != NULL) {
            strcat(generator->buffer, label);
        } else {
            strcat(generator->buffer, end_label);
        }
        
        strcat(generator->buffer, "\n\n");

        generate_scope(generator, predicate->scope);

        free(predicate->scope);

        strcat(generator->buffer, "    jmp ");
        strcat(generator->buffer, end_label);
        strcat(generator->buffer, "\n");

        if (predicate->if_predicate != NULL) {
            strcat(generator->buffer, "\n");
            strcat(generator->buffer, label);
            strcat(generator->buffer, ":\n");

            free(label);

            generate_if_predicate(generator, predicate->if_predicate, end_label);

            free(predicate->if_predicate);
        } else {
            strcat(generator->buffer, "\n");
        }

        free(predicate);
    } else if (if_predicate->predicate_type == NODE_IF_PREDICATE_ELSE) {
        strcat(generator->buffer, "    ; else\n");

        NodeIfPredicateElse *predciate = (NodeIfPredicateElse *)if_predicate->if_predicate;

        generate_scope(generator, predciate->scope);

        free(predciate->scope);
        free(predciate);
    } else {
        fprintf(stderr, "Invalid NodeIfPredicateType\n");
        exit(EXIT_FAILURE);
    }
}

// generate statement
void generate_statement(Generator *generator, NodeStatement *statement) {
    if (statement->statement_type == NODE_STATEMENT_EXIT) {
        strcat(generator->buffer, "    ; exit\n");

        NodeStatementExit *exit_statement = (NodeStatementExit *)statement->statement;
        NodeExpression *expression = (NodeExpression *)exit_statement->expression;

        generate_expression(generator, expression); // evaulate expression

        free(exit_statement);

        strcat(generator->buffer, "    mov rax, 60\n"); // 60 -> exit syscall
        pop(generator, "rdi"); // pop expression value into rdi
        strcat(generator->buffer, "    syscall\n\n");
    } else if (statement->statement_type == NODE_STATEMENT_LET) {
        NodeStatementDeclaration *declaration_statement = (NodeStatementDeclaration *)statement->statement;
        Token *identifier = (Token *)declaration_statement->identifier;
        
        Variable *temp = find_variable(generator, (char *)identifier->value);

        // identifier already declared
        if (temp != NULL) {
            fprintf(stderr, "Identifier: %s already declared before attempted re-declaration on line %zu\n", (char *)identifier->value, (size_t)identifier->line_number);
            exit(EXIT_FAILURE);
        }

        strcat(generator->buffer, "    ; let declaration of variable: ");
        strcat(generator->buffer, (char *)identifier->value);
        strcat(generator->buffer, "\n");

        Variable *variable = create_variable((char *)identifier->value, NODE_STATEMENT_FIELD_LET, generator->stack_size);

        generator->variables[generator->variable_count++] = variable;

        generate_expression(generator, declaration_statement->expression); // evaulate expression

        free(identifier->value);
        free(declaration_statement);
    } else if (statement->statement_type == NODE_STATEMENT_CONST) {
        NodeStatementDeclaration *declaration_statement = (NodeStatementDeclaration *)statement->statement;
        Token *identifier = (Token *)declaration_statement->identifier;
        
        Variable *temp = find_variable(generator, (char *)identifier->value);

        // identifier already declared
        if (temp != NULL) {
            fprintf(stderr, "Identifier: %s already declared before attempted re-declaration on line %zu\n", (char *)identifier->value, (size_t)identifier->line_number);
            exit(EXIT_FAILURE);
        }

        strcat(generator->buffer, "    ; const declaration of variable: ");
        strcat(generator->buffer, (char *)identifier->value);
        strcat(generator->buffer, "\n");

        Variable *variable = create_variable((char *)identifier->value, NODE_STATEMENT_FIELD_CONST, generator->stack_size);

        generator->variables[generator->variable_count++] = variable;

        generate_expression(generator, declaration_statement->expression); // evaulate expression

        free(identifier->value);
        free(declaration_statement);
    } else if (statement->statement_type == NODE_STATEMENT_ASSIGNMENT) {
        NodeStatementAssignment *assignment_statement = (NodeStatementAssignment *)statement->statement;
        Token *identifier = (Token *)assignment_statement->identifier;

        Variable *temp = find_variable(generator, (char *)identifier->value);

        // undeclared identifier
        if (temp == NULL) {
            fprintf(stderr, "Undeclared identifier: %s on line %zu\n", (char *)identifier->value, (size_t)identifier->line_number);
            exit(EXIT_FAILURE);
        }

        // attempt to reassign a const declaration
        if (temp->declaration_type == NODE_STATEMENT_FIELD_CONST) {
            fprintf(stderr, "Attempt to reassign a const declaration: %s on line %zu\n", (char *)identifier->value, (size_t)identifier->line_number);
        }

        strcat(generator->buffer, "    ; reassignment of variable: ");
        strcat(generator->buffer, (char *)identifier->value);
        strcat(generator->buffer, "\n");

        generate_expression(generator, assignment_statement->expression); // evaluate expression

        pop(generator, "rax");

        char count[BUFFER_CAPACITY];
        memset(count, 0, BUFFER_CAPACITY);
        snprintf(count, sizeof(count), "%zu", (generator->stack_size - temp->stack_location - 1) * 8); // 8 bytes -> 64 bits

        strcat(generator->buffer, "    mov [rsp + ");
        strcat(generator->buffer, count);
        strcat(generator->buffer, "], rax\n\n");

        free(identifier->value);
        free(assignment_statement);
    } else if (statement->statement_type == NODE_STATEMENT_SCOPE) {
        strcat(generator->buffer, "    ; begin scope\n");
        NodeScope *scope = (NodeScope *)statement->statement;

        generate_scope(generator, scope);

        strcat(generator->buffer, "    ; end scope\n");
        strcat(generator->buffer, "\n");

        free(scope);
    } else if (statement->statement_type == NODE_STATEMENT_IF) {
        strcat(generator->buffer, "    ; if\n");
        NodeStatementIf *if_statement = (NodeStatementIf *)statement->statement;

        generate_expression(generator, if_statement->expression);

        pop(generator, "rax");

        char *label = create_label(generator);

        strcat(generator->buffer, "    test rax, rax\n");
        strcat(generator->buffer, "    jz ");
        strcat(generator->buffer, label);
        strcat(generator->buffer, "\n\n");

        generate_scope(generator, if_statement->scope);

        free(if_statement->scope);

        if (if_statement->if_predicate != NULL) {
            char *end_label = create_label(generator);

            strcat(generator->buffer, "    jmp ");
            strcat(generator->buffer, end_label);
            strcat(generator->buffer, "\n\n");

            strcat(generator->buffer, label);
            strcat(generator->buffer, ":\n");

            generate_if_predicate(generator, if_statement->if_predicate, end_label);

            strcat(generator->buffer, end_label);
            strcat(generator->buffer, ":\n");

            free(end_label);
            free(if_statement->if_predicate);
        } else {
            strcat(generator->buffer, label);
            strcat(generator->buffer, ":\n");
        }

        free(label);
        free(if_statement);
    } else {
        fprintf(stderr, "Invalid NodeStatementType\n");
        exit(EXIT_FAILURE);
    }
}

// generate x86-64 assembly from input program
void generate_program(Generator *generator) {
    // default x86-64 boiler plate code
    strcat(generator->buffer, "section .data\n");
    strcat(generator->buffer, "FALSE equ 0\n\n");
    strcat(generator->buffer, "section .text\n");
    strcat(generator->buffer, "global _start\n\n_start:\n");

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
    size_t pop_count = generator->variable_count - generator->scopes[--generator->scope_count];

    if (pop_count != 0) {
        char count[BUFFER_CAPACITY];
        memset(count, 0, BUFFER_CAPACITY);
        snprintf(count, sizeof(count), "%zu", pop_count * 8); // 8 bytes -> 64 bits

        strcat(generator->buffer, "    add rsp, ");
        strcat(generator->buffer, count);
        strcat(generator->buffer, "\n");
    }

    generator->stack_size -= pop_count;

    for (size_t i = 0; i < pop_count; i++) {
        free(generator->variables[--generator->variable_count]->identifier);
        free(generator->variables[generator->variable_count]);
    }
}

void free_generator(Generator *generator) {
    for (size_t i = 0; i < generator->variable_count; i++) {
        free(generator->variables[i]->identifier);
        free(generator->variables[i]);
    }

    free(generator);
}
