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

short RemoveFree(FreeList *fl, int v, int *color, Pointer p, TableCell *tcell){
    return RemoveFreeIntern(&fl->f[v], color, p, tcell);
}

int RemoveFreeByColor(FreeList *fl, int v, int color, Table *table){
    TableCell *tcell;

    if (IsFreeColor(v, color, *fl)){
        tcell = &table->matrix[v][color];
        RemoveFreeIntern(&fl->f[v], &color, tcell->cPointer, tcell);
    }
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
        return f.first->next->color;
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
            return p->color;
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

int GetIndexOfEdge(int v, int w, EdgeList elist){
    int i; 
    for (i = 0; i < elist.size; i++){
        if ((elist.edge[i].v == v) && (elist.edge[i].w == w)){
            return i; 
        }
        if ((elist.edge[i].v == w) && (elist.edge[i].w == v)){
            return i;
        }
    }
    return -1;
}

Edge GetEdge(int v, int w, EdgeList elist){
    int index;
    index = GetIndexOfEdge(v, w, elist);
    return elist.edge[index];
}

int GetOtherVertex(int v, Edge edge){
    if (edge.v == v)
        return edge.w;
    else
        return edge.v;
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
    int index; 
    index = GetIndexOfColorlessEdge(elist);
    return elist.edge(index);
}

int GetIndexOfColorlessEdge(EdgeList elist){
    int i; 
    for (i = 0; i < elist.size; i++){
        if (elist.edge[i].color == -1){
            return i;
        }
    }
    return NULO;
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

/****************** Operações de Path ******************/
void CreatePath(Path *path){
    path->allocated = 1;
    path->size = 0;
    path->iEdges = (int *)malloc (path->allocated*ALLOC_BLOCK*sizeof(int));
}

void AddEdgeToPath(int iEdge, Path *path){
    if (path->size >= path->allocated*ALLOC_BLOCK){
        path->allocated++;
        path->iEdges = (int *)realloc(path, path->allocated*ALLOC_BLOCK*sizeof(int));
    }
    path->iEdges[path->size] = iEdge;
    path->size++;
}

Path TwoColorsPath(int v, int color1, int color2, EdgeList edge_list, Table table){
    Edge edge;
    int iEdge;
    Path path;
    short endOfPath;

    path = NULL;
    endOfPath = FALSE;
    iEdge = table.matrix[v][color2].adj;
    edge = edge_list.edge[iEdge];
    if (edge != NULL){
        AddEdgeToPath(iEdge, &path);
        v = GetOtherVertex(v, edge);
    }
    while (!endOfPath){
        if (table.matrix[v][color1].status){
            iEdge = table.matrix[v][color1].adj;
            edge = edge_list.edge[iEdge];
            AddEdgeToPath(iEdge, &path);
            v = GetOtherVertex(v, edge);
            if (table.matrix[v][color2].status){
                iEdge = table.matrix[v][color2].adj;
                edge = edge_list.edge[iEdge];
                AddEdgeToPath(iEdge, &path);
                v = GetOtherVertex(v, edge);
            }
            else{
                endOfPath = TRUE;
            }
        }
        else{
            endOfPath = TRUE;
        }
    }
}

short EndsInW(int w, Path path, EdgeList edge_list){
    int vertex, iEdge;
    iEdge = path.iEdges[size - 1];
    vertex = edge_list.edge[iEdge].w;
    return (vertex == w);
}

void InvertPathColors(int color1, int color2, Path path, FreeList *free_list, EdgeList *edge_list, Table *table){
    Edge edge;
    int i, color;
    
    if (path.size == 0){
        printf("\nCaminho vazio");
        return;
    }

    /* Garante que as cores possam ser enviadas por parametro em qualquer ordem */
    edge = edge_list.edge[path.iEdges[0]];
    if (color1 != edge.color){
        color = color1;
        color1 = edge.color;
        color2 = color;
    }
    if (path.size > 1){
        edge = edge_list.edge[path.iEdges[1]];
        color2 = edge.color;
    }

    for (i = 0; i < path.size; i++){
        edge = edge_list.edge[path.iEdges[i]];
        if (i%2 == 0){
            /* Atualiza no EdgeList */
            edge_list.edge[path.iEdges[i]].color = color2;
            /* Atualiza no FreeList */
            RemoveFreeByColor(free_list, edge.v, color2, table);
            RemoveFreeByColor(free_list, edge.w, color2, table);
            InsertFree(edge.v, color1, free_list, table);
            InsertFree(edge.w, color1, free_list, table);
            /* Atualiza no Table */
            UpdateInTable(edge.v, edge.w, color2, path.iEdges[i], table);

        }
        else{
            edge_list.edge[path.iEdges[i]].color = color1;
            /* Atualiza no FreeList */
            RemoveFreeByColor(free_list, edge.v, color1, table);
            RemoveFreeByColor(free_list, edge.w, color1, table);
            InsertFree(edge.v, color2, free_list, table);
            InsertFree(edge.w, color2, free_list, table);
            /* Atualiza no Table */
            UpdateInTable(edge.v, edge.w, color1, path.iEdges[i], table);
        }
        
    }
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