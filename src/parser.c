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

// create program
NodeProgram *create_program() {
    NodeProgram *program = (NodeProgram *)malloc(sizeof(NodeProgram));
    program->program_length = 0;

    return program;
}

// create integer literal expression node
NodeExpressionIntegerLiteral *create_node_expression_integer_literal(Token *token) {
    NodeExpressionIntegerLiteral *expression = (NodeExpressionIntegerLiteral *)malloc(sizeof(NodeExpressionIntegerLiteral));
    expression->integer_literal = token;

    return expression;
}

// create identifier expression node
NodeExpressionIdentifier *create_node_expression_identifier(Token *token) {
    NodeExpressionIdentifier *expression = (NodeExpressionIdentifier *)malloc(sizeof(NodeExpressionIdentifier));
    expression->identifier = token;

    return expression;
}

// create expression node
NodeExpression *create_node_expression(Token *token, NodeExpressionType type) {
    NodeExpression *expression = (NodeExpression *)malloc(sizeof(NodeExpression));

    if (type == NODE_EXPRESSION_INTEGER_LITERAL) {
        expression->expression = create_node_expression_integer_literal(token);
    } else if (type == NODE_EXPRESSION_IDENTIFIER) {
        expression->expression = create_node_expression_identifier(token);
    } else {
        fprintf(stderr, "Invalid NodeExpressionType\n");
        exit(EXIT_FAILURE);
    }

    expression->expression_type = type;

    return expression;
}

// create exit statement node
NodeStatementExit *create_node_statement_exit(NodeExpression *expression) {
    NodeStatementExit *statement_exit = (NodeStatementExit *)malloc(sizeof(NodeStatementExit));
    statement_exit->expression = expression;

    return statement_exit;
}

// create let statement node
NodeStatementLet *create_node_statement_let(Token *token, NodeExpression *expression) {
    NodeStatementLet *statement_let = (NodeStatementLet *)malloc(sizeof(NodeStatementLet));
    statement_let->identifier = token;
    statement_let->expression = expression;

    return statement_let;
}

// create statement node
NodeStatement *create_node_statement(NodeExpression *expression, Token *identifier, NodeStatementType type) {
    NodeStatement *statement = (NodeStatement *)malloc(sizeof(NodeStatement));
    
    if (type == NODE_STATEMENT_EXIT) {
        statement->statement = create_node_statement_exit(expression);
    } else if (type == NODE_STATEMENT_LET) {
        statement->statement = create_node_statement_let(identifier, expression);
    } else {
        fprintf(stderr, "Invalid NodeStatementType\n");
        exit(EXIT_FAILURE);
    }

    statement->statement_type = type;

    return statement;
}

// peek at token that is count indices ahead of current index
Token *peek_parser(Parser parser, int offset) {
    if (parser.index + offset >= parser.tokens->count) {
        return NULL; // index out of bounds -> no token to return
    }

    return &peek_at(*parser.tokens, parser.index + offset)->token;
}

// consume token at current index of parser
Token *consume_parser(Parser *parser) {
    return &peek_at(*parser->tokens, parser->index++)->token;
}

// parse expression
NodeExpression *parse_expression(Parser *parser) {
    if (peek_parser(*parser, 0) == NULL) {
        return NULL;
    }

    if (peek_parser(*parser, 0)->token_type == INTEGER_LITERAL) {
        return create_node_expression(consume_parser(parser), NODE_EXPRESSION_INTEGER_LITERAL);
    } else if (peek_parser(*parser, 0)->token_type == IDENTIFIER) {
        return create_node_expression(consume_parser(parser), NODE_EXPRESSION_IDENTIFIER);
    } else {
        fprintf(stderr, "Invalid expression\n");
        exit(EXIT_FAILURE);
    }
}

// parse statement
NodeStatement *parse_statement(Parser *parser) {
    if (peek_parser(*parser, 0) == NULL) {
        return NULL;
    }

    NodeStatement *statement;

    if (peek_parser(*parser, 1) != NULL && peek_parser(*parser, 1)->token_type == OPEN_PARENTHESIS) {
            consume_parser(parser); // consume exit
            consume_parser(parser); // consume '('

            NodeExpression *expression = parse_expression(parser);

            if (expression == NULL) {
                fprintf(stderr, "Invalid expression\n");
                exit(EXIT_FAILURE);
            }

            statement = create_node_statement(expression, NULL, NODE_STATEMENT_EXIT);

            if (peek_parser(*parser, 0) == NULL || peek_parser(*parser, 0)->token_type != CLOSE_PARENTHESIS) {
                fprintf(stderr, "Expected ')'\n");
                exit(EXIT_FAILURE);
            }

            consume_parser(parser); // consume ')'

            if (peek_parser(*parser, 0) == NULL || peek_parser(*parser, 0)->token_type != SEMICOLON) {
                fprintf(stderr, "Expected ';'\n");
                exit(EXIT_FAILURE);
            }

            consume_parser(parser); // consume ';'
    } else if (peek_parser(*parser, 0) != NULL && peek_parser(*parser, 0)->token_type == LET) {
        if (peek_parser(*parser, 1) == NULL || peek_parser(*parser, 1)->token_type != IDENTIFIER) {
            fprintf(stderr, "Expected identifier\n");
            exit(EXIT_FAILURE);
        }

        if (peek_parser(*parser, 2) == NULL || peek_parser(*parser, 2)->token_type != EQUALS) {
            fprintf(stderr, "Expected '='\n");
            exit(EXIT_FAILURE);
        }

        consume_parser(parser); // consume let
        Token *identifier = consume_parser(parser); // consume identifier
        consume_parser(parser); // consume '='

        NodeExpression *expression = parse_expression(parser);

        if (expression == NULL) {
            fprintf(stderr, "Invalid expression\n");
            exit(EXIT_FAILURE);
        }

        if (peek_parser(*parser, 0) == NULL || peek_parser(*parser, 0)->token_type != SEMICOLON) {
            fprintf(stderr, "Expected ';'\n");
            exit(EXIT_FAILURE);
        }

        consume_parser(parser); // consume ';'

        statement = create_node_statement(expression, identifier, NODE_STATEMENT_LET);
    } else {
        fprintf(stderr, "Invalid statement\n");
        exit(EXIT_FAILURE);
    }

    return statement;
}

// parse program
NodeProgram *parse_program(Parser *parser) {
    NodeProgram *program = create_program();

    while (peek_parser(*parser, 0) != NULL) {
        NodeStatement *statement = parse_statement(parser);

        if (statement == NULL) {
            fprintf(stderr, "Invalid statement\n");
            exit(EXIT_FAILURE);
        }

        program->statements[program->program_length++] = statement;
    }

    return program;
}
