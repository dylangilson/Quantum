/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * July 12, 2018
 */

#include <stdlib.h>
#include <stdio.h>

#include <linked_list.h>

// NOTE: pushing a value to the list that is not a pointer requires a cast to a pointer with this syntax : push_head(list, (int *)<VALUE>, INTEGER, INTEGERR_LITERAL);
// NOTE: reading a value popped from the list has syntax : int *output = pop_head(list); printf("%ld", (long)output);
// NOTE: when using a linked list, always free nodes after popping

// create node
Node *create_node(void *value, NodeType node_type, TokenType token_type) {
	Node *temp = (Node *)malloc(sizeof(Node));
	temp->token.value = value;
	temp->token.token_type = token_type;
	temp->node_type = node_type;
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
void push_head(LinkedList *list, void *value, NodeType node_type, TokenType token_type) {
	if (is_empty(*list)) {
		list->head = create_node(value, node_type, token_type);
		list->tail = list->head;
	} else {
		Node *temp = list->head;
		list->head = create_node(value, node_type, token_type);
		list->head->next = temp;
	}

	list->count++;
}

// push node to tail of list
void push_tail(LinkedList *list, void *value, NodeType node_type, TokenType token_type) {
	if (is_empty(*list)) {
		list->head = create_node(value, node_type, token_type);
		list->tail = list->head;
	} else {
		Node* temp = list->tail;
		list->tail = create_node(value, node_type, token_type);
		temp->next = list->tail;
	}

	list->count++;
}

// peek at node at given index
Node *peek_at(LinkedList list, int index) {
	if (is_empty(list)) {
		fprintf(stderr, "Attempting to peek on empty list\n");
		return NULL;
	}

	if (index > list.count) {
		fprintf(stderr, "Index larger than list length\n");
		return NULL;
	}

	Node *ptr = list.head;

	for (int i = 0; i < index; i++) {
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

// pop node at given index
Node *pop_at(LinkedList *list, int index) {
	if (is_empty(*list)) {
		fprintf(stderr, "Attempting to pop at on empty list\n");
		return false;
	}

	if (index > list->count) {
		fprintf(stderr, "Index larger than list length\n");
		return NULL;
	}

	Node *temp_a = list->head;
	Node *temp_b = temp_a->next;

	for (int i = 0; i < index - 1; i++) {
		temp_b = temp_b->next;
		temp_a = temp_a->next;
	}

	Node *ret = temp_b;

	temp_a->next = temp_b->next;

	return ret;
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
		for (int i = 2; i < list->count; i++) {
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
				free(ptr);

				list->count--;

				break;
			}

			free(ptr);

			ptr = temp;
			list->count--;
		}

		free(list);

		list = NULL;
	}
}

// search for element in list
bool search(LinkedList list, void *value) {
	if (is_empty(list)) {
		fprintf(stderr, "Attempting to search on empty list\n");
		return false;
	}

	Node* ptr = list.head;

	while (ptr != NULL) {
		if (ptr->token.value == value) {
			return true;
		} else {
			if (ptr->next != NULL) {
				ptr = ptr->next;
			} else {
				break;
			}
		}
	}

	return false;
}

// display token types of list
void display_token_types(LinkedList list) {
	Node *ptr = list.head;

	printf("[");

	while (ptr != NULL) {
		if (ptr->node_type == STRING) {
			printf("%s", (char *)ptr->token.token_type);
		} else if (ptr->node_type == INTEGER || ptr->node_type == TOKEN) {
			printf("%ld", (long)ptr->token.token_type);
		}

		if (ptr->next != NULL) {
			ptr = ptr->next;

			printf(", ");
		} else {
			break;
		}
	}

	printf("]\n");
}
