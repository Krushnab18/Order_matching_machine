gcc -Wall -c main.c ./src/main/order.c ./src/main/avl_tree.c ./src/main/generate_order.c ./src/main/matching.c ./src/main/order_book.c
cc main.o order.o avl_tree.o generate_order.o  matching.o order_book.o -o sim
