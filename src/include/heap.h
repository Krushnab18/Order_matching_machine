#ifndef HEAP_H
#define HEAP_H

#include "order.h"

typedef struct MaxHeap {
    Order *orders; // Array of Orders
    int size; // Current size of the heap
    int capacity; // Maximum capacity of the heap
} MaxHeap;

typedef struct MinHeap {
    Order *orders; // Array of Orders
    int size; // Current size of the heap
    int capacity; // Maximum capacity of the heap
} MinHeap;

// Function declarations for max-heap
void insert_max_heap(MaxHeap *heap, Order order);
Order remove_max(MaxHeap *heap);
void heapify_max(MaxHeap *heap, int index);
int compare_max(const Order* order1, const Order* order2);

// Function declarations for min-heap
void insert_min_heap(MinHeap *heap, Order order);
Order remove_min(MinHeap *heap);
void heapify_min(MinHeap *heap, int index);
int compare_min(const Order* order1, const Order* order2);

#endif // HEAP_H
