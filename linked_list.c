#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

/*******************************************************************************
* Useful Debugging Functions
*******************************************************************************/
void print(node_t* node) {
  printf("(%d, %d)", node->x, node->y);
}

void printAll(node_t* head) {
  node_t *current_node = head;
  while ( current_node != NULL) {
    print(current_node);
    current_node = current_node->next;
  }
  printf("\n\n");
}

/*******************************************************************************
* Data structure operations
*******************************************************************************/
void dequeue(linked_list_t* list) {
  node_t* last = list->end;
  list->end = list->end->prev;
  list->end->next = NULL;
  free(last);
}

void pushFront(linked_list_t* list, node_t* node) {
  node_t* first = list->start;
	list->start = node;
	list->start->next = first;
	first->prev = list->start;
	list->start->prev = NULL;
}

void pushEnd(linked_list_t* list, node_t* node) {
  node_t* last = list->end;
  list->end = node;
  list->end->prev = last;
  last->next = list->end;
  list->end->next = NULL;
}

void clearList(linked_list_t* list) {

}