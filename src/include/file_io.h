#ifndef FILE_IO
#define FILE_IO

#include <sys/time.h>
#include <stdio.h>
#include "../include/order.h"
#include "../include/order_book.h"
#include "../include/avl_tree.h"

void convert_to_timeval(const char *time_str, struct timeval *tv);
void readcsv(const char *filename, sell_order_avl *sell_tree, buy_order_avl *buy_tree, Order_book *order_book, FILE *fp);
void openFile(FILE *fp, const char *fileName);
void writeToCSV(FILE *fp, Order *buy, Order *sell);

#endif // CSV_HANDLER_H
