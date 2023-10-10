/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * July 12, 2018
 */

#include <stdlib.h>
#include <stdio.h>

#include <linked_list.h>

// NOTE: pushing a value to the list that is not a pointer requires a cast to a pointer with this syntax : push_head(list, (int *)<VALUE>, INTEGER_LITERAL);
// NOTE: reading a value popped from the list has syntax : int *output = pop_head(list); printf("%ld", (long)output);
// NOTE: when using a linked list, always free nodes after popping

// create node
Node *create_node(void *value, TokenType token_type) {
	Token *token = (Token *)malloc(sizeof(Token));
	token->value = value;
	token->token_type = token_type;

	Node *temp = (Node *)malloc(sizeof(Node));
	temp->token = token;
	temp->next = NULL;

	return temp;
}

// create list
LinkedList *create_list() {
	LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
	list->head = NULL;
	list->tail = NULL;
	list->count = 0;

	return list;
}

// check if list is empty
bool is_empty(LinkedList list) {
	return (list.count == 0);
}

// push node to head of list
void push_head(LinkedList *list, void *value, TokenType token_type) {
	if (is_empty(*list)) {
		list->head = create_node(value, token_type);
		list->tail = list->head;
	} else {
		Node *temp = list->head;
		list->head = create_node(value, token_type);
		list->head->next = temp;
	}

	list->count++;
}

// push node to tail of list
void push_tail(LinkedList *list, void *value, TokenType token_type) {
	if (is_empty(*list)) {
		list->head = create_node(value, token_type);
		list->tail = list->head;
	} else {
		Node* temp = list->tail;
		list->tail = create_node(value, token_type);
		temp->next = list->tail;
	}

	list->count++;
}

// peek at node at given index
Node *peek_at(LinkedList list, size_t index) {
	if (is_empty(list)) {
		fprintf(stderr, "Attempting to peek on empty list\n");
		return NULL;
	}

	if (index > list.count) {
		fprintf(stderr, "Index larger than list length\n");
		return NULL;
	}

	Node *ptr = list.head;

	for (size_t i = 0; i < index; i++) {
		ptr = ptr->next;
	}

	return ptr;
}

// peek at node at head of list
Node *peek_head(LinkedList list) {
	if (is_empty(list)) {
		fprintf(stderr, "Attempting to peek head on empty list\n");
		return NULL;
	}
	
	return list.head;
}

// peek at node at tail of list
Node *peek_tail(LinkedList list) {
	if (is_empty(list)) {
		fprintf(stderr, "Attempting to peek tail on empty list\n");
		return NULL;
	}

	return list.tail;
}

// pop node at head of list
Node *pop_head(LinkedList *list) {
	if (is_empty(*list)) {
		fprintf(stderr, "Attempting to pop head on empty list\n");
		return NULL;
	}
	
	void *ret = list->head;

	if (list->head->next != NULL) {
		Node *temp = list->head->next;
		list->head = temp;
	} else {
		list->head = NULL;
		list->tail = NULL;
	}

	list->count--;

	return ret;
}

// pop node at tail of list
Node *pop_tail(LinkedList *list) {
	if (is_empty(*list)) {
		fprintf(stderr, "Attempting to pop tail on empty list\n");
		return NULL;
	}

	void *ret = list->tail;
	Node* ptr = list->head;

	if (list->count == 1) {
		list->head = NULL;
		list->tail = NULL;
	} else {
		for (size_t i = 2; i < list->count; i++) {
			ptr = ptr->next;
		}

		ptr->next = NULL;
		list->tail = ptr;
	}

	list->count--;

	return ret;
}

// free list
void free_list(LinkedList *list) {
	if (list != NULL) {
		Node *ptr = list->head;

		while (ptr != NULL) {
			Node *temp;

			if (ptr->next != NULL) {
				temp = ptr->next;
			} else {
				free(ptr->token);
				free(ptr);

				list->count--;

				break;
			}

			free(ptr->token);
			free(ptr);

			ptr = temp;
			list->count--;
		}

		free(list);

		list = NULL;
	}
}
