#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "io.h"


int readInt(FILE **file, int *n){
    return (fscanf(*file, "%d", n) != EOF);
}

void readAdjMatrix(char *file_name, AdjMatrixGraph *graph){
    int i, j, num;
    int degree, nedges;
    FILE *file;


    file = fopen(file_name, "r");
    if (!file){
        printf("\nHouve um problema com o arquivo [%s]", file_name);
        exit(0);
    }

    num = 0;
    nedges = 0;
    readInt(&file, &graph->n);
    printf ("n = %d\n", graph->n);

    CreateAdjMatrix(graph, graph->n);
    for (i = 0; i < graph->n; i++){
        for (j = 0; j < graph->n; j++){
            readInt(&file, &num);
            printf ("%d  ", num);
            nedges += num;
            graph->matrix[i][j] = num;
        }
        printf("\n");
    }
    
    graph->m = nedges/2;
    printf ("\nm = %d\n", graph->m);

    graph->delta = 0; /* maximo grau do grafo */
    for (i = 0; i < graph->n; i++){
        degree = 0;
        for (j = 0; j < graph->n; j++){
            degree += graph->matrix[i][j];
        }
        if (degree > graph->delta){
            graph->delta = degree;
        }
    }
}

void printAdjMatrixGraph(AdjMatrixGraph graph){
    int i, j;
    printf("%d x %d [delta = %d]\n", graph.n, graph.m, graph.delta);
    for (i = 0; i < graph.n; i++){
        for (j = 0; j < graph.n; j++){
        printf("%d ", graph.matrix[i][j]);
        }
        printf("\n");
    }
}

void printEdgeList(EdgeList edge_list){
    int i;

    printf("\nEdge List: ");
    for (i = 0; i < edge_list.size; i++){
        printf("\n(%d, %d): %d", edge_list.edge[i].v, edge_list.edge[i].w, edge_list.edge[i].color);
    }
}

void printFreeList(FreeList free_list){
    int i;
    Pointer p; 

    printf("\nFree List: ");
    for (i = 0; i < free_list.size; i++){
        printf("\nVertex %d: ", i);
        if (!IsEmpty(free_list.f[i])){
            p = free_list.f[i].first->next;
            while (p != NULL){
                printf("%d ", p->color);
                p = p->next;
            }
        }
    }
}

void printTable(Table table){
    int i, j;
    printf("\nTable (%d, %d):", table.sizeX, table.sizeY);
    for (i = 0; i < table.sizeX; i++){
        for (j = 0; j < table.sizeY; j++){
            printf("\n(v = %d, c = %d) status = %d | adj = %d | c = %d", i, j, table.matrix[i][j].status, table.matrix[i][j].adj, table.matrix[i][j].cPointer->next->color);
        }
    } 
}

void printGraph(EdgeList edge_list, FreeList free_list, Table table){
    
    printEdgeList(edge_list);

    printFreeList(free_list);

    printTable(table);   
}