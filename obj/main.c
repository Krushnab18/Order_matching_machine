#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "./src/include/order.h"
#include "./src/include/order_book.h"
#include "./src/include/avl_tree.h"
#include "./src/include/generate_order.h"
#include "./src/include/display.h"

void convert_to_timeval(const char *time_str, struct timeval *tv) {
    int hours, minutes, seconds;
    // Parse the HH:MM:SS time format
    sscanf(time_str, "%d:%d:%d", &hours, &minutes, &seconds);

    // Calculate total seconds from midnight
    (tv)->tv_sec = (hours * 3600) + (minutes * 60) + seconds;
    (tv)->tv_usec = 0;  // Set microseconds to 0 as we only have HH:MM:SS format
}
// Function to generate random prices with realistic intervals
double generate_random_price() {
    return (1520 + (rand() % 30) * 0.25 * 100) / 100.0;  // Generate prices in intervals of 0.25
}

// Function to generate random quantities
int generate_random_quantity() {
    return rand() % 10 + 1;  // Quantity between 1 to 10
}

// Simulate order generation logic
void simulate_order_flow(sell_order_avl *sell_tree, buy_order_avl *buy_tree, Order_book *order_book) {
    srand(time(NULL)); // Seed for randomness

    for (int i = 0; i < 1000; i++) { // Simulate 1000 orders
        Order *order = (Order *)malloc(sizeof(Order));
        order->order_id = i;  // Unique Order ID
        order->side = (rand() % 2) ? 'B' : 'S'; // Randomly generate Buy or Sell order
        strcpy(order->timestamp, "09:00:00"); // Set a static timestamp
        order->price = generate_random_price();
        order->quantity = generate_random_quantity();
        convert_to_timeval(order->timestamp, &(order->time_stamp));

        matching_fifo(buy_tree, sell_tree, order, order_book);

        printf("Generated Order ID: %lld | Side: %c | Price: %.2f | Quantity: %d\n", 
                order->order_id, order->side, order->price, order->quantity);
        
        free(order);
        usleep(100); // Simulate real-time delays between orders
    }
}

int main() {
    sell_order_avl sell_tree;
    buy_order_avl buy_tree;

    init_buy_order_avl(&buy_tree);
    init_sell_order_avl(&sell_tree);

    Order_book order_book;
    init_order_book(&order_book);

    printf("Starting order matching simulation...\n");
    simulate_order_flow(&sell_tree, &buy_tree, &order_book);

    printf("\nFinal AVL Tree Traversal - Buy Orders:\n");
    inorder_buy_order(buy_tree.root);

    printf("\nFinal AVL Tree Traversal - Sell Orders:\n");
    inorder_sell_order(sell_tree.root);

    return 0;
}
