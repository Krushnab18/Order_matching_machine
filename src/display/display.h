#include <sys/time.h>
#include<stdbool.h>
#define SIZE 64

typedef struct User{
    //User_Name,Password,Gmail,Mobile
    char user_name[SIZE], password[SIZE], gmail[SIZE], mobile[SIZE];
}User;

typedef struct Record{
    //User_Name,Quantity,price,side
    char user_name[SIZE], side;
    int quantity;
    double price;
}Record;

typedef struct Order {
    long long order_id;
    int quantity;
    double price;
    struct timeval time_stamp; // Correct type for time_stamp
    char side;
}Order;

void display(Order *);

void print_menu();

bool login(char *);

void new_reg(char *);

void add_order(Order *, char *);