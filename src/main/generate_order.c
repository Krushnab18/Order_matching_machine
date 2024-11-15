#include "../include/order.h"
#include "../include/order_book.h"
#include "../include/avl_tree.h"
#include "../include/matching.h"
#include <unistd.h>  // Add this at the top of your file
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
// Function to generate a unique 10-digit order ID
long long int generate_unique_order_id() {
    static long long int counter = 1000000000; // Start from a 10-digit number
    return counter++; // Increment and return unique ID
}

Order* create_unique_order(char type) {
    Order* order = (Order*)malloc(sizeof(Order));

    // Generate a unique 10-digit order_id
    order->order_id = generate_unique_order_id();
    order->side = type;

    // Randomize quantity between 1 and 100
    order->quantity = rand() % 100 + 1; // Random quantity between 1 and 100

    // Randomize price between 50.00 and 60.00, with two decimal places
    order->price = (rand() % 11) + 50.00; // Random price between 50.00 and 60.00

    // Round to two decimal places
    order->price = ((int)(order->price * 100)) / 100.0;

    // Get current time for unique timestamp
    struct timeval tv;
    gettimeofday(&tv, NULL);

    // Introduce a small delay (in microseconds) to prevent same timestamp for multiple orders
    usleep(10);  // 10 microseconds delay (or adjust as necessary)

    order->time_stamp.tv_sec = tv.tv_sec;
    order->time_stamp.tv_usec = tv.tv_usec;

    return order;
}

// void insert_sample_orders(buy_order_avl *buy_tree, sell_order_avl *sell_tree) {
//     srand(time(NULL)); // Initialize random seed
//
//     for (int i = 0; i < 10; i++) {
//         // Create and insert buy order
//         Order *buy_order = create_unique_order('B');
//
//
//         // Create and insert sell order
//         Order *sell_order = create_unique_order('S');
//         insert_sell_order(sell_tree, sell_order);
//     }
// }
