#include <stdio.h>
#include <stdlib.h>

#ifndef _H_GRAFO
#define _H_GRAFO

#define FALSE 0
#define TRUE 1
#define ALLOC_BLOCK 5
#define NULO -1


/* Estruturas a definir no grafo G(V, E), onde |V| = n e |E| = m, onde C é o conjunto
de cores usadas para colorir as arestas do grafo e delta é o maior grau de G
- Free
	armazena o conjunto de até delta + 1 cores disponíveis para colorir as arestas 
	incidentes a um dado vértice v. 
- Edge
	conjunto de tamanho m onde cada elemento e é uma abstração de uma das arestas de G
	e possui as variáveis v, w, e color. As variáveis e.v e e.w representam os vértices 
	conectados pela aresta e e a variável e.color armazena a cor atribuída à aresta e.
- Table
	matriz de dimensões [v X (delta +1)] onde cada linha está associada a um vértice
	do grafo G e cada coluna está associada a uma cor a de C, onde |C| = (delta + 1). 
	Cada elemento da matriz possui as variáveis status, adj e cIndex. A variável 
	Table(v, a).status é verdadeiro sempre que uma aresta de cor a incide no vértice 
	v e assume valor falso sempre que esta cor esteja livre no vértice v. A variável 
	Table(v, a).adj é um apontador que referencia a aresta e, que conecta os vértices 
	v e Table(v, a).adj na estrutura Edge(G). A variável Table(v, a).cIndex armazena
	o índice da cor a no conjunto Free(v), o que possibilita que a remoção de uma cor
	de Free seja O(1).
*/
typedef struct{
    int v;
    int w;
    int color;	
} Edge;

typedef struct{
    Edge *edge;
    int size;
} EdgeList;

typedef struct FreeCell_struct *Pointer;

typedef struct FreeCell_struct{
    int color;
    Pointer next;
} FreeCell;

typedef struct{
    Pointer first;
    Pointer last;
} Free;

typedef struct{
    Free *f;
    int size;
} FreeList;

typedef struct {
    short status;
    int adj;
    Pointer cPointer;
} TableCell;

typedef struct{
    TableCell **matrix;
    int sizeX;
    int sizeY;
} Table;

typedef struct {
    int **matrix;
    int n;
    int m;
    int delta;
} AdjMatrixGraph;

void CreateFree(Free *f);
void CreateFreeList(FreeList *fl, int nvertices);
short IsEmpty(Free f);
void InsertFree(int color, Free *f, TableCell *tcell);
void InsertFree(int v, int color, FreeList *fl, TableCell *tcell);
int RemoveFree(Free *f, Pointer p, int *color, TableCell *tcell);
void RemoveFree(FreeList *fl, int v, Pointer p, int *color, TableCell *tcell);

void CreateEdgeList(EdgeList *elist, int nedges);
void AddEdge(int v, int w, int color, EdgeList *elist);
Edge GetEdge(int v, int w, EdgeList elist);

void CreateTable(Table *table, int nvertices, ncolors);

void CreateAdjMatrix(AdjMatrixGraph *graph, int nvertices);
void InitWithAdjMatrix(AdjListGraph *graph, EdgeList *edge_list, FreeList *free_list, Table *table);


#endif
