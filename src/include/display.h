#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>  // For struct timeval
#include "order.h"     // Assuming this contains the Order and User structs

// Function prototypes
void print_menu();
bool number_valid(char *num);
bool login(char *name);
void new_reg(char *name);
void add_order(Order *order, char *name);
void display(Order *order);

#endif // DISPLAY_H
