/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 30, 2023
 */

#include <stdio.h>
#include <stdlib.h>

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

// create parenthesis term node
NodeTermParenthesis *create_node_term_parenthesis(NodeExpression *expression) {
    NodeTermParenthesis *parenthesis_term = (NodeTermParenthesis *)malloc(sizeof(NodeTermParenthesis));
    parenthesis_term->expression = expression;

    return parenthesis_term;
}

// create term node
NodeTerm *create_node_term(void *value, NodeTermType term_type) {
    NodeTerm *term = (NodeTerm *)malloc(sizeof(NodeTerm));
    term->value = value;
    term->term_type = term_type;

    return term;
}

// create scope
NodeScope *create_scope() {
    NodeScope *scope = (NodeScope *)malloc(sizeof(NodeScope));
    scope->scope_size = 0;

    return scope;
}

// get binary precedence from token type
int get_binary_precedence(const TokenType token_type) {
    switch (token_type) {
        case STAR:
        case FORWARD_SLASH:
            return 1;
        case PLUS:
        case MINUS:
            return 0;
        default:
            return MAX_PRECEDENCE;
    }
}

// create binary expression
BinaryExpression *create_binary_expression(NodeExpression *left_hand_side, NodeExpression *right_hand_side) {
    BinaryExpression *binary_expression = (BinaryExpression *)malloc(sizeof(BinaryExpression));
    binary_expression->left_hand_side = left_hand_side;
    binary_expression->right_hand_side = right_hand_side;

    return binary_expression;
}

// create binary expresion node
NodeBinaryExpression *create_node_binary_expression(BinaryExpression *expression, NodeBinaryExpressionType binary_expression_type) {
    NodeBinaryExpression *binary_expression = (NodeBinaryExpression *)malloc(sizeof(NodeBinaryExpression));
    binary_expression->expression = expression;
    binary_expression->binary_expression_type = binary_expression_type;

    return binary_expression;
}

// create exit statement node
NodeStatementExit *create_node_statement_exit(NodeExpression *expression) {
    NodeStatementExit *exit_statement = (NodeStatementExit *)malloc(sizeof(NodeStatementExit));
    exit_statement->expression = expression;

    return exit_statement;
}

// create let statement node
NodeStatementLet *create_node_statement_let(Token *token, NodeExpression *expression) {
    NodeStatementLet *if_statement = (NodeStatementLet *)malloc(sizeof(NodeStatementLet));
    if_statement->identifier = token;
    if_statement->expression = expression;

    return if_statement;
}



// create if statement node
NodeStatementIf *create_node_statement_if(NodeExpression *expression, NodeScope *scope) {
    NodeStatementIf *if_statement = (NodeStatementIf *)malloc(sizeof(NodeStatementIf));
    if_statement->expression = expression;
    if_statement->scope = scope;

    return if_statement;
}

// create assignment statement node
NodeStatementAssignment *create_node_statement_assignment(Token *token, NodeExpression *expression) {
    NodeStatementAssignment *assignment = (NodeStatementAssignment *)malloc(sizeof(NodeStatementAssignment));
    assignment->identifier = token;
    assignment->expression = expression;

    return assignment;
}

// create statement node
NodeStatement *create_node_statement(Parser *parser, NodeExpression *expression, Token *identifier, NodeScope *scope, NodeStatementType statement_type) {
    NodeStatement *statement = (NodeStatement *)malloc(sizeof(NodeStatement));
    
    if (statement_type == NODE_STATEMENT_EXIT) {
        statement->statement = create_node_statement_exit(expression);
    } else if (statement_type == NODE_STATEMENT_LET) {
        statement->statement = create_node_statement_let(identifier, expression);
    } else if (statement_type == NODE_STATEMENT_ASSIGNMENT) {
        statement->statement = create_node_statement_assignment(identifier, expression);
    } else if (statement_type == NODE_STATEMENT_IF) {
        NodeStatementIf *if_statement = create_node_statement_if(expression, scope);
        if_statement->if_predicate = parse_if_predicate(parser);

        statement->statement = if_statement;
    } else if (statement_type == NODE_STATEMENT_SCOPE) {
        statement->statement = scope;
    } else {
        fprintf(stderr, "Invalid NodeStatementType\n");
        exit(EXIT_FAILURE);
    }

    statement->statement_type = statement_type;

    return statement;
}

// parse term
NodeTerm *parse_term(Parser *parser) {
    if (peek_parser(*parser, 0)->token_type == INTEGER_LITERAL) {
        return create_node_term(consume_parser(parser), NODE_TERM_INTEGER_LITERAL);
    } else if (peek_parser(*parser, 0)->token_type == IDENTIFIER) {
        return create_node_term(consume_parser(parser), NODE_TERM_IDENTIFIER);
    } else if (peek_parser(*parser, 0)->token_type == OPEN_PARENTHESIS) {
        consume_parser(parser); // consume '('
        NodeExpression *expression = parse_expression(parser, 0);

        // expected expression
        if (expression == NULL) {
            parser_error(*parser, "expression");
        }

        try_consume_parser(parser, CLOSE_PARENTHESIS); // consume ')'

        NodeTermParenthesis *term_parenthesis = create_node_term_parenthesis(expression);
        NodeTerm *term = create_node_term(term_parenthesis, NODE_TERM_PARENTHESIS);

        return term;
    } else {
        fprintf(stderr, "Invalid term\n");
        exit(EXIT_FAILURE);
    }
}

