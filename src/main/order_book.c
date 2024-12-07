#include <stdio.h>
#include <stdlib.h>
#include "../include/order_book.h"
#include "../include/order.h"


// to initialize the order book
void init_order_book(Order_book *order_book) {
    *order_book = NULL;
    return;
}

// add orders among which transaction is done
void addorder_to_orderBook(Order_book *orderBook, Order *buy, Order *sell) {
    orderNode *newnode = (orderNode *)malloc(sizeof(orderNode));
    newnode -> buyOrder = buy;
    newnode -> sellOrder = sell;
    newnode -> next = *orderBook;
    *orderBook = newnode;
    return;
}

//display order
/*void display_matched_order(Order_book *orderBook) {
    if(*orderBook == NULL) {
        return;
    } 
    orderNode *temp = *orderBook;
    printf("The buy-order of id:%lld has successfully done agreement with sell-order of id:%lld.\n", temp -> buyOrder -> order_id, temp -> sellOrder -> order_id);
    printf("The number of shares in agreement is : %d.\n", temp -> buyOrder -> quantity);
    printf("The price of per share is : %lf.\n", temp -> buyOrder -> price);
    printf("\n");
    return;
}*/

/*
void display_matched_order(Order_book *orderBook) {
    if (*orderBook == NULL) {
        return;
    }

    orderNode *temp = *orderBook;

    printf("Buy Order ID: %lld | Sell Order ID: %lld | Shares: %d | Price/Share: %.2lf\n", 
           temp->buyOrder->order_id, 
           temp->sellOrder->order_id, 
           temp->buyOrder->quantity, 
           temp->buyOrder->price);
}*/



void display_matched_order(Order_book *orderBook) {
    if (*orderBook == NULL) {
        return;
    }

    orderNode *temp = *orderBook;

    printf("%-12lld %-12lld %-8d %-11.2lf\n", 
           temp->buyOrder->order_id, 
           temp->sellOrder->order_id, 
           temp->buyOrder->quantity, 
           temp->buyOrder->price);
}
