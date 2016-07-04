#include "grafo.h"


/****************** Operações de Free e FreeList ******************/
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
    tcell->status = FALSE;
    tcell->cPointer = f->last;

    f->last->next = (Pointer) malloc(sizeof(FreeCell));
    f->last = f->last->next;
    f->last->color = color;
    f->last->next = NULL;
}

void InsertFree(int v, int color, FreeList *fl, Table *table){
    InsertFreeIntern(color, &fl->f[v], &table->matrix[v][color]);
}

short RemoveFreeIntern(Free *f, int *color, Pointer p, TableCell *tcell){
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

short RemoveFree(FreeList *fl, int v, int *color, Pointer p, Table *tcell){
    return RemoveFreeIntern(&fl->f[v], p, color, tcell);
}

int RemoveFreeByColor(FreeList *fl, int v, int color, Table *table){
    Pointer prev, p;
    Free f;
    TableCell *tcell;

    tcell = &table->matrix[v][color];
    RemoveFreeIntern(&fl->f[v], &color, tcell->cPointer, tcell);
}

short IsFreeColor(int v, int color, FreeList free_list){
    Free f;
    Pointer p;
    f = free_list.f[v];
    if (IsEmpty(f))
        return FALSE;
    p = f.first->next;
    while (p != NULL){
        if (p->color == color){
            return TRUE;
        }
        p = p->next;
    }
    return FALSE;
}

int GetCommonFreeColor(Edge edge, FreeList free_list){
    int v, color;
    Free f;
    Pointer p;

    v = edge.v;
    f = free_list.f[v];
    if (IsEmpty(f))
        return FALSE;
    p = f.first->next;
    while (p != NULL){
        color = p->color;
        if (IsFreeColor(edge.w, color, free_list))
            return color;
        p = p->next;
    }
    return NULO;
}

int GetFreeColor(int v, FreeList free_list){
    Free f; 

    f = free_list.f[v];
    if (!IsEmpty(f)){
        return f->first->next->color;
    }
    else{
        return NULO;
    }
}

int GetFreeColorNotTaboo(int v, FreeList free_list, int taboo){
    Free f;
    Pointer p;

    f = free_list.f[v];
    if (IsEmpty(f))
        return FALSE;
    
    p = f.first->next;
    while (p != NULL){
        if (p->color != taboo)
            return color;
        p = p->next;
    }
    return NULO;
}

/****************** Operacoes de Edge e EdgeList ******************/
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

Edge GetEdge(int v, int w, EdgeList elist){
    index = GetIndexOfEdge(v, w, elist);
    return elist.edge(index);
}

int GetIndexOfEdge(int v, int w, EdgeList elist){
    int i; 
    for (i = 0; i < elist.size; i++){
        if ((elist.edge[i].v == v) && (elist.edge[i].w == w)){
            return i; 
        }
        if ((elist.edge[i].v == w) && (elist.edge[i].w == v)){
            return i
        }
    }
    return -1;
}

int GetColorOfEdge(int v, int w, EdgeList elist){
    Edge edge;
    edge = GetEdge(v, w, elist);
    if (edge != NULL)
        return edge.color;
    else
        return -1;
}

Edge GetColorlessEdge(EdgeList elist){
    index = GetIndexOfColorlessEdge(elist);
    return elist.edge(index);
}

int GetIndexOfColorlessEdge(EdgeList elist){
    int i; 
    for (i = 0; i < elist.size; i++){
        if (elist.edge[i].color = -1){
            return i;
        }
    }
    return NULL;
}

Edge IsTottalyColored(EdgeList elist){
    return (GetColorlessEdge(elist) == NULL);
}

void coloringEdge(int color, int iEdge, EdgeList *edge_list, FreeList *free_list, Table *table){
    Edge edge;
    edge_list->edge[iEdge].color = color;
    edge = edge_list->edge[iEdge];

    RemoveFreeByColor(free_list, edge.v, edge.color, table);
    RemoveFreeByColor(free_list, edge.w, edge.color, table);
    UpdateInTable(edge.v, edge.w, edge.color, iEdge, table);    
}


/****************** Operações de Table ******************/
void CreateTableCell(TableCell *tcell){
    tcell->status = FALSE;
    tcell->adj = -1;
    tcell->cPointer = NULL;    
}

void CreateTable(Table *table, int nvertices, int delta){
    int i, j;
    int ncolors;
    ncolors = delta + 1;
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

void UpdateInTable(int v, int w, int color, int iEdge, Table *table){
    table->matrix[v][color].status = TRUE;
    table->matrix[v][color].adj = iEdge;
    table->matrix[v][color].cPointer = NULL;

    table->matrix[w][color].status = TRUE;
    table->matrix[w][color].adj = iEdge;
    table->matrix[w][color].cPointer = NULL;
}

/****************** Operações de AdjMatrix ******************/
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
        /* Adiciona arestas apenas uma vez. 
        Para adicionar duas basta colocar j < graph->n */
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