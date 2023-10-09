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
char peek_tokenizer(Tokenizer tokenizer, size_t offset) {
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
            if (strncmp(tokenizer->buffer, "exit", strlen(tokenizer->buffer)) == 0) {
                push_tail(tokens, NULL, EXIT); // append exit token
                memset(tokenizer->buffer, 0, BUFFER_CAPACITY); // clear buffer
            } else if (strncmp(tokenizer->buffer, "let", strlen(tokenizer->buffer)) == 0) {
                push_tail(tokens, NULL, LET); // append let token
                memset(tokenizer->buffer, 0, BUFFER_CAPACITY); // clear buffer
            } else {
                char *identifier = (char *)malloc((strlen(tokenizer->buffer) + 1) * sizeof(char));
                strcpy(identifier, tokenizer->buffer);
                push_tail(tokens, identifier, IDENTIFIER);
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
            push_tail(tokens, (int *)temp, INTEGER_LITERAL); // append number to buffer
            memset(tokenizer->buffer, 0, BUFFER_CAPACITY); // clear buffer
        } else if (peek_tokenizer(*tokenizer, 0) == '(') {
            consume_tokenizer(tokenizer); // consume '('
            push_tail(tokens, NULL, OPEN_PARENTHESIS); // append open parenthesis
        } else if (peek_tokenizer(*tokenizer, 0) == ')'){
            consume_tokenizer(tokenizer); // consume ')'
            push_tail(tokens, NULL, CLOSE_PARENTHESIS); // append close parenthesis
        } else if (peek_tokenizer(*tokenizer, 0) == '='){
            consume_tokenizer(tokenizer); // consume '='
            push_tail(tokens, NULL, EQUALS); // append equals
        } else if (peek_tokenizer(*tokenizer, 0) == '+') {
            consume_tokenizer(tokenizer); // consume '+'
            push_tail(tokens, NULL, PLUS); // append plus
        } else if (peek_tokenizer(*tokenizer, 0) == '-') {
            consume_tokenizer(tokenizer); // consume '-'
            push_tail(tokens, NULL, MINUS); // append minus
        } else if (peek_tokenizer(*tokenizer, 0) == '*') {
            consume_tokenizer(tokenizer); // consume '*'
            push_tail(tokens, NULL, STAR); // append star
        } else if (peek_tokenizer(*tokenizer, 0) == '/') {
            consume_tokenizer(tokenizer); // consume '/'
            push_tail(tokens, NULL, FORWARD_SLASH); // append forward slash
        } else if (peek_tokenizer(*tokenizer, 0) == ';') {
            consume_tokenizer(tokenizer); // consume ';'
            push_tail(tokens, NULL, SEMICOLON); // append semicolon
        } else if (isspace(peek_tokenizer(*tokenizer, 0))) {
            consume_tokenizer(tokenizer); // ignore whitespace
        } else {
            fprintf(stderr, "Invalid expression\n");
            exit(EXIT_FAILURE);
        }
    }

    tokenizer->index = 0;

    return tokens;
}
