#include "../include/avl_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/order.h"
#include <unistd.h>

void init_buy_order_avl(buy_order_avl *t) {
    t->max_order = NULL;
    t->root = NULL;
    return;
}

void init_sell_order_avl(sell_order_avl *t) {
    t->min_order = NULL;
    t->root = NULL;
    return;
}

Node *createNode(Order *order) {

    Node *newNode = (Node*)malloc(sizeof(Node));

    if(!newNode) {
        printf("New node not created, malloc failed!\n");
        return NULL;
    }

    newNode->order = order;
    newNode->lchild = NULL;
    newNode->rchild = NULL;
    newNode->height = 1;

    return newNode;
}

int getHeight(Node *node) {

    if(!node) {
        return 0;
    }

    return node->height;
}


int max(int a, int b) {
    return a > b ? a : b;
}

// Helper function to get balance factor of a node
int getBalance(Node *node) {
    return node ? getHeight(node->lchild) - getHeight(node->rchild) : 0;
}

// Helper function to update height
void updateHeight(Node *node) {
    if (node) {
        node->height = 1 + max(getHeight(node->lchild), getHeight(node->rchild));
    }
}

// Right rotation for balancing
Node* rotateRight(Node *y) {
    Node *x = y->lchild;
    Node *T2 = x->rchild;

    x->rchild = y;
    y->lchild = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

// Left rotation for balancing
Node* rotateLeft(Node *x) {
    Node *y = x->rchild;
    Node *T2 = y->lchild;

    y->lchild = x;
    x->rchild = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

// Rebalance the AVL tree
Node* rebalance(Node *node) {
    updateHeight(node);
    int balance = getBalance(node);

    if (balance > 1) {
        if (getBalance(node->lchild) < 0) {
            node->lchild = rotateLeft(node->lchild);
        }
        return rotateRight(node);
    }

    if (balance < -1) {
        if (getBalance(node->rchild) > 0) {
            node->rchild = rotateRight(node->rchild);
        }
        return rotateLeft(node);
    }

    return node;
}

Node* insert_buy_order_recursive(Node *node, Order *order, buy_order_avl *t) {
    if (!node) {
        // Create a new node for the order
        Node *newNode = createNode(order);
        if (!t->max_order || order->price > t->max_order->price ||
            (order->price == t->max_order->price && compareOrders(order, t->max_order) == -1)) {
            t->max_order = order;
        }
        return newNode;
    }

    // Compare the price and the timestamp to decide whether to go left or right
    if (order->price < node->order->price ||
        (order->price == node->order->price && compareOrders(order, node->order) == -1)) {
        node->lchild = insert_buy_order_recursive(node->lchild, order, t);
    } else {
        node->rchild = insert_buy_order_recursive(node->rchild, order, t);
    }

    // Update height of the current node
    updateHeight(node);

    // Rebalance the node if necessary
    return rebalance(node);
}

Node* insert_sell_order_recursive(Node *node, Order *order, sell_order_avl *t) {
    if (!node) {
        // Create a new node for the order
        Node *newNode = createNode(order);
        if (!t->min_order || order->price < t->min_order->price ||
            (order->price == t->min_order->price && compareOrders(order, t->min_order) == -1)) {
            t->min_order = order;
        }
        return newNode;
    }

    // Compare the price and the timestamp to decide whether to go left or right
    if (order->price < node->order->price ||
        (order->price == node->order->price && compareOrders(order, node->order) == -1)) {
        node->lchild = insert_sell_order_recursive(node->lchild, order, t);
    } else {
        node->rchild = insert_sell_order_recursive(node->rchild, order, t);
    }

    // Update height of the current node
    updateHeight(node);

    // Rebalance the node if necessary
    return rebalance(node);
}

void insert_buy_order(buy_order_avl *t, Order *order) {
    t->root = insert_buy_order_recursive(t->root, order, t);
}

void insert_sell_order(sell_order_avl *t, Order *order) {
    t->root = insert_sell_order_recursive(t->root, order, t);
}


int delete_two_children_node(Node *node) {
    if (node == NULL || ((node)->lchild == NULL && (node)->rchild == NULL)) {
        return 0;
    }

    // Find the in-order successor (smallest node in right subtree)
    Node *successorParent = node;
    Node *successor = (node)->rchild;
    while (successor->lchild != NULL) {
        successorParent = successor;
        successor = successor->lchild;
    }

    // Copy successor's order data to the current node
    (node)->order = successor->order;

    // Delete the successor node
    if (successorParent->lchild == successor) {
        successorParent->lchild = successor->rchild;
    } else {
        successorParent->rchild = successor->rchild;
    }
    free(successor);

    return 1;
}

int delete_one_child_node(Node **node) {
    if (*node == NULL) {
        return 0;
    }

    Node *child = (*node)->lchild ? (*node)->lchild : (*node)->rchild;
    free(*node);
    *node = child;
    return 1;
}

int delete_leaf_node(Node **node) {
    if (*node == NULL) {
        return 0;
    }
    free(*node);
    *node = NULL;
    return 1;
}

int delete_buy_order(buy_order_avl *t, Order *order, Node **n) {
    int flag = 0;
    if(*n == NULL) { 
        printf("Node not found\n");
        return 0;
    }

    if((*n)->order->price == order->price && (*n)->order->order_id == order->order_id) {
        if ((*n)->lchild == NULL && (*n)->rchild == NULL) {
            // Node is a leaf
            flag = delete_leaf_node(n);
        } else if ((*n)->lchild == NULL || (*n)->rchild == NULL) {
            // Node has only one child
            flag = delete_one_child_node(n);
        } else {
            // Node has two children
            flag = delete_two_children_node(*n);
        }
    }
    else if((*n)->order->price > order->price ||((*n)->order->price == order->price && compareOrders(order, (*n)->order) == -1)) {
        flag = delete_buy_order(t, order, &(*n)->lchild);
    }

    else if((*n)->order->price < order->price ||((*n)->order->price == order->price && compareOrders(order, (*n)->order) == 1)) {

        flag = delete_buy_order(t, order, &(*n)->rchild);
    }

    if(flag) {
        rebalance(*n);
    }

    return flag;
}


int delete_sell_order(sell_order_avl *t, Order *order, Node **n) {
    int flag = 0;
    if(*n == NULL) { 
        printf("Node not found\n");
        return 0;
    }

    if((*n)->order->price == order->price && (*n)->order->order_id == order->order_id) {
        if ((*n)->lchild == NULL && (*n)->rchild == NULL) {
            // Node is a leaf
            flag = delete_leaf_node(n);
        } else if ((*n)->lchild == NULL || (*n)->rchild == NULL) {
            // Node has only one child
            flag = delete_one_child_node(n);
        } else {
            // Node has two children
            flag = delete_two_children_node(*n);
        }
    }
    else if((*n)->order->price > order->price ||((*n)->order->price == order->price && compareOrders(order, (*n)->order) == -1)) {
        flag = delete_sell_order(t, order, &(*n)->lchild);
    }

    else if((*n)->order->price < order->price ||((*n)->order->price == order->price && compareOrders(order, (*n)->order) == 1)) {

        flag = delete_sell_order(t, order, &(*n)->rchild);
    }

    if(flag) {
        rebalance(*n);
    }

    return flag;
}
    // In-order traversal for buy-order AVL tree
void inorder_buy_order(Node *node) {
    if (node == NULL) {
        return;
    }

    // Traverse left subtree
    inorder_buy_order(node->lchild);

    // Visit the node (print the order)

    printf("Order ID: %lld, Price: %.2f, time_stamp: %ld.%06ld\n",
           node->order->order_id,
           node->order->price,
           (long)node->order->time_stamp.tv_sec,
           (long)node->order->time_stamp.tv_usec);
    // Traverse right subtree
    inorder_buy_order(node->rchild);
}

// In-order traversal for sell-order AVL tree
void inorder_sell_order(Node *node) {
    if (node == NULL) {
        return;
    }

    // Traverse left subtree
    inorder_sell_order(node->lchild);

    // Visit the node (print the order)
    
    printf("Side: %c, Order ID: %lld, Timestamp: %s, Timeval (sec: %ld, usec: %ld), Price: %.5f, Quantity: %d\n", 
                node->order->side, node->order->order_id, node->order->timestamp, (long)node->order->time_stamp.tv_sec, 
                (long)node->order->time_stamp.tv_usec, node->order->price, node->order->quantity);
    /*printf("Order ID: %lld, Price: %.2f, time_stamp: %ld\n",
           node->order->order_id,
           node->order->price,
           (long)node->order->time_stamp.tv_sec);*/


    // Traverse right subtree
    inorder_sell_order(node->rchild);
}


Order *search(buy_order_avl *t, long long order_id, double price) {

    Node *n = t->root;
    while(n) {
        if(n->order->price == price && n->order->order_id == order_id) {
            return n->order;
        }
        else if(n->order->price > price) {
            n = n->lchild;
        }
        else {
            n = n->rchild;
        }
    }
    return NULL;
}

Order *search_sell(sell_order_avl *t, long long order_id, double price) {

    Node *n = t->root;
    while(n) {
        if(n->order->price == price && n->order->order_id == order_id) {
            return n->order;
        }
        else if(n->order->price > price) {
            n = n->lchild;
        }
        else {
            n = n->rchild;
        }
    }
    return NULL;
}
// Example call
