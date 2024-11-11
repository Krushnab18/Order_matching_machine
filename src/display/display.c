#include"display.h"
#include<stdio.h>
#include<string.h>
#include<stdbool.h>

void print_menu(){
    printf("'A' -> Login \n"
           "'B' -> New registration\n"
        );
    return;
}

bool login(char *name){
    FILE *file = fopen("user.csv", "r");

    if(file == NULL)
        return false;
    
    char buffer[1024];
    char *data;
    User user[1024];
    int record = 0;

    fgets(buffer, sizeof(buffer), file);

    while(fgets(buffer, sizeof(buffer), file)){
        data = strtok(buffer, ",");
        strcpy(user[record].user_name, data);

        data = strtok(NULL, ",");
        strcpy(user[record].password, data);

        data = strtok(NULL, ",");
        strcpy(user[record].gmail, data);

        data = strtok(NULL, ",");
        strcpy(user[record].mobile, data);

        record++;
    }


    char password[64];
    bool result = false;

    printf("Enter User Name: ");
    scanf("%s", name);

    int i;
    for(i = 0; i < record; i++){
        if(strcmp(name, user[i].user_name) == 0){
            result = true;
            break;
        }
    }

    if(!result){
        printf("Invalid User name\n");
        return false;
    }

    printf("Enter Password: ");
    scanf("%s", password);

    result = false;

    if((strcmp(password, user[i].password) == 0))
        result = true;

    if(!result){
        printf("Invalid Password\n");
        return false;
    }

    printf("Login Successful !!\n");

    
    return true;
}

void new_reg(char *name){
    
    FILE *file;
    
    file = fopen("user.csv", "a+");

    if(file == NULL)
        return;
    
    User user;

    printf("Enter user name : ");
    scanf("%s", user.user_name);

    printf("\nEnter Password : ");
    scanf("%s", user.password);

    printf("\nEnter Gmail : ");
    scanf("%s", user.gmail);

    printf("\nEnter Mobile No : ");
    scanf("%s", user.mobile);

    fprintf(file , 
            "%s,%s,%s,%s\n",
            user.user_name, 
            user.password,
            user.gmail,
            user.mobile);

    fclose(file);

    strcpy(name, user.user_name);

    return;
}

void add_order(Order *order, char *name) {
    printf("Enter the Quantity: ");
    scanf("%d", &order->quantity);
    
    // Prompt and read 'B' or 'S' for side
    printf("'B' for Buy\n'S' for Sell\nEnter your choice: ");
    getchar();  // Consume any lingering newline or whitespace
    scanf("%c", &order->side);
    
    printf("Enter the Price: ");
    scanf("%lf", &order->price);

    // Get current time for timestamp
    struct timeval tv;
    gettimeofday(&tv, NULL);
    order->time_stamp = tv;

    // Open the file in append mode
    FILE *file = fopen("order.csv", "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Write the order details to the CSV file
    fprintf(file, "%s,%d,%.2lf,%c,%ld.%06ld\n",
            name,
            order->quantity,
            order->price,
            order->side,
            (long)order->time_stamp.tv_sec,
            (long)order->time_stamp.tv_usec);

    // Close the file after writing
    fclose(file);
    
    printf("Order added successfully.\n");
}
void display(Order *order){
    print_menu();
    char choice; 
    char name[64];

    scanf("%c", &choice);
    switch (choice){
        case 'A' :
            if(!(login(name)))
                return;
            break;
        
        case 'B' :
            new_reg(name);
            break;

        default:
        printf("Invalid choice");
    }

    add_order(order, name);


    printf("Order ID: %lld, Price: %.2f, time_stamp: %ld.%06ld\n",
               order->order_id,
               order->price,
               (long)order->time_stamp.tv_sec,
               (long)order->time_stamp.tv_usec);
    return;
}
