#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

int main() {
    
    
    srand(time(NULL));

    printf("**************************  WELCOME TO OUR ORDER MATCHING MACHINE  ******************************\n");
    
    generate_orders();

    while(true) {
        
        //here user would be able to add order, cancel order, veiw order log;
        display_menu();

        // these function would generate  100 random orders with current time_stamp  and add it to the max heap and min heap
        generate_orders();

        // These function would call matching engine and print the matched orders, and add the matched orders to the order log
        match_orders();

        sleep(5); // stops execution for 5 seconds;

    }

    return 0;

}
