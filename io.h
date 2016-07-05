#include <stdio.h>

#ifndef _H_IO
#define _H_IO

int readInt(FILE **file, int *n);
void readAdjMatrix(char *file_name, AdjMatrixGraph *graph);

void printAdjMatrixGraph(AdjMatrixGraph graph);
void printEdgeList(EdgeList edge_list);
void printFreeList(FreeList free_list);
void printTable(Table table);
void printGraph(EdgeList edge_list, FreeList free_list, Table table);

#endif