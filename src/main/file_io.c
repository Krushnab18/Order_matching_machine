#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/order.h"
#include "../include/order_book.h"
#include "../include/avl_tree.h"
#include "../include/file_io.h"
#include "../include/matching.h"


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
void readcsv(const char *filename, sell_order_avl *sell_tree, buy_order_avl *buy_tree, Order_book *order_book, FILE *fp) {

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
        

        matching_fifo(buy_tree, sell_tree, order, order_book, fp);
        // Output the extracted data
       /*printf("Side: %c, Order ID: %lld, Timestamp: %s, Timeval (sec: %ld, usec: %ld), Price: %.5f, Quantity: %d\n", 
                order.side, order.order_id, order.timestamp, (long)order.time_stamp.tv_sec, 
                (long)order.time_stamp.tv_usec, order.price, order.quantity); */
    }

    fclose(file);
}

void openFile(FILE *fp, const char *fileName) {
    if (fp == NULL) {
        fp = fopen(fileName, "w");
        if (fp == NULL) {
            printf("Error opening file");
            exit(1);
        }
    }
    return;
}

void writeToCSV(FILE *fp, Order *buy, Order *sell) {
    
    fprintf(fp, "%lld,%lld,%d,%lf\n", buy -> order_id, sell -> order_id, buy -> quantity, buy -> price);
    return;

}
