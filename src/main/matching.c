#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/order.h"
#include "../include/order_book.h"
#include "../include/avl_tree.h"


void matching_fifo(buy_order_avl *buy_tree, sell_order_avl *sell_tree, Order *new_order, Order_book *orderBook) {

    if(new_order -> side == 'B') {
        // newly generated order is buy-order

        // initially when there is no sell-order placed 
        if(sell_tree -> root == NULL) {

            insert_buy_order(buy_tree, new_order);
            printf("There is no sell-order placed by this time.\n");

            return;
        }

        Node *temp = sell_tree -> root;
        // when there is single order in the sell-order 
        if((temp -> lchild) == NULL && (temp -> rchild) == NULL) {

            // when the buy order doesn't match with the only sell-order
            if((new_order -> price) != (temp -> order -> price)) {

                insert_buy_order(buy_tree, new_order);
                printf("There is no match for the given buy-order in the sell-order\n");

                return;

            } else {

                // when the buy-order-quantity matches with the sell-order-quantity
                if((temp -> order -> quantity) == (new_order -> quantity)) {

                    addorder_to_orderBook(orderBook, new_order, temp -> order);
                    delete_sell_order(sell_tree, temp -> order, &sell_tree->root);
                    printf("The order is matched.\n");
                    display_matched_order(orderBook);

                } else if((temp -> order -> quantity) > (new_order -> quantity)) {

                    Order *matched_order = (Order *)malloc(sizeof(Order));
                    memcpy(matched_order, temp -> order, sizeof(Order));
                    matched_order -> quantity = new_order -> quantity;

                    addorder_to_orderBook(orderBook, new_order, matched_order);
                    temp -> order -> quantity = (temp -> order -> quantity) - (new_order -> quantity);
                    printf("The order is matched.\n");
                    display_matched_order(orderBook);

                } else {

                    Order *matched_order = (Order *)malloc(sizeof(Order));
                    memcpy(matched_order, new_order, sizeof(Order));
                    matched_order -> quantity = temp -> order -> quantity;

                    addorder_to_orderBook(orderBook, matched_order, temp -> order);
                    new_order -> quantity = (new_order -> quantity) - (temp -> order -> quantity);
                    insert_buy_order(buy_tree, new_order);
                    delete_sell_order(sell_tree, temp -> order, &sell_tree->root);

                    printf("The order is matched.\n");
                    display_matched_order(orderBook);
                    
                }

                return;

            }

        } else {

            int flag = 0;  // used to match orders repeatedly until new-order gets over

            while(1) {

                    while((temp != NULL) && ((new_order -> price) != (temp -> order -> price))) {

                    if((temp -> order -> price) > (new_order -> price)) {
                        temp = temp -> lchild;
                    } else {
                        temp = temp -> rchild;
                    }

                }

                if(temp == NULL) {

                    insert_buy_order(buy_tree, new_order);
                    
                    // not a single order match
                    if(flag == 0) {
                        printf("There is no match for the given buy-order in the sell-order\n");
                    }

                    return;

                } else {

                    if((temp -> order -> quantity) == (new_order -> quantity)) {

                        addorder_to_orderBook(orderBook, new_order, temp -> order);
                        delete_sell_order(sell_tree, temp -> order, &sell_tree->root);
                        printf("The order is matched.\n");
                        display_matched_order(orderBook);
                        return;

                    } else if((temp -> order -> quantity) > (new_order -> quantity)) {

                        Order *matched_order = (Order *)malloc(sizeof(Order));
                        memcpy(matched_order, temp -> order, sizeof(Order));
                        matched_order -> quantity = new_order -> quantity;

                        addorder_to_orderBook(orderBook, new_order, matched_order);
                        temp -> order -> quantity = (temp -> order -> quantity) - (new_order -> quantity);
                        printf("The order is matched.\n");
                        display_matched_order(orderBook);

                        return;

                    } else {

                        Order *matched_order = (Order *)malloc(sizeof(Order));
                        memcpy(matched_order, new_order, sizeof(Order));
                        matched_order -> quantity = temp -> order -> quantity;

                        addorder_to_orderBook(orderBook, matched_order, temp -> order);
                        new_order -> quantity = (new_order -> quantity) - (temp -> order -> quantity);
                        delete_sell_order(sell_tree, temp -> order, &sell_tree->root);

                        printf("The order is matched.\n");
                        display_matched_order(orderBook);

                        temp = sell_tree -> root; // run again while loop to match remaining new-order quantity
                        flag = 1;

                    }

                }

            }

        }

    } else {
        // newly generated order is sell-order

        // initially when there is no buy-order placed 
        if(buy_tree -> root == NULL) {

            insert_sell_order(sell_tree, new_order);
            printf("There is no buy-order placed by this time.\n");

            return;
        }

        Node *temp = buy_tree -> root;
        // when there is single order in the buy-order
        if((temp -> lchild) == NULL && (temp -> rchild) == NULL) {

            // when the sell order doesn't match with the only buy-order
            if((new_order -> price) != (temp -> order -> price)) {

                insert_sell_order(sell_tree, new_order);
                printf("There is no match for the given sell-order in the buy-order\n");

                return;

            } else {

                // when the sell-order-quantity matches with the buy-order-quantity
                if((temp -> order -> quantity) == (new_order -> quantity)) {

                    addorder_to_orderBook(orderBook, temp -> order, new_order);
                    delete_buy_order(buy_tree, temp -> order, &buy_tree->root);
                    printf("The order is matched.\n");
                    display_matched_order(orderBook);

                } else if((temp -> order -> quantity) > (new_order -> quantity)) {

                    Order *matched_order = (Order *)malloc(sizeof(Order));
                    memcpy(matched_order, temp -> order, sizeof(Order));
                    matched_order -> quantity = new_order -> quantity;

                    addorder_to_orderBook(orderBook, matched_order, new_order);
                    temp -> order -> quantity = (temp -> order -> quantity) - (new_order -> quantity);
                    printf("The order is matched.\n");
                    display_matched_order(orderBook);

                } else {

                    Order *matched_order = (Order *)malloc(sizeof(Order));
                    memcpy(matched_order, new_order, sizeof(Order));
                    matched_order -> quantity = temp -> order -> quantity;

                    addorder_to_orderBook(orderBook, temp -> order, matched_order);
                    new_order -> quantity = (new_order -> quantity) - (temp -> order -> quantity);
                    insert_sell_order(sell_tree, new_order);
                    delete_buy_order(buy_tree, temp -> order, &buy_tree->root);

                    printf("The order is matched.\n");
                    display_matched_order(orderBook);
                    
                }

                return;

            }

        } else {

            int flag = 0;  // used to match orders repeatedly until new-order gets over

            while(1) {

                    while((temp != NULL) && ((new_order -> price) != (temp -> order -> price))) {

                    if((temp -> order -> price) > (new_order -> price)) {
                        temp = temp -> lchild;
                    } else {
                        temp = temp -> rchild;
                    }

                }

                if(temp == NULL) {

                    insert_sell_order(sell_tree, new_order);
                    
                    // not a single order match
                    if(flag == 0) {
                        printf("There is no match for the given buy-order in the sell-order\n");
                    }

                    return;

                } else {

                    if((temp -> order -> quantity) == (new_order -> quantity)) {

                        addorder_to_orderBook(orderBook, temp -> order, new_order);
                        delete_buy_order(buy_tree, temp -> order, &buy_tree->root);
                        printf("The order is matched.\n");
                        display_matched_order(orderBook);

                        return;

                    } else if((temp -> order -> quantity) > (new_order -> quantity)) {

                        Order *matched_order = (Order *)malloc(sizeof(Order));
                        memcpy(matched_order, temp -> order, sizeof(Order));
                        matched_order -> quantity = new_order -> quantity;

                        addorder_to_orderBook(orderBook, matched_order, new_order);
                        temp -> order -> quantity = (temp -> order -> quantity) - (new_order -> quantity);
                        printf("The order is matched.\n");
                        display_matched_order(orderBook);

                        return;

                    } else {

                        Order *matched_order = (Order *)malloc(sizeof(Order));
                        memcpy(matched_order, new_order, sizeof(Order));
                        matched_order -> quantity = temp -> order -> quantity;

                        addorder_to_orderBook(orderBook, temp -> order, matched_order);
                        new_order -> quantity = (new_order -> quantity) - (temp -> order -> quantity);
                        delete_buy_order(buy_tree, temp -> order, &buy_tree->root);

                        printf("The order is matched.\n");
                        display_matched_order(orderBook);

                        temp = buy_tree -> root; // run again while loop to match remaining new-order quantity
                        flag = 1;

                    }

                }

            }

        }

    }

}