// parse expression
NodeExpression *parse_expression(Parser *parser, const int min_precedence) {
    NodeTerm *term = parse_term(parser);

    if (term == NULL) {
        return NULL;
    }

    NodeExpression *left_hand_side = (NodeExpression *)malloc(sizeof(NodeExpression));
    left_hand_side->expression = term;
    left_hand_side->expression_type = NODE_EXPRESSION_TERM;

    while (TRUE) {
        Token *token = peek_parser(*parser, 0);

        if (token == NULL) {
            break;
        }

        int precedence = get_binary_precedence(token->token_type);

        if (precedence == MAX_PRECEDENCE || precedence < min_precedence) {
            break;
        }

        Token *operator = consume_parser(parser);
        int next_min_precedence = precedence + 1;
        NodeExpression *right_hand_side = parse_expression(parser, next_min_precedence);

        if (right_hand_side == NULL) {
            parser_error(*parser, "expression");
        }

        NodeExpression *left_hand_side2 = (NodeExpression *)malloc(sizeof(NodeExpression));
        left_hand_side2->expression = left_hand_side->expression;
        left_hand_side2->expression_type = left_hand_side->expression_type;

        BinaryExpression *binary_expression = create_binary_expression(left_hand_side2, right_hand_side);
        NodeBinaryExpression *binary_expression_node;

        if (operator->token_type == STAR) {
            binary_expression_node = create_node_binary_expression(binary_expression, NODE_BINARY_MULTIPLICATION);
        } else if (operator->token_type == FORWARD_SLASH) {
            binary_expression_node = create_node_binary_expression(binary_expression, NODE_BINARY_DIVISION);
        } else if (operator->token_type == PLUS) {
            binary_expression_node = create_node_binary_expression(binary_expression, NODE_BINARY_ADDITION);
        } else if (operator->token_type == MINUS) {
            binary_expression_node = create_node_binary_expression(binary_expression, NODE_BINARY_SUBTRACTION);
        }

        left_hand_side->expression = binary_expression_node;
        left_hand_side->expression_type = NODE_EXPRESSION_BINARY;
    }

    return left_hand_side;
}

// parse scope
NodeScope *parse_scope(Parser *parser) {
    if (peek_parser(*parser, 0) == NULL || peek_parser(*parser, 0)->token_type != OPEN_BRACKET) {
        return NULL;
    }

    consume_parser(parser); // consume '{'

    NodeScope *scope = create_scope();

    while (TRUE) {
        NodeStatement *statement = parse_statement(parser);

        if (statement == NULL) {
            break;
        }

        scope->statements[scope->scope_size++] = statement;
    }

    try_consume_parser(parser, CLOSE_BRACKET);

    return scope;
}

// parse if predicate
NodeIfPredicate *parse_if_predicate(Parser *parser) {
    if (peek_parser(*parser, 0) == NULL) {
        return NULL;
    }

    NodeIfPredicate *if_predicate;

    if (peek_parser(*parser, 0)->token_type == ELIF) {
        consume_parser(parser); // consume elif

        try_consume_parser(parser, OPEN_PARENTHESIS); // consume '('

        NodeIfPredicateElif *predicate = (NodeIfPredicateElif *)malloc(sizeof(NodeIfPredicateElif));
        NodeExpression *expression = parse_expression(parser, 0);

        if (expression == NULL) {
            parser_error(*parser, "expression");
        }

        predicate->expression = expression;

        try_consume_parser(parser, CLOSE_PARENTHESIS); // consume ')'

        NodeScope *scope = parse_scope(parser);

        if (scope == NULL) {
            parser_error(*parser, "scope");
        }

        predicate->scope = scope;
        predicate->if_predicate = parse_if_predicate(parser);

        if_predicate = (NodeIfPredicate *)malloc(sizeof(NodeIfPredicate));
        if_predicate->if_predicate = predicate;
        if_predicate->predicate_type = NODE_IF_PREDICATE_ELIF;
    } else if (peek_parser(*parser, 0)->token_type == ELSE) {
        consume_parser(parser); // consume else

        NodeIfPredicateElse *predicate = (NodeIfPredicateElse *)malloc(sizeof(NodeIfPredicateElse));
        NodeScope *scope = parse_scope(parser);

        if (scope == NULL) {
            parser_error(*parser, "scope");
        }

        predicate->scope = scope;

        if_predicate = (NodeIfPredicate *)malloc(sizeof(NodeIfPredicate));
        if_predicate->if_predicate = predicate;
        if_predicate->predicate_type = NODE_IF_PREDICATE_ELSE;
    } else {
        return NULL;
    }

    return if_predicate;
}

