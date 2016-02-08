/*
 * Generic queue implementation.
 *
 */
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct node {
    struct node* next;
    struct node* prev;
    void* value;
} Node;

typedef struct queue {
    Node* start;
    Node* end;
    int length;
} queue_t;

queue_t*
queue_new() {
    queue_t * new_queue = (queue_t *) malloc(sizeof(queue_t));
    new_queue->length = 0;
    return new_queue;
}

int
queue_prepend(queue_t *queue, void* item) {
    if (queue == NULL || item == NULL) return -1;

    Node* new_node = (Node *)malloc(sizeof(Node));
    new_node->value = item;
    new_node->prev = NULL;

    if (queue->length == 0) {
        new_node->next = NULL;
	queue->start = new_node;
	queue->end = new_node;
    }
    else {
	new_node->next = queue->start;
        queue->start = new_node;
    }

    queue->length++;
    return 0;
}

int
queue_append(queue_t *queue, void* item) {
    if (queue == NULL || item == NULL) return -1;
    Node* new_node = (Node *)malloc(sizeof(Node));
    new_node->value = item;
    new_node->next = NULL;

    if (queue->length == 0) {
	new_node->prev = NULL;
        queue->start = new_node;
	queue->end = new_node;
    }
    else{
	new_node->prev = queue->end;
        queue->end->next = new_node;
        queue->end = new_node;
    }

    queue->length++;
    return 0;
}

int
queue_dequeue(queue_t *queue, void** item) {
    if (queue->length == 0) {
        *item = NULL;
        return -1;
    }
    else {
  	*item = queue->start->value;
        Node *temp = queue->start;
        queue->start = queue->start->next;
  	queue->start->prev = NULL;
        free(temp);
        queue->length --;
        return 0;
    }
}

int
queue_iterate(queue_t *queue, func_t f, void* arg) {
    if(f == NULL || queue == NULL) return -1;
    if (queue->length == 0) return 0;
    printf("start queue_iterate:\n");
    Node *temp = queue->start;
    while (temp != NULL) {
	f(temp->value, arg);
        printf("keep going\n");
        temp = temp->next;
    }
    return 0;
}

int
queue_free (queue_t *queue) {
    if (queue->length != 0 || queue == NULL) return -1;

    free(queue);
    return 0;

}

int
queue_length(const queue_t *queue) {
    return queue->length;
}

int
queue_delete(queue_t *queue, void* item) {
    if (queue == NULL || queue->length == 0) return -1;

     Node *curr = queue->start;
     while (curr != NULL) {
	if (*((int *)(curr->value)) == *((int *)(item))) {
	    if(curr->prev == NULL) {
		queue->start = curr->next;
		queue->start->prev = NULL;
	    }
            else curr->prev->next = curr->next;
            free(curr);
            queue->length --;
            return 0;
	}
        curr = curr->next;
     }
     return -1;
}

void
add(void *a, void *b) {
   *(int *)a += *(int *)b;
}

int main(int argc, const char* argv[]) {
    queue_t* que = queue_new();
    int a = 2;
    int b = 3;
    int c = 4;
    int d = 5;
    int e = 3;
    
    queue_append(que, &a);
    queue_append(que, &b);
    queue_prepend(que, &c);
    queue_prepend(que, &d);

    Node *temp = que->start;
    while (temp != NULL) {
	printf("%d ",*((int*)(temp->value)));
        temp = temp->next;
    }

    func_t f = &add;
    queue_iterate(que, f, &e);
    queue_delete(que, &e);

    temp = que->start;
    printf("\ndelete %d:\n", e);
    while (temp != NULL) {
	printf("%d ",*((int*)(temp->value)));
        temp = temp->next;
    }
    printf("\n");
    printf("queue length: %d\n", queue_length(que)); 
    return 0;

}
