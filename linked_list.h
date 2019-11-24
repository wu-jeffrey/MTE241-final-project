#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct node_t node_t;
struct node_t {
    int x,y;
    struct node_t * prev;
    struct node_t* next;
};

typedef struct linked_list_t linked_list_t;
struct linked_list_t {
    node_t* start;
    node_t* end;
};

void print(node_t* node);
void printAll(node_t* head);

void dequeue(linked_list_t* list);
void pushFront(linked_list_t* list, node_t* node);
void pushEnd(linked_list_t* list, node_t* node);
void clearList(linked_list_t* list);

#endif