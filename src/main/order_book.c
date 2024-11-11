#include <stdio.h>
#include <stdlib.h>
#include "order_book.h"
#include "order.h"


// to initialize the order book
void init_order_book(Order_book *order_book) {
    *order_book = NULL;
    return;
}

// add orders among which transaction is done
void addorder_to_orderBook(Order_book *orderBook, Order *buy, order *sell) {
    orderNode *newnode = (orderNode *)malloc(sizeof(orderNode));
    newnode -> buyOrder = buy;
    newnode -> sellOrder = sell;
    newnode -> next = *orderBook;
    *orderBook = newnode;
    return;
}
