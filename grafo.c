#include <grafo.h>

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
        CreateFree(*fl->f[i]);
    }
}

short IsEmpty(Free f){
    return (f->first == f->last);
}

/*
void ReallocFree(Free *f){
    f->allocated++;
    f->vec = (int *) realloc(f->vec, f->allocated*ALLOC_BLOCK*sizeof(int));
}
*/
void InsertFree(int color, Free *f, TableCell *tcell){
    tcell->cPointer = f->last;
    f->last->next = (Pointer) malloc(sizeof(FreeCell));
    f->last = f->last->next;
    f->last->color = color;
    f->last->next = NULL;
}

void InsertFree(int v, int color, FreeList *fl, TableCell *tcell){
    InsertFree(color, fl[v], tcell);
}

int RemoveFree(Free *f, Pointer p, int *color, TableCell *tcell){
    Pointer nextP;
    if (IsEmpty(*f) || p == NULL || p.next == NULL){
        printf("\nErro: Lista vazia ou posicao inexistente");
        return -1;
    }
    nextP = p->next;
    *color = nextP->color;
    p->next = nextP->next;
    if (p->next == NULL)
        f->last = p;
    free(nextP);
}

void RemoveFree(FreeList *fl, int v, Pointer p, int *color, TableCell *tcell){
    RemoveFree(fl->f[v], p, color, tcell);
}

void CreateEdgeList(EdgeList *elist, int nedges)
    elist->size = nedges;
    elist->edge = (Edge) malloc(elist->size*sizeof(Edge));
}

void CreateTableCell(TableCell *tcell){
    tcell->status = FALSE;
    tcell->adj = -1;
    tcell->cPointer = NULL;    
}

void CreateTable(Table *table, int nvertices, ncolors){
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