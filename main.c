#include "grafo.h"
#include "io.h"

int main (int argc, char** argv){
    AdjMatrixGraph graph;
    Table table;
    FreeList free_list;
    EdgeList edge_list;
    char file_name;

    file_name = "teste/grafo1.txt";

    readAdjMatrix(file_name, &graph);

}