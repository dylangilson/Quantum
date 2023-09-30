/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 30, 2023
 */

#pragma once

#include "linked_list.h"

#define BUFFER_CAPACITY 4096

typedef struct Tokenizer {
    const char *src;
    size_t index;
} Tokenizer;

Tokenizer *create_tokenizer(const char *src);
char peek_tokenizer(Tokenizer tokenizer, size_t count);
char consume_tokenizer(Tokenizer *tokenzier);
LinkedList *tokenize(Tokenizer *tokenizer, LinkedList *tokens);
