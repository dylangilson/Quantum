/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 30, 2023
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

// create parser
Parser *create_parser(LinkedList *tokens) {
    Parser *parser = (Parser *)malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->index = 0;

    return parser;
}

// create expression node
NodeExpression *create_node_expression(Token token) {
    NodeExpression *expression = (NodeExpression *)malloc(sizeof(NodeExpression));
    expression->integer_literal = token;

    return expression;
}

// create exit node
NodeExit *create_node_exit(NodeExpression expression) {
    NodeExit *exit = (NodeExit *)malloc(sizeof(NodeExit));
    exit->expression = expression;

    return exit;
}

// peek at Token that is count indices ahead of current index
Token *peek_parser(Parser parser) {
    if (parser.index > parser.tokens->count) {
        return NULL; // index out of bounds -> no token to return
    } else {
        return &peek_at(*parser.tokens, parser.index)->token;
    }
}

// consume Token at current index of parser
Token *consume_parser(Parser *parser) {
    return &peek_at(*parser->tokens, parser->index++)->token;
}

// parse expression
NodeExpression *parse_expression(Parser *parser) {
    if (peek_parser(*parser) != NULL && peek_parser(*parser)->token_type == INTEGER_LITERAL) {
        return create_node_expression(*consume_parser(parser));
    }

    return NULL;
}

// parse tree
NodeExit *parse_tree(Parser *parser) {
    NodeExit *exit_node;

    while (peek_parser(*parser) != NULL) {
        if (peek_parser(*parser)->token_type == EXIT) {
            consume_parser(parser);

            NodeExpression *expression = parse_expression(parser);

            if (expression != NULL) {
                exit_node = create_node_exit(*expression);
                free(expression);
            } else {
                fprintf(stderr, "Invalid expression\n");
                exit(EXIT_FAILURE);
            }

            if (peek_parser(*parser) != NULL && peek_parser(*parser)->token_type == SEMICOLON) {
                consume_parser(parser);
            } else {
                fprintf(stderr, "Invalid expression\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    parser->index = 0;

    return exit_node; 
}
