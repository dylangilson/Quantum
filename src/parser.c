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
NodeTermIntegerLiteral *create_node_term_integer_literal(Token *token) {
    NodeTermIntegerLiteral *term = (NodeTermIntegerLiteral *)malloc(sizeof(NodeTermIntegerLiteral));
    term->integer_literal = token;

    return term;
}

// create identifier expression node
NodeTermIdentifier *create_node_term_identifier(Token *token) {
    NodeTermIdentifier *term = (NodeTermIdentifier *)malloc(sizeof(NodeTermIdentifier));
    term->identifier = token;

    return term;
}

// create term node
NodeTerm *create_node_term(Token *token, NodeTermType type) {
    NodeTerm *term = (NodeTerm *)malloc(sizeof(NodeTerm));

    if (type == NODE_TERM_INTEGER_LITERAL) {
        term->value = create_node_term_integer_literal(token);
    } else if (type == NODE_TERM_IDENTIFIER) {
        term->value = create_node_term_identifier(token);
    } else {
        fprintf(stderr, "Invalid NodeTermType\n");
        exit(EXIT_FAILURE);
    }

    term->term_type = type;

    return term;
}

// create binary expression
BinaryExpression *create_binary_expression(NodeExpression *left_hand_side, NodeExpression *right_hand_side) {
    BinaryExpression *binary_expression = (BinaryExpression *)malloc(sizeof(BinaryExpression));
    binary_expression->left_hand_side = left_hand_side;
    binary_expression->right_hand_side = right_hand_side;

    return binary_expression;
}

// create binary expresion node
NodeBinaryExpression *create_node_binary_expression(BinaryExpression *expression, NodeBinaryExpressionType type) {
    NodeBinaryExpression *binary_expression = (NodeBinaryExpression *)malloc(sizeof(NodeBinaryExpression));
    binary_expression->expression = expression;
    binary_expression->type = type;

    return binary_expression;
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
Token *peek_parser(Parser parser, size_t offset) {
    if (parser.index + offset >= parser.tokens->count) {
        return NULL; // index out of bounds -> no token to return
    }

    return peek_at(*parser.tokens, parser.index + offset)->token;
}

// consume token at current index of parser
Token *consume_parser(Parser *parser) {
    return peek_at(*parser->tokens, parser->index++)->token;
}

// parse term
NodeTerm *parse_term(Parser *parser) {
    if (peek_parser(*parser, 0)->token_type == INTEGER_LITERAL) {
        return create_node_term(consume_parser(parser), NODE_TERM_INTEGER_LITERAL);
    } else if (peek_parser(*parser, 0)->token_type == IDENTIFIER) {
        return create_node_term(consume_parser(parser), NODE_TERM_IDENTIFIER);
    } else {
        fprintf(stderr, "Invalid term\n");
        exit(EXIT_FAILURE);
    }
}

// parse expression
NodeExpression *parse_expression(Parser *parser) {
    if (peek_parser(*parser, 0) == NULL) {
        return NULL;
    }

    NodeTerm *term = parse_term(parser);

    if (term == NULL) {
        return NULL;
    }

    if (peek_parser(*parser, 0) != NULL && peek_parser(*parser, 0)->token_type == PLUS) {
        NodeExpression *left_hand_side = (NodeExpression *)malloc(sizeof(NodeExpression));
        left_hand_side->expression = term;
        left_hand_side->expression_type = NODE_EXPRESSION_TERM;

        consume_parser(parser); // consume '+'

        NodeExpression *right_hand_side = parse_expression(parser);

        if (right_hand_side == NULL) {
            fprintf(stderr, "Expected expression\n");
            exit(EXIT_FAILURE);
        }

        BinaryExpression *binary_expression = create_binary_expression(left_hand_side, right_hand_side);
        NodeBinaryExpression *binary_expression_node = create_node_binary_expression(binary_expression, NODE_BINARY_ADDITION);

        NodeExpression *expression = (NodeExpression *)malloc(sizeof(NodeExpression));
        expression->expression = binary_expression_node;
        expression->expression_type = NODE_EXPRESSION_BINARY;

        return expression;
    } else {
        NodeExpression *expression = (NodeExpression *)malloc(sizeof(NodeExpression));
        expression->expression = term;
        expression->expression_type = NODE_EXPRESSION_TERM;
        
        return expression;
    }
}

// parse binary expresion
NodeBinaryExpression *parse_binary_expression(Parser *parser) {
    NodeExpression *left_hand_side = parse_expression(parser);

    if (left_hand_side == NULL) {
        return NULL;
    }

    if (peek_parser(*parser, 0) != NULL && peek_parser(*parser, 0)->token_type == PLUS) {
        consume_parser(parser); // consume '+'

        NodeExpression *right_hand_side = parse_expression(parser);

        if (right_hand_side == NULL) {
            fprintf(stderr, "Expected expression\n");
            exit(EXIT_FAILURE);
        }

        BinaryExpression *binary_expression = create_binary_expression(left_hand_side, right_hand_side);
        NodeBinaryExpression *binary_expression_node = create_node_binary_expression(binary_expression, NODE_BINARY_ADDITION);

        return binary_expression_node;
    } else {
        fprintf(stderr, "Unsupported binary operator\n");
        exit(EXIT_FAILURE);
    }

    return NULL;
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
        consume_parser(parser); // consume let

        if (peek_parser(*parser, 0) == NULL || peek_parser(*parser, 0)->token_type != IDENTIFIER) {
            fprintf(stderr, "Expected identifier\n");
            exit(EXIT_FAILURE);
        }

        Token *identifier = consume_parser(parser); // consume identifier

        if (peek_parser(*parser, 0) == NULL || peek_parser(*parser, 0)->token_type != EQUALS) {
            fprintf(stderr, "Expected '='\n");
            exit(EXIT_FAILURE);
        }
        
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
