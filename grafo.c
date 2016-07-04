#include "grafo.h"

void CreateFree(Free *f){
    f->first = (Pointer) malloc(sizeof(FreeCell));
    f->last = f->first;
    f->first->next = NULL;
}

void CreateFreeList(FreeList *fl, int nvertices){
    int i; 
    fl->size = nvertices;
    fl->f = (Free *) malloc (fl->size*sizeof(Free));
    for (i = 0; i < fl->size; i++){
        CreateFree(&fl->f[i]);
    }
}

short IsEmpty(Free f){
    return (f.first == f.last);
}

void InsertFreeIntern(int color, Free *f, TableCell *tcell){
    tcell->cPointer = f->last;
    f->last->next = (Pointer) malloc(sizeof(FreeCell));
    f->last = f->last->next;
    f->last->color = color;
    f->last->next = NULL;
}

void InsertFree(int v, int color, FreeList *fl, TableCell *tcell){
    InsertFreeIntern(color, &fl->f[v], tcell);
}

short RemoveFreeIntern(Free *f, Pointer p, int *color, TableCell *tcell){
    Pointer nextP;
    if (IsEmpty(*f) || p == NULL || p->next == NULL){
        printf("\nErro: Lista vazia ou posicao inexistente");
        return -1;
    }
    nextP = p->next;
    *color = nextP->color;
    p->next = nextP->next;
    if (p->next == NULL)
        f->last = p;
    free(nextP);

    tcell->cPointer = NULL;
    return 1;
}

short RemoveFree(FreeList *fl, int v, Pointer p, int *color, TableCell *tcell){
    return RemoveFreeIntern(&fl->f[v], p, color, tcell);
}

void CreateEdgeList(EdgeList *elist, int nedges){
    elist->edge = (Edge *) malloc(nedges*sizeof(Edge));
    elist->size = 0;
}

void AddEdge(int v, int w, int color, EdgeList *elist){
    Edge edge;
    edge.v = v;
    edge.w = w;
    edge.color = color;
    elist->edge[elist->size] = edge;
    elist->size++;
}

void CreateTableCell(TableCell *tcell){
    tcell->status = FALSE;
    tcell->adj = -1;
    tcell->cPointer = NULL;    
}

void CreateTable(Table *table, int nvertices, int ncolors){
    int i, j;
    table->sizeX = nvertices;
    table->sizeY = ncolors;

    table->matrix = (TableCell **) malloc(table->sizeX*sizeof(TableCell *));
    for (i = 0; i < table->sizeX; i++){
        table->matrix[i] = (TableCell *) malloc(table->sizeY*sizeof(TableCell));
        for (j = 0; j < table->sizeY; j++){
            table->matrix[i][j].status = FALSE;
            table->matrix[i][j].adj = -1;
            table->matrix[i][j].cPointer = NULL;
            /* Pode trocar por uma chamada a CreateTableCell, mas é um gasto a mais de tempo */
        }
    }    
}

void CreateAdjMatrix(AdjMatrixGraph *graph, int nvertices){
    int i;
    
    graph->n = nvertices;
    graph->matrix = (int **) malloc (graph->n*sizeof(int *)); 
    for (i = 0; i < graph->n; i++){
        graph->matrix[i] = (int *) malloc (graph->n*sizeof(int));
    }
}

void InitWithAdjMatrix(AdjMatrixGraph *graph, EdgeList *edge_list, FreeList *free_list, Table *table){
    int i, j;
    CreateFreeList(free_list, graph->n);
    CreateEdgeList(edge_list, graph->m);
    for (i = 0; i < graph->n; i++){
        for (j = 0; j < i; j++){
            if (graph->matrix[i][j] == 1){
                AddEdge(i, j, NULO, edge_list); /* Adciona aresta descolorida ao EdgeList */
            }
        }
    }
    CreateTable(table, graph->n, graph->delta);
    for (i = 0; i < table->sizeX; i++){
        for (j = 0; j < table->sizeY; j++){
            table->matrix[i][j].status = FALSE;
            table->matrix[i][j].adj = NULO;
            table->matrix[i][j].cPointer = NULL;
        }
    }
}