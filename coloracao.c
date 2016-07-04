#include "grafo.h"
#include "io.h"
#include "coloracao.h"

void coloring (EdgeList *edge_list, FreeList *free_list, Table *table){
    int color1, color2;
    int taboo; 
    int edge0, edge1;
    Edge e0;
    Path path;

    color1 = NULO;
    color2 = NULO;
    taboo = NULO;
    edge0 = NULO;
    edge1 = NULO;

    while (!IsTottalyColored(*elist)){
        if (taboo != NULO){
            edge0 = edge1; /* conferir */
        }
        else{
            edge0 = GetIndexOfColorlessEdge(*elist);
            e0 = edge_list->edge[edge0];
            color1 = GetCommonFreeColor(edge_list.edge[edge0], *free_list);
            if (color1 != NULO){
                coloringEdge(color1, edge0, edge_list, free_list, table);
                taboo = NULO;
            }
            else{

                color1 = GetFreeColorNotTaboo(e0.v, *free_list, taboo);
                if (taboo != NULO){
                    color2 = GetFreeColorNotTaboo(e0.w, *free_list, taboo);
                }
                path = TwoColorsPath(e0.v, color1, color2, *edge_list, *table);
                if (EndsInW(e.w, path, *edge_list)) {
                    InvertPathColors(color1, color2, path, free_list, edge_list, table);
                    coloringEdge(color2, edge0, edge_list, free_list, table);
                    taboo = NULO;
                }
                else{
                    edge1 = table->matrix[edge0.w, color2].adj;
                    coloringEdge(color1, edge0, edge_list, free_list, table);
                    taboo = color1;
                }
            }
    }
}