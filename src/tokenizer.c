/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 30, 2023
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

// create tokenizer
Tokenizer *create_tokenizer(const char *src) {
    Tokenizer *tokenizer = (Tokenizer *)malloc(sizeof(Tokenizer));
    tokenizer->src = src;
    tokenizer->index = 0;

    return tokenizer;
}

// peek at character that is count indices ahead of current index
char peek_tokenizer(Tokenizer tokenizer, size_t count) {
    if (tokenizer.index + count > strlen(tokenizer.src)) {
        return '\0'; // empty string
    } else {
        return tokenizer.src[tokenizer.index];
    }
}

// consume character at current index of tokenizer
char consume_tokenizer(Tokenizer *tokenizer) {
    return tokenizer->src[tokenizer->index++];
}

// convert string into list of tokens
LinkedList *tokenize(Tokenizer *tokenizer, LinkedList *tokens) {
    char buffer[BUFFER_CAPACITY]; // file contents
    memset(buffer, 0, sizeof(buffer)); // clear buffer

    while(peek_tokenizer(*tokenizer, 0) != '\0') {
        if (isalpha(peek_tokenizer(*tokenizer, 0))) {
            char c = consume_tokenizer(tokenizer); // get charcater
            strncat(buffer, &c, 1); // append character to buffer
            
            // while character is alphanumeric -> append to buffer
            while (peek_tokenizer(*tokenizer, 0) != '\0' && isalnum(peek_tokenizer(*tokenizer, 0))) {
                char c = consume_tokenizer(tokenizer); // get character
                strncat(buffer, &c, 1);
            }

            // push token to token list
            if (strncmp(buffer, "exit", strlen(buffer)) == 0) {
                push_tail(tokens, NULL, TOKEN, EXIT); // append token
                memset(buffer, 0, sizeof(buffer)); // clear buffer
            } else {
                fprintf(stderr, "Invalid expression\n");
                exit(EXIT_FAILURE);
            }
        } else if (isdigit(peek_tokenizer(*tokenizer, 0))) {
            char c = consume_tokenizer(tokenizer); // get character
            strncat(buffer, &c, 1); // append character to buffer

            // while character is digit -> append to buffer
            while (peek_tokenizer(*tokenizer, 0) != '\0' && isdigit(peek_tokenizer(*tokenizer, 0))) {
                char c = consume_tokenizer(tokenizer); // get character
                strncat(buffer, &c, 1);
            }

            long temp = atoi(buffer); // convert string to number
            push_tail(tokens, (int *)temp, INTEGER, INTEGER_LITERAL); // append number to buffer
            memset(buffer, 0, sizeof(buffer)); // clear buffer
        } else if (peek_tokenizer(*tokenizer, 0) == ';') {
            consume_tokenizer(tokenizer);
            push_tail(tokens, NULL, TOKEN, SEMICOLON); // append semicolon
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
