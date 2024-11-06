#include "order.h"

typedef struct Node{
    Order *order;
    struct Node *lchild, *rchild;
    int height;
}Node;

typedef struct buy_order_avl{
    Order *max_order;
    Node *root;
}buy_order_avl;


typedef struct sell_order_avl{
    Order *min_order;
    Node *root;
}sell_order_avl;


init_buy_order_avl(buy_order_avl *t);
init_sell_order_avl(sell_order_avl *t);
Node *createNode(Order *order);
int getHeight(Node *node);
int getBf(Node *node);
int max(int a, int b);

Node *leftRotate(Node *y);
Node *rightRotate(Node *x);

void insert_buy_order(buy_order_avl *t, Order *order);
void insert_sell_order(sell_order_avl *t, Order *order);
void delete_buy_order(buy_order_avl *t, Order *order);
void delete_sell_order(sell_order_avl *t, Order *order);

void inorder_buy_order(Node *node);
void inorder_sell_order(Node *node);
