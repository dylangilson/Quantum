/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 30, 2023
 */

#pragma once

#include "linked_list.h"

/*** Term Nodes ***/ 

typedef struct NodeTermIntegerLiteral {
    Token *integer_literal;
} NodeTermIntegerLiteral;

typedef struct NodeTermIdentifier {
    Token *identifier;
} NodeTermIdentifier;

typedef enum NodeTermType {
    NODE_TERM_INTEGER_LITERAL = INTEGER_LITERAL,
    NODE_TERM_IDENTIFIER = IDENTIFIER,
} NodeTermType;

typedef struct NodeTerm {
    void *value;
    NodeTermType term_type;
} NodeTerm;

/*** Expression Nodes ***/

typedef enum NodeExpressionType {
    NODE_EXPRESSION_TERM,
    NODE_EXPRESSION_BINARY
} NodeExpressionType;

typedef struct NodeExpression {
    void *expression;
    NodeExpressionType expression_type;
} NodeExpression;

typedef struct BinaryExpression {
    NodeExpression *left_hand_side;
    NodeExpression *right_hand_side;
} BinaryExpression;

typedef enum NodeBinaryExpressionType {
    NODE_BINARY_MULTIPLICATION,
    NODE_BINARY_DIVISION,
    NODE_BINARY_ADDITION,
    NODE_BINARY_SUBTRACTION
} NodeBinaryExpressionType;

typedef struct NodeBinaryExpression {
    BinaryExpression *expression;
    NodeBinaryExpressionType type;
} NodeBinaryExpression;

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

/*** Program Node ***/

typedef struct NodeProgram {
    NodeStatement *statements[PROGRAM_CAPACITY];
    size_t program_length;
} NodeProgram;

typedef struct Parser {
    LinkedList *tokens;
    size_t index;
} Parser;

Parser *create_parser(LinkedList *list);
NodeTermIntegerLiteral *create_node_term_integer_literal(Token *token);
NodeTermIdentifier *create_node_term_identifier(Token *token);
NodeTerm *create_node_term(Token *token, NodeTermType type);
BinaryExpression *create_binary_expression(NodeExpression *left_hand_side, NodeExpression *right_hand_side);
NodeBinaryExpression *create_node_binary_expression(BinaryExpression *expression, NodeBinaryExpressionType type);
NodeStatementExit *create_node_statement_exit(NodeExpression *expression);
NodeStatementLet *create_node_statement_let(Token *identifier, NodeExpression *expression);
NodeStatement *create_node_statement(NodeExpression *expression, Token *identifier, NodeStatementType type);
Token *peek_parser(Parser parser, size_t offset);
Token *consume_parser(Parser *parser);
NodeTerm *parse_term(Parser *parser);
NodeExpression *parse_expression(Parser *parser);
NodeBinaryExpression *parse_binary_expression(Parser *parser);
NodeProgram *parse_program(Parser *parser);
