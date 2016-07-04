#include <stdio.h>

#ifndef _H_IO
#define _H_IO

int readInt(FILE **file, int *n);
void readAdjMatrix(char *file_name, AdjMatrixGraph *graph);
void printGraph(AdjMatrixGraph graph);
void printGraph(EdgeList edge_list, FreeList free_list, Table table);

#endif