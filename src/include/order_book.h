#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "../include/order.h"
// Node structure for the linked list of orders
typedef struct orderNode {
    Order *buyOrder, *sellOrder;
    struct orderNode *next;
} orderNode;

// Order book structure
typedef orderNode* Order_book;

void init_order_book(Order_book *order_book);
void addorder_to_orderBook(Order_book *orderBook, Order *buy, Order *sell);
void display_matched_order(Order_book *orderBook);
#endif // !ORDERBOOK_H
