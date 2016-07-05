#include "grafo.h"
#include "io.h"
#include "coloracao.h"

int main (/*int argc, char** argv*/){
    AdjMatrixGraph graph;
    Table table;
    FreeList free_list;
    EdgeList edge_list;

    char *file_name = "testes/grafo1.txt";

    readAdjMatrix(file_name, &graph);
    InitWithAdjMatrix(&graph, &edge_list, &free_list, &table);
    printAdjMatrixGraph(graph);
    printGraph(edge_list, free_list, table);

    coloring(&edge_list, &free_list, &table);
    printf("\n********** GRAFO COLORIDO **********\n");
    printGraph(edge_list, free_list, table);
    return 0;
}