#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./src/include/order.h"
#include "./src/include/order_book.h"
#include "./src/include/avl_tree.h"
#include "./src/include/generate_order.h"
#include "./src/include/display.h"

int main() {

    buy_order_avl buy_tree;
    sell_order_avl sell_tree;

    init_buy_order_avl(&buy_tree);
    init_sell_order_avl(&sell_tree);

    Order_book order_book;
    init_order_book(&order_book);


    for(int i = 0; i < 100; i++) {
        Order *buy_order = create_unique_order('B');
        matching_fifo(&buy_tree, &sell_tree, buy_order, &order_book);

        Order *sell_order = create_unique_order('S');
        matching_fifo(&buy_tree, &sell_tree, sell_order, &order_book);

    }

    printf("These is inorder traversal of buy tree: \n");
    inorder_buy_order(buy_tree.root);
    printf("These is inorder traversal of sell tree\n");
    inorder_sell_order(sell_tree.root);


    return 0;
}
