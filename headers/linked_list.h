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

Node *createNode(void *value, NodeType type, TokenType token_type);
LinkedList* createList();
bool isEmpty(LinkedList list);
void pushHead(LinkedList *list, void *value, NodeType node_type, TokenType token_type);
void pushTail(LinkedList *list, void *value, NodeType node_type, TokenType token_type);
Node *at(LinkedList list, int index);
Node *peekHead(LinkedList list);
Node *peekTail(LinkedList list);
Node *popHead(LinkedList *list);
Node *popTail(LinkedList *list);
void freeList(LinkedList *list);
void display_token_types(LinkedList list);
