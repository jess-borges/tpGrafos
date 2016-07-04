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
    int degree;
    FILE *file;

    file = fopen(file_name, "r");
    if (!file){
        printf("\nHouve um problema com o arquivo");
        exit(0);
    }
    readInt(&file, &graph->n);
    readInt(&file, &graph->m);
    for (i = 0; i < graph->n; i++){
        for (j = 0; j < graph->m; j++){
            readInt(&file, &num);
            graph->matrix[i][j] = num;
        }
    }

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

void printGraph(AdjMatrixGraph graph){
    int i, j;
    printf("%d x %d [delta = %d]", graph->n, graph->m, graph->delta);
    for (i = 0; i < graph->n; i++){
        for (j = 0; j < graph->n; j++){
        printf("%d ", graph->matrix[i][j])
        }
        printf("\n");
    }
}

void printGraph(EdgeList edge_list, FreeList free_list, Table table){
    int i, j;
    Pointer p;
    printf("\nEdge List: ");
    for (i = 0; i < edge_list.size; i++){
        printf("\n(%d, %d): %d", edge_list[i].v, edge_list[i].w, edge_list[i].color);
    }

    printf("\nFree List: ");
    for (i = 0; i < free_list.size; i++){
        printf("\nVertex %d: ", i);
        if (!IsEmpty(free_list[i])){
            p = free_list[i].first;
            while (p != free_list[i].last){
                printf("%d ", p->color);
                p = p->next;
            }
        }
    }

    printf("\nTable (%d, %d):", table.sizeX, table.sizeY);
    for (i = 0; i < table.sizeX; i++){
        for (j = 0; j < table.sizeY; j++){
            printf("\n(v = %d, c = %d) status = %d | adj = %d", table[i][j].status, table[i][j].adj);
        }
    }    
}