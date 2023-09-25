/*
 * Dylan Gilson
 * dylan.gilson@outlook.com
 * July 12, 2018
 */

#include <stdlib.h>
#include <stdio.h>

#include <linked_list.h>

// NOTE: pushing a value to the list that is not a pointer requires a cast to a pointer with this syntax : pushHead(list, (int *)<VALUE>, INTEGER, INTEGERR_LITERAL);
// NOTE: reading a value popped from the list has syntax : int *output = popHead(list); printf("%ld", (long)output);

Node *createNode(void *value, NodeType node_type, TokenType token_type) {
	Node *temp = (Node *)malloc(sizeof(Node));
	temp->token.value = value;
	temp->token.token_type = token_type;
	temp->node_type = node_type;
	temp->next = NULL;

	return temp;
}

LinkedList *createList() {
	LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
	list->head = NULL;
	list->tail = NULL;
	list->count = 0;

	return list;
}

bool isEmpty(LinkedList list) {
	return (list.count == 0);
}

void pushHead(LinkedList *list, void *value, NodeType node_type, TokenType token_type) {
	if (isEmpty(*list)) {
		list->head = createNode(value, node_type, token_type);
		list->tail = list->head;
	} else {
		Node *temp = list->head;
		list->head = createNode(value, node_type, token_type);
		list->head->next = temp;
	}

	list->count++;
}

void pushTail(LinkedList *list, void *value, NodeType node_type, TokenType token_type) {
	if (isEmpty(*list)) {
		list->head = createNode(value, node_type, token_type);
		list->tail = list->head;
	} else {
		Node* temp = list->tail;
		list->tail = createNode(value, node_type, token_type);
		temp->next = list->tail;
	}

	list->count++;
}

Node *at(LinkedList list, int index) {
	if (isEmpty(list)) {
		fprintf(stderr, "Attempting to peek head on empty list\n");
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

Node *peekHead(LinkedList list) {
	if (isEmpty(list)) {
		fprintf(stderr, "Attempting to peek head on empty list\n");
		return NULL;
	}
	
	return list.head;
}

Node *peekTail(LinkedList list) {
	if (isEmpty(list)) {
		fprintf(stderr, "Attempting to peek tail on empty list\n");
		return NULL;
	}

	return list.tail;
}

Node *popHead(LinkedList *list) {
	if (isEmpty(*list)) {
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

Node *popTail(LinkedList *list) {
	if (isEmpty(*list)) {
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

void freeList(LinkedList *list) {
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

bool search(LinkedList list, void *value) {
	if (isEmpty(list)) {
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
