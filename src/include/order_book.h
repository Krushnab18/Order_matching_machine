#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "order.h"

// Node structure for the linked list of orders
typedef struct OrderNode {
    Order order;                  // The order data
    struct OrderNode *next;      // Pointer to the next order node
} OrderNode;

// Order book structure
typedef struct Order_book {
    OrderNode *head;  // Pointer to the head of the linked list
    OrderNode *tail;  // Pointer to the tail of the linked list
} Order_book;

// Function prototypes
void init_order_book(Order_book *order_book);                   // Initialize the order book
int add_order(Order_book *order_book, const Order *new_order); // Add an order
int delete_order(Order_book *order_book, int order_id);        // Remove an order by ID
void print_order_book(const Order_book *order_book);           // Print all orders
Order *get_order_by_id(const Order_book *order_book, int id);  // Get order by ID

#endif // ORDERBOOK_H