// parse statement
NodeStatement *parse_statement(Parser *parser) {
    if (peek_parser(*parser, 0) == NULL) {
        return NULL;
    }

    NodeStatement *statement;

    if (peek_parser(*parser, 0)->token_type == EXIT) {
        consume_parser(parser); // consume exit
        try_consume_parser(parser, OPEN_PARENTHESIS); // consume '('

        NodeExpression *expression = parse_expression(parser, 0);

        if (expression == NULL) {
            parser_error(*parser, "expression");
        }

        try_consume_parser(parser, CLOSE_PARENTHESIS); // consume ')'
        try_consume_parser(parser, SEMICOLON); // consume ';'

        statement = create_node_statement(parser, expression, NULL, NULL, NODE_STATEMENT_EXIT);
    } else if (peek_parser(*parser, 0)->token_type == LET) {
        consume_parser(parser); // consume let

        Token *identifier = try_consume_parser(parser, IDENTIFIER); // consume identifier
        
        try_consume_parser(parser, EQUALS); // consume '='

        NodeExpression *expression = parse_expression(parser, 0);

        if (expression == NULL) {
            parser_error(*parser, "expression");
        }

        try_consume_parser(parser, SEMICOLON); // consume ';'

        statement = create_node_statement(parser, expression, identifier, NULL, NODE_STATEMENT_LET);
    } else if (peek_parser(*parser, 0)->token_type == IDENTIFIER) {
        Token *identifier = consume_parser(parser); // consume identifier

        try_consume_parser(parser, EQUALS); // consume '='

        NodeExpression *expression = parse_expression(parser, 0);

        if (expression == NULL) {
            parser_error(*parser, "expression");
        }

        try_consume_parser(parser, SEMICOLON); // consume ';'

        statement = create_node_statement(parser, expression, identifier, NULL, NODE_STATEMENT_ASSIGNMENT);
    } else if (peek_parser(*parser, 0)->token_type == IF) {
        consume_parser(parser); // consume if
        try_consume_parser(parser, OPEN_PARENTHESIS); // consume '('
   
        NodeExpression *expression = parse_expression(parser, 0);

        if (expression == NULL) {
            parser_error(*parser, "expression");
        }

        try_consume_parser(parser, CLOSE_PARENTHESIS); // consume ')'

        NodeScope *scope = parse_scope(parser);

        if (scope == NULL) {
            parser_error(*parser, "scope");
        }

        statement = create_node_statement(parser, expression, NULL, scope, NODE_STATEMENT_IF);
    } else if (peek_parser(*parser, 0)->token_type == OPEN_BRACKET) {
        NodeScope *scope = parse_scope(parser);

        if (scope == NULL) {
            parser_error(*parser, "scope");
        }

        statement = create_node_statement(parser, NULL, NULL, scope, NODE_STATEMENT_SCOPE);
    } else {
        return NULL;
    }

    return statement;
}

// parse program
NodeProgram *parse_program(Parser *parser) {
    NodeProgram *program = create_program();

    while (peek_parser(*parser, 0) != NULL) {
        NodeStatement *statement = parse_statement(parser);

        if (statement == NULL) {
            parser_error(*parser, "statement");
        }

        program->statements[program->program_length++] = statement;
    }

    return program;
}

// convert token type to string
char *token_to_string(const TokenType token_type) {
    switch (token_type) {
        case EXIT:
            return "'exit'";
        case INTEGER_LITERAL:
            return "integer literal";
        case SEMICOLON:
            return "';'";
        case OPEN_PARENTHESIS:
            return "'('";
        case CLOSE_PARENTHESIS:
            return "')'";
        case IDENTIFIER:
            return "identifier";
        case LET:
            return "'let'";
        case EQUALS:
            return "'='";
        case PLUS:
            return "'+'";
        case STAR:
            return "'*'";
        case MINUS:
            return "'-'";
        case FORWARD_SLASH:
            return "'/'";
        case OPEN_BRACKET:
            return "'{'";
        case CLOSE_BRACKET:
            return "'}'";
        case IF:
            return "'if'";
        case ELIF:
            return "'elif'";
        case ELSE:
            return "'else'";
        default:
            return NULL;
    }
}

// peek at token that is count indices ahead of current index
Token *peek_parser(const Parser parser, const size_t offset) {
    if (parser.index + offset >= parser.tokens->count) {
        return NULL; // index out of bounds -> no token to return
    }

    return peek_at(*parser.tokens, parser.index + offset)->token;
}

// consume token at current index of parser
Token *consume_parser(Parser *parser) {
    return peek_at(*parser->tokens, parser->index++)->token;
}

// try to consume token at current index of parser, otherwise throw error
Token *try_consume_parser(Parser *parser, const TokenType token_type) {
    if (peek_parser(*parser, 0) != NULL && peek_parser(*parser, 0)->token_type == token_type) {
        return consume_parser(parser);
    }

    parser_error(*parser, token_to_string(token_type));

    return NULL;
}

// parser error on line x
void parser_error(Parser parser, const char *message) {
    fprintf(stderr, "Expected %s on line %zu\n", message, peek_parser(parser, -1)->line_number);
    exit(EXIT_FAILURE);
}
