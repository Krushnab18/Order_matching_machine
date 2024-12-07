#ifndef ORDER_H
#define ORDER_H

#include <sys/time.h> // Include for struct timeval

typedef struct Order {
    long long order_id;
    int quantity;
    char timestamp[20];
    double price;
    struct timeval time_stamp; // Correct type for time_stamp
    char side;
} Order;

Order *create_new_order(long long order_id, int quantity, double price, char side);
void print_order(const Order order);
int compareOrders(const Order* order1, const Order* order2);

#endif // ORDER_H
