#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>  // For multithreading
#include "./src/include/order.h"
#include "./src/include/order_book.h"
#include "./src/include/avl_tree.h"
#include "./src/include/generate_order.h"

#define NUM_THREADS 5
// Define a structure to hold arguments for the order processing function
typedef struct {
    buy_order_avl *buy_tree;
    sell_order_avl *sell_tree;
    Order_book *order_book;
} OrderProcessingArgs;

void *process_orders(void *args) {
    // Cast the argument to our custom struct type
    OrderProcessingArgs *op_args = (OrderProcessingArgs *)args;
    buy_order_avl *buy_tree = op_args->buy_tree;
    sell_order_avl *sell_tree = op_args->sell_tree;
    Order_book *order_book = op_args->order_book;

    // Process 250,000 orders per thread (adjust as needed)
    for (int i = 0; i < 250000; i++) {
        Order *buy_order = create_unique_order('B');
        matching_fifo(buy_tree, sell_tree, buy_order, order_book);

        Order *sell_order = create_unique_order('S');
        matching_fifo(buy_tree, sell_tree, sell_order, order_book);
    }

    return NULL;
}

int main() {
    buy_order_avl buy_tree;
    sell_order_avl sell_tree;
    Order_book order_book;

    init_buy_order_avl(&buy_tree);
    init_sell_order_avl(&sell_tree);
    init_order_book(&order_book);

    // Initialize the arguments to be passed to each thread
    OrderProcessingArgs args = {&buy_tree, &sell_tree, &order_book};

    // Create and start threads
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, process_orders, &args);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Inorder traversal of buy tree: \n");
    inorder_buy_order(buy_tree.root);
    printf("Inorder traversal of sell tree: \n");
    inorder_sell_order(sell_tree.root);

    return 0;
}
