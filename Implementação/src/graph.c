/*
                                TRABALHO PRÁTICO - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
                                Vinicius Reis Gonçalves             15491921
*/

//Arquivo 'graph.c', contém funções que trabalham com a criação e manipulação do grafo

#include "all.h"


ADJ_NODE* create_adj_node()
{
    ADJ_NODE* adj = (ADJ_NODE*)calloc(1,sizeof(ADJ_NODE));
    strcpy(adj->DataFimQueSegue,"---------");
    strcpy(adj->DataInicioQueSegue,"--------");
    adj->grauAmizade = '$';
    adj->next = NULL;
    adj->nomeUsuarioqueESeguido = NULL;

    return adj;
}

VERTEX* create_vertex()
{
    VERTEX* vertex = (VERTEX*)calloc(1,sizeof(VERTEX));
    vertex->adj_head = NULL;
    vertex->nomeUsuarioqueSegue = NULL;

    return vertex;
}

GRAPH* create_graph(int vertices_n)
{
    GRAPH* graph = (GRAPH*)calloc(1,sizeof(GRAPH));
    graph->vertices_n = vertices_n;
    graph->vertices_array = (VERTEX*)calloc(vertices_n,sizeof(VERTEX));

    for(int i = 0; i < vertices_n; i ++)
    {
        graph->vertices_array[i] = *create_vertex();
    }
    return graph;
}