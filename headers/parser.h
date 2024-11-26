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
    NodeBinaryExpressionType binary_expression_type;
} NodeBinaryExpression;

typedef struct NodeStatementExit {
    NodeExpression *expression;
} NodeStatementExit;

typedef enum NodeStatementDeclarationType {
    NODE_STATEMENT_FIELD_LET = LET,
    NODE_STATEMENT_FIELD_CONST = CONST
} NodeStatementDeclarationType;

typedef struct NodeStatementDeclaration {
    Token *identifier;
    NodeExpression *expression;
    NodeStatementDeclarationType declaration_type;
} NodeStatementDeclaration;

typedef enum NodeStatementType {
    NODE_STATEMENT_EXIT = EXIT,
    NODE_STATEMENT_LET = LET,
    NODE_STATEMENT_CONST = CONST,
    NODE_STATEMENT_SCOPE = OPEN_BRACKET,
    NODE_STATEMENT_IF = IF,
    NODE_STATEMENT_ASSIGNMENT
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

typedef struct NodeStatementAssignment {
    Token *identifier;
    NodeExpression *expression;
} NodeStatementAssignment;

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
NodeTerm *create_node_term(void *value, NodeTermType term_type);
NodeScope *create_scope();
int get_binary_precedence(const TokenType token_type);
BinaryExpression *create_binary_expression(NodeExpression *left_hand_side, NodeExpression *right_hand_side);
NodeBinaryExpression *create_node_binary_expression(BinaryExpression *expression, NodeBinaryExpressionType binary_expression_type);
NodeStatementExit *create_node_statement_exit(NodeExpression *expression);
NodeStatementDeclaration *create_node_statement_declaration(Token *identifier, NodeExpression *expression, NodeStatementDeclarationType declaration_type);
NodeStatementAssignment *create_node_statement_assignment(Token *token, NodeExpression *expression);
NodeStatementIf *create_node_statement_if(NodeExpression *expression, NodeScope *scope);
NodeStatement *create_node_statement(Parser *parser, NodeExpression *expression, Token *identifier, NodeScope *scope, NodeStatementType statement_type);
NodeTerm *parse_term(Parser *parser);
NodeExpression *parse_expression(Parser *parser, const int min_precedence);
NodeScope *parse_scope(Parser *parser);
NodeIfPredicate *parse_if_predicate(Parser *parser);
NodeStatement *parse_statement(Parser *parser);
NodeProgram *parse_program(Parser *parser);
char *token_to_string(const TokenType token_type);
Token *peek_parser(const Parser parser, const size_t offset);
Token *consume_parser(Parser *parser);
Token *try_consume_parser(Parser *parser, const TokenType token_type);
void parser_error(Parser parser, const char *message);
