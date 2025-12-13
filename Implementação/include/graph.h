/*
                                TRABALHO PRÁTICO - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
                                Vinicius Reis Gonçalves             15491921
*/

// Arquivo 'graph.h', contém a definição das funções relacionadas à criação e manipulação do grafo

#ifndef GRAPH_H
#define GRAPH_H

#include "all.h"

// Estrutura de dados que representa a lista de adjacências do grafo
typedef struct _adjacence_node
{

    char* nomeUsuarioqueESeguido;       // Nome do usuário do vértice adjacente
    char DataInicioQueSegue [DATE_LEN + 1]; // Data que se iniciou a relação entre os usuários
    char DataFimQueSegue [DATE_LEN + 1];    // Data que se terminou a relação entre os usuários
    char grauAmizade;                   // Grau de amizade entre os usuários
    struct _adjacence_node* next;       // Próximo elemento na lista (Próxima adjacência do vértice atual)
    struct _adjacence_node* ant;        // Elemento anterior ao atual

}ADJ_NODE;

// Represent um vértice qualquer do grafo, contendo o nome de usuário e sua lista de adjacências
typedef struct _graph_vertex
{
    char* nomeUsuarioqueSegue;  // Nome do referente usuário
    ADJ_NODE* adj_head;         // Início da lista de adjacências do mesmo

}VERTEX;

// Super-estrutura que representa o grafo, contém o n° de vértices eo vetor de vértices propriamente dito
typedef struct _graph
{
    int vertices_n;         // Número de vértices do grafo
    VERTEX** vertices_array; // Vetor de vértices
    
}GRAPH;

GRAPH* generate_graph(FILE* data_file, FILE* index_file, FILE* follow_file);

void printf_graph(GRAPH* graph);


#endif