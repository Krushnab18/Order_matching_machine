typedef struct Time{
    short hours;
    short minute;
    short second;
}Time;

typedef struct Order {
    long long orderId;
    char side;
    double price;
    int quantity;
    Time time_stamp;
}Order




