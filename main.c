#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./src/include/order.h"
#include "./src/include/order_book.h"
#include "./src/include/avl_tree.h"
#include "./src/include/generate_order.h"
#include "./src/include/display.h"

#define MAX_LINE_LENGTH 1024

// Function to convert time (HH:MM:SS) to struct timeval
void convert_to_timeval(const char *time_str, struct timeval *tv) {
    int hours, minutes, seconds;
    // Parse the HH:MM:SS time format
    sscanf(time_str, "%d:%d:%d", &hours, &minutes, &seconds);

    // Calculate total seconds from midnight
    (tv)->tv_sec = (hours * 3600) + (minutes * 60) + seconds;
    (tv)->tv_usec = 0;  // Set microseconds to 0 as we only have HH:MM:SS format
}

// Function to read the CSV and extract necessary data
void readcsv(const char *filename, sell_order_avl *sell_tree, buy_order_avl *buy_tree, Order_book *order_book) {

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    
    while (fgets(line, sizeof(line), file)) {
        Order *order = (Order *)malloc(sizeof(Order));
        // Split the line by the delimiter '|'
        char *token = strtok(line, "|");
        // Extract the 'Side' - first token
        order->side = token[0];
        
        // Extract the 'Order ID' - second token
        token = strtok(NULL, "|");
        order->order_id = atoi(token);  // Convert the string to an integer
        
        // Skip the third token ('ADORWELD', which isn't needed)
        strtok(NULL, "|");
        
        // Skip the fourth token ('EQ', which isn't needed)
        strtok(NULL, "|");

        // Extract the 'Timestamp' - fifth token
        token = strtok(NULL, "|");
        strcpy(order->timestamp, token);
        
        // Extract the 'Price' - sixth token
        token = strtok(NULL, "|");
        order->price = atof(token);  // Convert the string to a double
        
        // Extract the 'Quantity' - seventh token
        token = strtok(NULL, "|");
        order->quantity = atof(token);  // Convert the string to a double

        // Convert the timestamp to struct timeval
        convert_to_timeval(order->timestamp, &(order->time_stamp));
        

        matching_fifo(buy_tree, sell_tree, order, order_book);
        // Output the extracted data
       /*printf("Side: %c, Order ID: %lld, Timestamp: %s, Timeval (sec: %ld, usec: %ld), Price: %.5f, Quantity: %d\n", 
                order.side, order.order_id, order.timestamp, (long)order.time_stamp.tv_sec, 
                (long)order.time_stamp.tv_usec, order.price, order.quantity); */
    }

    fclose(file);
}

int main() {

    clock_t start_time = clock();
    sell_order_avl sell_tree;
    buy_order_avl buy_tree;

    init_buy_order_avl(&buy_tree);
    init_sell_order_avl(&sell_tree);

    Order_book order_book;
    init_order_book(&order_book);
    readcsv("data.csv", &sell_tree, &buy_tree, &order_book);

/*
    for(int i = 0; i < 100; i++) {
        Order *buy_order = create_unique_order('B');
        matching_fifo(&buy_tree, &sell_tree, buy_order, &order_book);

        Order *sell_order = create_unique_order('S');
        matching_fifo(&buy_tree, &sell_tree, sell_order, &order_book);

    }
*/
    printf("These is inorder traversal of buy tree: \n");
    inorder_buy_order(buy_tree.root);
    printf("These is inorder traversal of sell tree\n");
    inorder_sell_order(sell_tree.root);
    clock_t end_time = clock();
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Execution time: %.6f seconds\n", time_taken);

    return 0;
}
