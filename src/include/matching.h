//matching algorithm
#ifndef MATCHING_H
#define MATCHING_H

#include "../include/order.h"
#include "../include/order_book.h"
#include "../include/avl_tree.h"

// Function declaration for matching orders based on FIFO logic
void matching_fifo(buy_order_avl *buy_tree, sell_order_avl *sell_tree, Order *new_order, Order_book *orderBook);

#endif /* MATCHING_H */
