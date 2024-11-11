#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include "../include/order.h"


Order *create_new_order(long long order_id, int quantity, double price, char side) {
    
    Order *newOrder = (Order*)malloc(sizeof(Order));

    if(!newOrder) {
        printf("Order not created\n");
        return NULL;
    }
    
    newOrder->order_id = order_id;
    newOrder->quantity = quantity;
    newOrder->price = price;
    newOrder->side = side;

    // Get current time with microseconds precision
    gettimeofday(&newOrder->time_stamp, NULL);

    return newOrder;
}

// Function to print an order's details
void print_order(const Order order) {
    printf("Order ID: %lld\n", order.order_id);
    printf("Quantity: %d\n", order.quantity);
    printf("Price: %.2f\n", order.price);
    printf("Side: %c\n", order.side);
    printf("Timestamp: %ld seconds, %ld microseconds\n", order.time_stamp.tv_sec, order.time_stamp.tv_usec);
    return;
}

// Function to compare two orders based on their timestamps
int compareOrders(const Order* order1, const Order* order2) {

    if (order1->time_stamp.tv_sec < order2->time_stamp.tv_sec) {
        return -1; // order1 is earlier
    }
    else if (order1->time_stamp.tv_sec > order2->time_stamp.tv_sec) {
        return 1; // order2 is earlier
    }
    else {
        // If seconds are the same, compare microseconds
        if (order1->time_stamp.tv_usec < order2->time_stamp.tv_usec) {
            return -1; // order1 is earlier
        }
        else if (order1->time_stamp.tv_usec > order2->time_stamp.tv_usec) {
            return 1; // order2 is earlier
        }
        else {
            return 0; // Orders were placed at the exact same time
        }
    }
}

