/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * July 12, 2018
 */

#pragma once

#include "bool.h"

#define BUFFER_CAPACITY 1024
#define PROGRAM_CAPACITY 512

typedef enum TokenType {
    EXIT,
    INTEGER_LITERAL,
    SEMICOLON,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    IDENTIFIER,
    LET,
    EQUALS,
    PLUS,
    MINUS,
    STAR,
    FORWARD_SLASH
} TokenType;

typedef struct Token {
    void *value;
    TokenType token_type;
} Token;

typedef struct Node {
    Token *token;
    struct Node *next;
} Node;

typedef struct LinkedList {
    Node *head;
    Node *tail;
    size_t count;
} LinkedList;

Node *create_node(void *value, TokenType token_type);
LinkedList *create_list();
bool is_empty(LinkedList list);
void push_head(LinkedList *list, void *value, TokenType token_type);
void push_tail(LinkedList *list, void *value, TokenType token_type);
Node *peek_at(LinkedList list, size_t index);
Node *peek_head(LinkedList list);
Node *peek_tail(LinkedList list);
Node *pop_head(LinkedList *list);
Node *pop_tail(LinkedList *list);
void free_list(LinkedList *list);
