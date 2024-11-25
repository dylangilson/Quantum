/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 30, 2023
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "tokenizer.h"

// create tokenizer
Tokenizer *create_tokenizer(const char *src) {
    Tokenizer *tokenizer = (Tokenizer *)malloc(sizeof(Tokenizer));
    tokenizer->src = src;
    tokenizer->buffer = (char *)malloc(BUFFER_CAPACITY * sizeof(char));
    memset(tokenizer->buffer, 0, BUFFER_CAPACITY); // clear buffer
    tokenizer->index = 0;

    return tokenizer;
}

// peek at character that is count indices ahead of current index
char peek_tokenizer(const Tokenizer tokenizer, const size_t offset) {
    if (tokenizer.index + offset >= strlen(tokenizer.src)) {
        return '\0'; // empty string
    }
    
    return tokenizer.src[tokenizer.index + offset];
}

// consume character at current index of tokenizer
char consume_tokenizer(Tokenizer *tokenizer) {
    return tokenizer->src[tokenizer->index++];
}

// convert string into list of tokens
LinkedList *tokenize(Tokenizer *tokenizer, LinkedList *tokens) {
    size_t line_number = 1;

    while(peek_tokenizer(*tokenizer, 0) != '\0') {
        if (isalpha(peek_tokenizer(*tokenizer, 0))) {
            char c = consume_tokenizer(tokenizer); // get character
            strncat(tokenizer->buffer, &c, 1); // append character to buffer
            
            // while character is alphanumeric -> append character to buffer
            while (peek_tokenizer(*tokenizer, 0) != '\0' && isalnum(peek_tokenizer(*tokenizer, 0))) {
                char c = consume_tokenizer(tokenizer); // get character
                strncat(tokenizer->buffer, &c, 1);
            }

            // check for key words
            if (strcmp(tokenizer->buffer, "exit") == 0) {
                push_tail(tokens, NULL, EXIT, line_number); // append exit token
                memset(tokenizer->buffer, 0, BUFFER_CAPACITY); // clear buffer
            } else if (strcmp(tokenizer->buffer, "let") == 0) {
                push_tail(tokens, NULL, LET, line_number); // append let token
                memset(tokenizer->buffer, 0, BUFFER_CAPACITY); // clear buffer
            } else if (strcmp(tokenizer->buffer, "const") == 0) {
                push_tail(tokens, NULL, CONST, line_number); // append let token
                memset(tokenizer->buffer, 0, BUFFER_CAPACITY); // clear buffer
            } else if (strcmp(tokenizer->buffer, "if") == 0) {
                push_tail(tokens, NULL, IF, line_number); // append if token
                memset(tokenizer->buffer, 0, BUFFER_CAPACITY); // clear buffer
            } else if (strcmp(tokenizer->buffer, "elif") == 0) {
                push_tail(tokens, NULL, ELIF, line_number); // append elif token
                memset(tokenizer->buffer, 0, BUFFER_CAPACITY); // clear buffer
            } else if (strcmp(tokenizer->buffer, "else") == 0) {
                push_tail(tokens, NULL, ELSE, line_number); // append else token
                memset(tokenizer->buffer, 0, BUFFER_CAPACITY); // clear buffer
            } else {
                char *identifier = (char *)malloc((strlen(tokenizer->buffer) + 1) * sizeof(char));
                strcpy(identifier, tokenizer->buffer);
                push_tail(tokens, identifier, IDENTIFIER, line_number);
                memset(tokenizer->buffer, 0, BUFFER_CAPACITY); // clear buffer
            }
        } else if (isdigit(peek_tokenizer(*tokenizer, 0))) {
            char c = consume_tokenizer(tokenizer); // get character
            strncat(tokenizer->buffer, &c, 1); // append character to buffer

            // while character is digit -> append to buffer
            while (peek_tokenizer(*tokenizer, 0) != '\0' && isdigit(peek_tokenizer(*tokenizer, 0))) {
                char c = consume_tokenizer(tokenizer); // get character
                strncat(tokenizer->buffer, &c, 1);
            }

            long temp = atoi(tokenizer->buffer); // convert string to number
            push_tail(tokens, (int *)temp, INTEGER_LITERAL, line_number); // append number to buffer
            memset(tokenizer->buffer, 0, BUFFER_CAPACITY); // clear buffer
        } else if (peek_tokenizer(*tokenizer, 0) == '/' && peek_tokenizer(*tokenizer, 1) != '\0' && peek_tokenizer(*tokenizer, 1) == '/') {
            consume_tokenizer(tokenizer); // consume '/'
            consume_tokenizer(tokenizer); // consume '/'

            while (peek_tokenizer(*tokenizer, 0) != '\0' && peek_tokenizer(*tokenizer, 0) != '\n') {
                consume_tokenizer(tokenizer); // consume comment
            }
        } else if (peek_tokenizer(*tokenizer, 0) == '/' && peek_tokenizer(*tokenizer, 1) != '\0' && peek_tokenizer(*tokenizer, 1) == '*') {
            consume_tokenizer(tokenizer); // consume '/'
            consume_tokenizer(tokenizer); // consume '*'

            while (peek_tokenizer(*tokenizer, 0) != '\0') {
                if (peek_tokenizer(*tokenizer, 0) == '*' && peek_tokenizer(*tokenizer, 1) != '\0' && peek_tokenizer(*tokenizer, 1) == '/') {
                    break;               
                }

                char c = consume_tokenizer(tokenizer); // consume comment

                if (c == '\n') {
                    line_number++;
                }
            }

            // check for end of multi-line comment
            if (peek_tokenizer(*tokenizer, 0) == feof(stdin)) {
                fprintf(stderr, "Expected '*/' on line %zu\n", line_number);
                exit(EXIT_FAILURE);
            }

            consume_tokenizer(tokenizer); // consume '*'
            consume_tokenizer(tokenizer); // consume '/'
        } else if (peek_tokenizer(*tokenizer, 0) == '(') {
            consume_tokenizer(tokenizer); // consume '('
            push_tail(tokens, NULL, OPEN_PARENTHESIS, line_number); // append open parenthesis
        } else if (peek_tokenizer(*tokenizer, 0) == ')'){
            consume_tokenizer(tokenizer); // consume ')'
            push_tail(tokens, NULL, CLOSE_PARENTHESIS, line_number); // append close parenthesis
        } else if (peek_tokenizer(*tokenizer, 0) == '='){
            consume_tokenizer(tokenizer); // consume '='
            push_tail(tokens, NULL, EQUALS, line_number); // append equals
        } else if (peek_tokenizer(*tokenizer, 0) == '+') {
            consume_tokenizer(tokenizer); // consume '+'
            push_tail(tokens, NULL, PLUS, line_number); // append plus
        } else if (peek_tokenizer(*tokenizer, 0) == '-') {
            consume_tokenizer(tokenizer); // consume '-'
            push_tail(tokens, NULL, MINUS, line_number); // append minus
        } else if (peek_tokenizer(*tokenizer, 0) == '*') {
            consume_tokenizer(tokenizer); // consume '*'
            push_tail(tokens, NULL, STAR, line_number); // append star
        } else if (peek_tokenizer(*tokenizer, 0) == '/') {
            consume_tokenizer(tokenizer); // consume '/'
            push_tail(tokens, NULL, FORWARD_SLASH, line_number); // append forward slash
        } else if (peek_tokenizer(*tokenizer, 0) == '{') {
            consume_tokenizer(tokenizer); // consume '{'
            push_tail(tokens, NULL, OPEN_BRACKET, line_number); // append open bracket
        } else if (peek_tokenizer(*tokenizer, 0) == '}') {
            consume_tokenizer(tokenizer); // consume '}'
            push_tail(tokens, NULL, CLOSE_BRACKET, line_number); // append close bracket
        } else if (peek_tokenizer(*tokenizer, 0) == ';') {
            consume_tokenizer(tokenizer); // consume ';'
            push_tail(tokens, NULL, SEMICOLON, line_number); // append semicolon
        } else if (peek_tokenizer(*tokenizer, 0) == '\n') {
            consume_tokenizer(tokenizer); // consume '\n'
            line_number++; // increment to new line
        } else if (isspace(peek_tokenizer(*tokenizer, 0))) {
            consume_tokenizer(tokenizer); // ignore whitespace
        } else {
            fprintf(stderr, "Invalid token on line %zu\n", line_number);
            exit(EXIT_FAILURE);
        }
    }

    tokenizer->index = 0;

    return tokens;
}
