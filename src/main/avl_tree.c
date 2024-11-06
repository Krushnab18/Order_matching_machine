#include "../include/avl_tree.h"
#include <stdio.h>
#include <stdlib.h>



init_buy_order_avl(buy_order_avl *t) {
    t->max_order = NULL;
    t->root = NULL;
    return;
}

init_sell_order_avl(sell_order_avl *t) {
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

// Iterative AVL insertion with duplicate handling and empty tree check
void insert_order(buy_order_avl *t, Order *order) {

    Node *newNode = createNode(order);

    if (!t->root) {
        // Tree is empty, insert the first node
        t->root = newNode;
        t->max_order = order; // First inserted node is max for buy orders
        return;
    }

    Node *current = t->root;
    Node *parent = NULL;
    Node *path[100];
    int pathIndex = 0;

    // Traverse the tree iteratively
    while (current) {
        path[pathIndex++] = current;
        parent = current;

        if (order->price < current->order->price ||
           (order->price == current->order->price && (compareOrders(order, current->order) == -1)) {
            current = current->lchild;
        } else {
            // Insert duplicates with equal price based on time_stamp
            current = current->rchild;
        }
    }

    // Insert the new node
    if (order->price < parent->order->price ||
       (order->price == parent->order->price && order->time_stamp < parent->order->time_stamp)) {
        parent->lchild = newNode;
    } else {
        parent->rchild = newNode;
    }

    // Rebalance and update max_order as we backtrack
    while (pathIndex > 0) {
        pathIndex--;
        path[pathIndex] = rebalance(path[pathIndex]);

        if (pathIndex != 0) {
            if (path[pathIndex - 1]->lchild == path[pathIndex + 1]) {
                path[pathIndex - 1]->lchild = path[pathIndex];
            } else {
                path[pathIndex - 1]->rchild = path[pathIndex];
            }
        } else {
            t->root = path[pathIndex];
        }
    }

    // Update max_order for buy orders
    if (!t->max_order || order->price > t->max_order->price ||
       (order->price == t->max_order->price && ((compareOrders(order, t->max_order) == 1)) {
        t->max_order = order;
    } 
}

// Similar insertion for sell orders, maintaining min_order
void insert_sell_order(sell_order_avl *t, Order *order) {

    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->order = order;
    newNode->lchild = newNode->rchild = NULL;
    newNode->height = 1;

    if (!t->root) {
        // Tree is empty, insert the first node
        t->root = newNode;
        t->min_order = order; // First inserted node is min for sell orders
        return;
    }

    Node *current = t->root;
    Node *parent = NULL;
    Node *path[100];
    int pathIndex = 0;

    while (current) {
        path[pathIndex++] = current;
        parent = current;

        if (order->price < current->order->price ||
           (order->price == current->order->price && order->time_stamp < current->order->time_stamp)) {
            current = current->lchild;
        } else {
            // Insert duplicates with equal price based on time_stamp
            current = current->rchild;
        }
    }

    if (order->price < parent->order->price ||
       (order->price == parent->order->price && order->time_stamp < parent->order->time_stamp)) {
        parent->lchild = newNode;
    } else {
        parent->rchild = newNode;
    }

    while (pathIndex > 0) {
        pathIndex--;
        path[pathIndex] = rebalance(path[pathIndex]);

        if (pathIndex != 0) {
            if (path[pathIndex - 1]->lchild == path[pathIndex + 1]) {
                path[pathIndex - 1]->lchild = path[pathIndex];
            } else {
                path[pathIndex - 1]->rchild = path[pathIndex];
            }
        } else {
            t->root = path[pathIndex];
        }
    }

    // Update min_order for sell orders
    if (!t->min_order || order->price < t->min_order->price ||
       (order->price == t->min_order->price && order->time_stamp < t->min_order->time_stamp)) {
        t->min_order = order;
    }
}

void delete_buy_order(buy_order_avl *t, Order *order) {
    if (!t->root) return; // Tree is empty, nothing to delete

    Node *current = t->root;
    Node *parent = NULL;
    Node *path[100];
    int pathIndex = 0;

    // Locate the node to delete, comparing price, time_stamp, and order_id to handle duplicates
    while (current && (current->order->price != order->price || 
                       current->order->time_stamp != order->time_stamp || 
                       current->order->order_id != order->order_id)) {
        path[pathIndex++] = current;
        parent = current;
        if (order->price < current->order->price || 
           (order->price == current->order->price && order->time_stamp < current->order->time_stamp) ||
           (order->price == current->order->price && order->time_stamp == current->order->time_stamp && order->order_id < current->order->order_id)) {
            current = current->lchild;
        } else {
            current = current->rchild;
        }
    }

    // If node not found
    if (!current) return;

    // Case 1: Node has no children (leaf)
    if (!current->lchild && !current->rchild) {
        if (!parent) {
            // Node to delete is the root
            t->root = NULL;
        } else if (parent->lchild == current) {
            parent->lchild = NULL;
        } else {
            parent->rchild = NULL;
        }
        free(current);
    }
    // Case 2: Node has one child
    else if (!current->lchild || !current->rchild) {
        Node *child = (current->lchild) ? current->lchild : current->rchild;
        if (!parent) {
            // Node to delete is the root
            t->root = child;
        } else if (parent->lchild == current) {
            parent->lchild = child;
        } else {
            parent->rchild = child;
        }
        free(current);
    }
    // Case 3: Node has two children
    else {
        // Find in-order successor
        Node *successor = current->rchild;
        Node *successorParent = current;
        path[pathIndex++] = current; // Add current to path

        while (successor->lchild) {
            path[pathIndex++] = successor;
            successorParent = successor;
            successor = successor->lchild;
        }

        // Replace current's order with successor's order
        current->order = successor->order;

        // Remove the successor node
        if (successorParent->lchild == successor) {
            successorParent->lchild = successor->rchild;
        } else {
            successorParent->rchild = successor->rchild;
        }
        free(successor);
    }

    // Rebalance the tree as we backtrack
    while (pathIndex > 0) {
        pathIndex--;
        path[pathIndex] = rebalance(path[pathIndex]);

        if (pathIndex != 0) {
            if (path[pathIndex - 1]->lchild == path[pathIndex + 1]) {
                path[pathIndex - 1]->lchild = path[pathIndex];
            } else {
                path[pathIndex - 1]->rchild = path[pathIndex];
            }
        } else {
            t->root = path[pathIndex];
        }
    }

    // Update max_order
    t->max_order = NULL; // Reset and find max order again
    Node *node = t->root;
    while (node) {
        if (!t->max_order || node->order->price > t->max_order->price ||
           (node->order->price == t->max_order->price && node->order->time_stamp > t->max_order->time_stamp) ||
           (node->order->price == t->max_order->price && node->order->time_stamp == t->max_order->time_stamp && node->order->order_id > t->max_order->order_id)) {
            t->max_order = node->order;
        }
        node = node->rchild; // Move to rightmost node for max order
    }
}

void delete_sell_order(sell_order_avl *t, Order *order) {
    if (!t->root) return; // Tree is empty, nothing to delete

    Node *current = t->root;
    Node *parent = NULL;
    Node *path[100];
    int pathIndex = 0;

    // Locate the node to delete, comparing price, time_stamp, and order_id to handle duplicates
    while (current && (current->order->price != order->price || 
                       current->order->time_stamp != order->time_stamp || 
                       current->order->order_id != order->order_id)) {
        path[pathIndex++] = current;
        parent = current;
        if (order->price < current->order->price || 
           (order->price == current->order->price && order->time_stamp < current->order->time_stamp) ||
           (order->price == current->order->price && order->time_stamp == current->order->time_stamp && order->order_id < current->order->order_id)) {
            current = current->lchild;
        } else {
            current = current->rchild;
        }
    }

    // If node not found
    if (!current) return;

    // Case 1: Node has no children (leaf)
    if (!current->lchild && !current->rchild) {
        if (!parent) {
            // Node to delete is the root
            t->root = NULL;
        } else if (parent->lchild == current) {
            parent->lchild = NULL;
        } else {
            parent->rchild = NULL;
        }
        free(current);
    }
    // Case 2: Node has one child
    else if (!current->lchild || !current->rchild) {
        Node *child = (current->lchild) ? current->lchild : current->rchild;
        if (!parent) {
            // Node to delete is the root
            t->root = child;
        } else if (parent->lchild == current) {
            parent->lchild = child;
        } else {
            parent->rchild = child;
        }
        free(current);
    }
    // Case 3: Node has two children
    else {
        // Find in-order successor
        Node *successor = current->rchild;
        Node *successorParent = current;
        path[pathIndex++] = current; // Add current to path

        while (successor->lchild) {
            path[pathIndex++] = successor;
            successorParent = successor;
            successor = successor->lchild;
        }

        // Replace current's order with successor's order
        current->order = successor->order;

        // Remove the successor node
        if (successorParent->lchild == successor) {
            successorParent->lchild = successor->rchild;
        } else {
            successorParent->rchild = successor->rchild;
        }
        free(successor);
    }

    // Rebalance the tree as we backtrack
    while (pathIndex > 0) {
        pathIndex--;
        path[pathIndex] = rebalance(path[pathIndex]);

        if (pathIndex != 0) {
            if (path[pathIndex - 1]->lchild == path[pathIndex + 1]) {
                path[pathIndex - 1]->lchild = path[pathIndex];
            } else {
                path[pathIndex - 1]->rchild = path[pathIndex];
            }
        } else {
            t->root = path[pathIndex];
        }
    }

    // Update min_order for sell orders
    t->min_order = NULL; // Reset min_order
    Node *node = t->root;
    while (node) {
        if (!t->min_order || node->order->price < t->min_order->price ||
           (node->order->price == t->min_order->price && node->order->time_stamp < t->min_order->time_stamp) ||
           (node->order->price == t->min_order->price && node->order->time_stamp == t->min_order->time_stamp && node->order->order_id < t->min_order->order_id)) {
            t->min_order = node->order;
        }
        node = node->lchild; // Move to leftmost node for min order
    }
}


// In-order traversal for buy-order AVL tree
void inorder_buy_order(Node *node) {
    if (node == NULL) {
        return;
    }
    
    // Traverse left subtree
    inorder_buy_order(node->lchild);
    
    // Visit the node (print the order)
    printf("Order ID: %d, Price: %.2f, TimeStamp: %ld\n", node->order->order_id, node->order->price, node->order->time_stamp);
    
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
    printf("Order ID: %d, Price: %.2f, TimeStamp: %ld\n", node->order->order_id, node->order->price, node->order->time_stamp);
    
    // Traverse right subtree
    inorder_sell_order(node->rchild);
}
int main() {
    return 0;
}
