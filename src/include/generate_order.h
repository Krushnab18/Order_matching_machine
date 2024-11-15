#ifndef GENERATE_ORDER_H
#define GENERATE_ORDER_H

#include "order.h"
#include "order_book.h"
#include "avl_tree.h"
#include "matching.h"

// Function to generate a unique 10-digit order ID
long long int generate_unique_order_id();

// Function to create and initialize a new unique order
Order* create_unique_order(char type);

// Function to insert sample orders into the buy and sell order trees
void insert_sample_orders(buy_order_avl *buy_tree, sell_order_avl *sell_tree);

#endif // GENERATE_ORDER_H
