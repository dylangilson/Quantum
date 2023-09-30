/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * July 12, 2018
 */

#pragma once

#include "bool.h"

typedef enum TokenType {
    EXIT,
    INTEGER_LITERAL,
    SEMICOLON
} TokenType;

typedef struct Token {
    void *value;
    TokenType token_type;
} Token;

typedef enum NodeType {
    TOKEN,
    INTEGER,
    STRING
} NodeType;

typedef struct Node {
    Token token;
    NodeType node_type;
    struct Node *next;
} Node;

typedef struct LinkedList {
    Node *head;
    Node *tail;
    int count;
} LinkedList;

Node *create_node(void *value, NodeType type, TokenType token_type);
LinkedList *create_list();
bool is_empty(LinkedList list);
void push_head(LinkedList *list, void *value, NodeType node_type, TokenType token_type);
void push_tail(LinkedList *list, void *value, NodeType node_type, TokenType token_type);
Node *peek_at(LinkedList list, int index);
Node *peek_head(LinkedList list);
Node *peek_tail(LinkedList list);
Node *popHead(LinkedList *list);
Node *pop_tail(LinkedList *list);
void free_list(LinkedList *list);
bool search(LinkedList list, void *value);
void display_token_types(LinkedList list);
