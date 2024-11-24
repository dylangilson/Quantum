/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 30, 2023
 */

#pragma once

#include "linked_list.h"

#define MAX_PRECEDENCE 100

typedef enum NodeTermType {
    NODE_TERM_INTEGER_LITERAL = INTEGER_LITERAL,
    NODE_TERM_IDENTIFIER = IDENTIFIER,
    NODE_TERM_PARENTHESIS = OPEN_PARENTHESIS
} NodeTermType;

typedef struct NodeTermParenthesis {
    struct NodeExpression *expression;
} NodeTermParenthesis;

typedef struct NodeTerm {
    void *value;
    NodeTermType term_type;
} NodeTerm;

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

typedef struct NodeStatementExit {
    NodeExpression *expression;
} NodeStatementExit;

typedef struct NodeStatementLet {
    Token *identifier;
    NodeExpression *expression;
} NodeStatementLet;

typedef enum NodeStatementType {
    NODE_STATEMENT_EXIT = EXIT,
    NODE_STATEMENT_LET = LET,
    NODE_STATEMENT_SCOPE = OPEN_BRACKET,
    NODE_STATEMENT_IF = IF
} NodeStatementType;

typedef struct NodeStatement {
    void *statement;
    NodeStatementType statement_type;
} NodeStatement;

typedef struct NodeScope {
    NodeStatement *statements[PROGRAM_CAPACITY];
    size_t scope_size;
} NodeScope;

typedef enum NodeIfPredicateType {
    NODE_IF_PREDICATE_ELIF = ELIF,
    NODE_IF_PREDICATE_ELSE = ELSE
} NodeIfPredicateType;

typedef struct NodeIfPredicate {
    void *if_predicate;
    NodeIfPredicateType predicate_type;
} NodeIfPredicate;

typedef struct NodeIfPredicateElif {
    NodeExpression *expression;
    NodeScope *scope;
    NodeIfPredicate *if_predicate;
} NodeIfPredicateElif;

typedef struct NodeIfPredicateElse {
    NodeScope *scope;
} NodeIfPredicateElse;

typedef struct NodeStatementIf {
    NodeExpression *expression;
    NodeScope *scope;
    NodeIfPredicate *if_predicate;
} NodeStatementIf;

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
NodeProgram *create_program();
NodeTermParenthesis *create_node_term_parenthesis(NodeExpression *expression);
NodeTerm *create_node_term(void *value, NodeTermType type);
NodeScope *create_scope();
int get_binary_precedence(TokenType type);
BinaryExpression *create_binary_expression(NodeExpression *left_hand_side, NodeExpression *right_hand_side);
NodeBinaryExpression *create_node_binary_expression(BinaryExpression *expression, NodeBinaryExpressionType type);
NodeStatementExit *create_node_statement_exit(NodeExpression *expression);
NodeStatementLet *create_node_statement_let(Token *identifier, NodeExpression *expression);
NodeStatementIf *create_node_statement_if(NodeExpression *expression, NodeScope *scope);
NodeStatement *create_node_statement(NodeExpression *expression, Token *identifier, NodeStatementType type);
Token *peek_parser(Parser parser, size_t offset);
Token *consume_parser(Parser *parser);
NodeTerm *parse_term(Parser *parser);
NodeExpression *parse_expression(Parser *parser, int min_precedence);
NodeScope *parse_scope(Parser *parser);
NodeIfPredicate *parse_if_predicate(Parser *parser);
NodeStatement *parse_statement(Parser *parser);
NodeProgram *parse_program(Parser *parser);
