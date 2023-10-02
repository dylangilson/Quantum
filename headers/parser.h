/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 30, 2023
 */

#pragma once

#include "linked_list.h"

#define PROGRAM_CAPACITY 512

/*** Expression Nodes ***/ 

typedef struct NodeExpressionIntegerLiteral {
    Token *integer_literal;
} NodeExpressionIntegerLiteral;

typedef struct NodeExpressionIdentifier {
    Token *identifier;
} NodeExpressionIdentifier;

typedef enum NodeExpressionType {
    NODE_EXPRESSION_INTEGER_LITERAL = INTEGER_LITERAL,
    NODE_EXPRESSION_IDENTIFIER = IDENTIFIER
} NodeExpressionType;

typedef struct NodeExpression {
    void *expression;
    NodeExpressionType expression_type;
} NodeExpression;

/*** Statement Nodes ***/

typedef struct NodeStatementExit {
    NodeExpression *expression;
} NodeStatementExit;

typedef struct NodeStatementLet {
    Token *identifier;
    NodeExpression *expression;
} NodeStatementLet;

typedef enum NodeStatementType {
    NODE_STATEMENT_EXIT = EXIT,
    NODE_STATEMENT_LET = LET
} NodeStatementType;

typedef struct NodeStatement {
    void *statement;
    NodeStatementType statement_type;
} NodeStatement;

typedef struct NodeProgram {
    NodeStatement *statements[PROGRAM_CAPACITY];
    int program_length;
} NodeProgram;

typedef struct Parser {
    LinkedList *tokens;
    int index;
} Parser;

Parser *create_parser(LinkedList *list);
NodeExpressionIntegerLiteral *create_node_expression_integer_literal(Token *token);
NodeExpressionIdentifier *create_node_expression_identifier(Token *token);
NodeExpression *create_node_expression(Token *token, NodeExpressionType type);
NodeStatementExit *create_node_statement_exit(NodeExpression *expression);
NodeStatementLet *create_node_statement_let(Token *identifier, NodeExpression *expression);
NodeStatement *create_node_statement(NodeExpression *expression, Token *identifier, NodeStatementType type);
Token *peek_parser(Parser parser, int offset);
Token *consume_parser(Parser *parser);
NodeExpression *parse_expression(Parser *parser);
NodeProgram *parse_program(Parser *parser);
