/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 30, 2023
 */

#pragma once

#include "linked_list.h"

typedef struct NodeExpression {
    Token integer_literal;
} NodeExpression;

typedef struct NodeExit {
    NodeExpression expression;
} NodeExit;

typedef struct Parser {
    LinkedList *tokens;
    int index;
} Parser;

Parser *create_parser(LinkedList *list);
NodeExpression *create_node_expression(Token token);
NodeExit *create_node_exit(NodeExpression expression);
Token *peek_parser(Parser parser);
Token *consume_parser(Parser *parser);
NodeExpression *parse_expression(Parser *parser);
NodeExit *parse_tree(Parser *parser);
