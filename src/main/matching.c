#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/order.h"
#include "../include/order_book.h"
#include "../include/avl_tree.h"
#include "../include/file_io.h"


void matching_fifo(buy_order_avl *buy_tree, sell_order_avl *sell_tree, Order *new_order, Order_book *orderBook, FILE *fp) {

    if(new_order -> side == 'B') {
        // newly generated order is buy-order

        // initially when there is no sell-order placed 
        if(sell_tree -> root == NULL) {

            insert_buy_order(buy_tree, new_order);

            return;//There is no sell-order placed by this time.
        }

        Node *temp = sell_tree -> root;
        // when there is single order in the sell-order 
        if((temp -> lchild) == NULL && (temp -> rchild) == NULL) {

            // when the buy order doesn't match with the only sell-order
            if((new_order -> price) != (temp -> order -> price)) {

                insert_buy_order(buy_tree, new_order);

                return; //There is no match for the given buy-order in the sell-order

            } else {

                // when the buy-order-quantity matches with the sell-order-quantity
                if((temp -> order -> quantity) == (new_order -> quantity)) {

                    addorder_to_orderBook(orderBook, new_order, temp -> order);
                    writeToCSV(fp, new_order, temp->order);
                    delete_sell_order(sell_tree, temp -> order, &sell_tree->root);
                    display_matched_order(orderBook);
                    return;//order is matched

                } else if((temp -> order -> quantity) > (new_order -> quantity)) {

                    Order *matched_order = (Order *)malloc(sizeof(Order));
                    memcpy(matched_order, temp -> order, sizeof(Order));
                    matched_order -> quantity = new_order -> quantity;

                    temp -> order -> quantity = (temp -> order -> quantity) - (new_order -> quantity);
                    addorder_to_orderBook(orderBook, new_order, matched_order);
                    writeToCSV(fp, new_order, matched_order);
                    display_matched_order(orderBook); //order matched

                } else {

                    Order *matched_order = (Order *)malloc(sizeof(Order));
                    memcpy(matched_order, new_order, sizeof(Order));
                    matched_order -> quantity = temp -> order -> quantity;

                    addorder_to_orderBook(orderBook, matched_order, temp -> order);
                    writeToCSV(fp, matched_order, temp->order);
                    new_order -> quantity = (new_order -> quantity) - (temp -> order -> quantity);
                    insert_buy_order(buy_tree, new_order);
                    delete_sell_order(sell_tree, temp -> order, &sell_tree->root);

                    display_matched_order(orderBook);
                    
                }

                return;

            }

        } else {


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

                    return;

                } else {

                    if((temp -> order -> quantity) == (new_order -> quantity)) {

                        addorder_to_orderBook(orderBook, new_order, temp -> order);
                        writeToCSV(fp, new_order, temp->order);
                        delete_sell_order(sell_tree, temp -> order, &sell_tree->root);
                        display_matched_order(orderBook);
                        return;

                    } else if((temp -> order -> quantity) > (new_order -> quantity)) {

                        Order *matched_order = (Order *)malloc(sizeof(Order));
                        memcpy(matched_order, temp -> order, sizeof(Order));
                        matched_order -> quantity = new_order -> quantity;

                        addorder_to_orderBook(orderBook, new_order, matched_order);
                        writeToCSV(fp, new_order, matched_order);
                        temp -> order -> quantity = (temp -> order -> quantity) - (new_order -> quantity);
                        display_matched_order(orderBook);

                        return;

                    } else {

                        Order *matched_order = (Order *)malloc(sizeof(Order));
                        memcpy(matched_order, new_order, sizeof(Order));
                        matched_order -> quantity = temp -> order -> quantity;

                        addorder_to_orderBook(orderBook, matched_order, temp -> order);
                        writeToCSV(fp, matched_order, temp->order);
                        new_order -> quantity = (new_order -> quantity) - (temp -> order -> quantity);
                        delete_sell_order(sell_tree, temp -> order, &sell_tree->root);

                        display_matched_order(orderBook);

                        temp = sell_tree -> root; // run again while loop to match remaining new-order quantity

                    }

                }

            }

        }

    } else {
        // newly generated order is sell-order

        // initially when there is no buy-order placed 
        if(buy_tree -> root == NULL) {

            insert_sell_order(sell_tree, new_order);

            return;
        }

        Node *temp = buy_tree -> root;
        // when there is single order in the buy-order
        if((temp -> lchild) == NULL && (temp -> rchild) == NULL) {

            // when the sell order doesn't match with the only buy-order
            if((new_order -> price) != (temp -> order -> price)) {

                insert_sell_order(sell_tree, new_order);

                return;

            } else {

                // when the sell-order-quantity matches with the buy-order-quantity
                if((temp -> order -> quantity) == (new_order -> quantity)) {

                    addorder_to_orderBook(orderBook, temp -> order, new_order);
                    writeToCSV(fp, temp->order, new_order);
                    delete_buy_order(buy_tree, temp -> order, &buy_tree->root);
                    display_matched_order(orderBook);

                } else if((temp -> order -> quantity) > (new_order -> quantity)) {

                    Order *matched_order = (Order *)malloc(sizeof(Order));
                    memcpy(matched_order, temp -> order, sizeof(Order));
                    matched_order -> quantity = new_order -> quantity;

                    addorder_to_orderBook(orderBook, matched_order, new_order);

                    writeToCSV(fp, matched_order, new_order);
                    temp -> order -> quantity = (temp -> order -> quantity) - (new_order -> quantity);
                    display_matched_order(orderBook);

                } else {

                    Order *matched_order = (Order *)malloc(sizeof(Order));
                    memcpy(matched_order, new_order, sizeof(Order));
                    matched_order -> quantity = temp -> order -> quantity;

                    addorder_to_orderBook(orderBook, temp -> order, matched_order);
                    writeToCSV(fp, temp->order, matched_order);
                    new_order -> quantity = (new_order -> quantity) - (temp -> order -> quantity);
                    insert_sell_order(sell_tree, new_order);
                    delete_buy_order(buy_tree, temp -> order, &buy_tree->root);

                    display_matched_order(orderBook);
                    
                }

                return;

            }

        } else {


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
                    
                    return;

                } else {

                    if((temp -> order -> quantity) == (new_order -> quantity)) {

                        addorder_to_orderBook(orderBook, temp -> order, new_order);
                        writeToCSV(fp, temp->order, new_order);
                        delete_buy_order(buy_tree, temp -> order, &buy_tree->root);
                        display_matched_order(orderBook);

                        return;

                    } else if((temp -> order -> quantity) > (new_order -> quantity)) {

                        Order *matched_order = (Order *)malloc(sizeof(Order));
                        memcpy(matched_order, temp -> order, sizeof(Order));
                        matched_order -> quantity = new_order -> quantity;

                        addorder_to_orderBook(orderBook, matched_order, new_order);
                        writeToCSV(fp, matched_order, new_order);
                        temp -> order -> quantity = (temp -> order -> quantity) - (new_order -> quantity);
                        display_matched_order(orderBook);

                        return;

                    } else {

                        Order *matched_order = (Order *)malloc(sizeof(Order));
                        memcpy(matched_order, new_order, sizeof(Order));
                        matched_order -> quantity = temp -> order -> quantity;

                        addorder_to_orderBook(orderBook, temp -> order, matched_order);
                        writeToCSV(fp, temp->order, matched_order);
                        new_order -> quantity = (new_order -> quantity) - (temp -> order -> quantity);
                        delete_buy_order(buy_tree, temp -> order, &buy_tree->root);

                        display_matched_order(orderBook);

                        temp = buy_tree -> root; // run again while loop to match remaining new-order quantity

                    }

                }

            }

        }

    }

}
