typedef struct Time{
    short hours;
    short minute;
    short second;
}Time;

typedef struct Order{
    long long order_id;
    int quantity;
    double price;
    Time time_stamp;
    char stock_symbol[15];
    char side;
}Order;

void create_new_order(long long order_id, int quantity, double price, Time time_stamp, char *stock_symbol, char side);
void print_order(const Order order);
void free_order(Order *order);
int compareOrders(const Order* order1, const Order* order2);
