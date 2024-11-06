#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

typedef struct Order {
    long long order_id;
    int quantity;
    double price;
    struct timeval time_stamp;
    char side;
} Order;

// Function to create a new order
Order *create_new_order(long long order_id, int quantity, double price, char side) {
    
    Order *newOrder = (Order*)malloc(sizeof(Order));

    if(!newOrder) {
        printf("Order not created\n");
        return NULL;
    }
    
    newOrder->order_id = order_id;
    newOrder->quantity = quantity;
    newOrder->price = price;
    newOrder->side = side;

    // Get current time with microseconds precision
    gettimeofday(&newOrder->time_stamp, NULL);

    return newOrder;
}

// Function to print an order's details
void print_order(const Order order) {
    printf("Order ID: %lld\n", order.order_id);
    printf("Quantity: %d\n", order.quantity);
    printf("Price: %.2f\n", order.price);
    printf("Side: %c\n", order.side);
    printf("Timestamp: %ld seconds, %ld microseconds\n", order.time_stamp.tv_sec, order.time_stamp.tv_usec);
    return;
}

// Function to compare two orders based on their timestamps
int compareOrders(const Order* order1, const Order* order2) {

    if (order1->time_stamp.tv_sec < order2->time_stamp.tv_sec) {
        return -1; // order1 is earlier
    }
    else if (order1->time_stamp.tv_sec > order2->time_stamp.tv_sec) {
        return 1; // order2 is earlier
    }
    else {
        // If seconds are the same, compare microseconds
        if (order1->time_stamp.tv_usec < order2->time_stamp.tv_usec) {
            return -1; // order1 is earlier
        }
        else if (order1->time_stamp.tv_usec > order2->time_stamp.tv_usec) {
            return 1; // order2 is earlier
        }
        else {
            return 0; // Orders were placed at the exact same time
        }
    }
}

int main() {
    Order order1, order2;

    // Create two new orders with different details
    create_new_order(1001, 50, 250.75, "AAPL", 'B', &order1);
    create_new_order(1002, 75, 245.50, "GOOG", 'S', &order2);

    // Print the orders
    print_order(order1);
    print_order(order2);

    // Compare orders based on timestamp
    int comparison_result = compareOrders(&order1, &order2);
    if (comparison_result == 0) {
        printf("Orders were placed at the same time.\n");
    } else if (comparison_result < 0) {
        printf("Order 1 was placed before Order 2.\n");
    } else {
        printf("Order 2 was placed before Order 1.\n");
    }

    return 0;
}
