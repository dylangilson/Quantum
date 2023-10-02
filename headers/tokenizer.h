/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * September 30, 2023
 */

#pragma once

#include "linked_list.h"

typedef struct Tokenizer {
    const char *src;
    int index;
} Tokenizer;

Tokenizer *create_tokenizer(const char *src);
char peek_tokenizer(Tokenizer tokenizer, int offset);
char consume_tokenizer(Tokenizer *tokenzier);
LinkedList *tokenize(Tokenizer *tokenizer, LinkedList *tokens);
