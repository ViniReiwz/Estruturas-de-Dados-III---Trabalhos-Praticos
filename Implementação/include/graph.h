/*
                                TRABALHO PRÁTICO - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
                                Vinicius Reis Gonçalves             15491921
*/

// Arquivo 'graph.h', contém a definição das funções relacionadas à criação e manipulação do grafo

#ifndef GRAPH_H
#define GRAPH_H

#include "all.h"

#define MAX_ARR 50000

// Estrutura de dados que representa a lista de adjacências do grafo
typedef struct _adjacence_node
{

    char* nomeUsuarioqueESeguido;           // Nome do usuário do vértice adjacente
    char DataInicioQueSegue [DATE_LEN + 1]; // Data que se iniciou a relação entre os usuários
    char DataFimQueSegue [DATE_LEN + 1];    // Data que se terminou a relação entre os usuários
    char grauAmizade;                       // Grau de amizade entre os usuários
    struct _adjacence_node* next;           // Próximo elemento na lista (Próxima adjacência do vértice atual)
    struct _adjacence_node* ant;            // Elemento anterior ao atual

}ADJ_NODE;

// Representa um vértice qualquer do grafo, contendo o nome de usuário e sua lista de adjacências
typedef struct _graph_vertex
{
    char* nomeUsuarioqueSegue;  // Nome do referente usuário
    ADJ_NODE* adj_head;         // Início da lista de adjacências do mesmo

}VERTEX;

// Super-estrutura que representa o grafo, contém o n° de vértices e o vetor de vértices propriamente dito (lista de adjacências)
typedef struct _graph
{
    int vertices_n;             // Número total de vértices do grafo
    int filled_vertices;        // Número de vértices preenchidos do grafo
    VERTEX** vertices_array;    // Vetor de vértices
    
}GRAPH;

typedef struct _edge
{
    char* nomeUsuarioqueSegue;
    char* nomeUsuarioqueESeguido;           // Nome do usuário do vértice adjacente
    char* DataInicioQueSegue; // Data que se iniciou a relação entre os usuários
    char* DataFimQueSegue;    // Data que se terminou a relação entre os usuários
    char grauAmizade;                       // Grau de amizade entre os usuários
    struct _edge* next;           // Próximo elemento na lista (Próxima adjacência do vértice atual)
    struct _edge* ant;            // Elemento anterior ao atual

}EDGE;

typedef struct _path
{
    EDGE* startpoint;
    int lengh;

}PATH;

/*
    Aloca memória para um nó da lista de adjacências

    return:
        ADJ_NODE* adj => Elemento da lista de adjacências em memória primária
*/
ADJ_NODE* create_adj_node();

/*
    Aloca memória para um vértice do grafo

    return:
        VERTEX* vertex => Vértice do grafo em memória primária
*/
VERTEX* create_vertex();

/*
    Aloca memória para um grafo, com os valores inicializados em nulo

    params:
        int vertices_n => Número de vértices do grafo
    
    return:
        GRAPH* graph => Grafo em memória primária
*/
GRAPH* create_graph(int vertices_n);

/*
    Libera a memória para a lista de adjacências
*/
void destroy_adj_list(ADJ_NODE* adj_head);

/*
    Libera a memória de um vértice no grafo
*/
void destroy_vertex(VERTEX* vertex);

/*
    Libera a memória de um grafo
*/
void destroy_graph(GRAPH* graph);

/*
    Compara vértices adjacentes para ordenar a lista, em ordem alfabética pelo campo 'nomeUsuario'
    Caso empata, desempata pelo campo 'dataInicioQueSegue'

    params:
        ADJ_NODE* a => Nó a ser inserido
        ADJ_NODE* b => Nó de comparação

    return:
        int compare => Valor 1 para a < b, 0 para o contrário
*/
int compare_nodes(ADJ_NODE* a, ADJ_NODE* b);

/*
    Compara os vértices do grafo, em ordem alfabética

    params:
        const void *a => Ponteiro genérico, convertido para VERTEX*
        const void *b => Ponteiro genérico, convertido para VERTEX*
    
    return:
        int => Valor que indica a posição do elemento (antes, se menor que 0, depois caso o contrário)
*/
int compare_vertex(const void *a, const void *b);

/*
    Ordena um grafo utilizando o quiscksort implementado na stdlib

    params:
        GRAPH* graph => Grafo a ser ordenado
    
    return:
        void
*/
void order_graph(GRAPH* graph);

/*
    Insere um vértice adjacente na lista de adjacências

    params:
        VERTEX* vertex => Vértice ao qual o referido é adjacente
        ADJ_NODE* adj => Adjacência a ser inserida
    
    return:
        void
*/
void insert_adjacence(VERTEX* vertex, ADJ_NODE* adj);

/*
    Carrega a adjacência a partir de um registro de dados do arquivo segue

    params:
        FOLLOW_DREG* f_dreg => Registro de dados a ser utilizado
    
    return:
        ADJ_NODE* adj => Adjacência em memória primária
*/
ADJ_NODE* load_adjacence(FOLLOW_DREG* f_dreg);

/*
    Carrega um vértice para a memória, junto com sua lista de adjacências

    params:
        FILE* data_file => Arquivo de dados onde persistem as informações
        INDEX_ARR* idx_arr => Arquivo de índice em memória primária
        FOLLOW_ARR* f_match-arr => Registros que contém todas as pessoas seguidas por um determinado usuário
    
    return:
        VERTEX* vertex => Vértice carregado com todas as informações
*/
VERTEX* load_vertex(FILE* data_file, INDEX_ARR* idx_arr, FOLLOW_ARR* f_match_arr);

/*
    Gera um grafo a partir dos arquivos do tipo 'pessoa', 'indice' e 'segue, que indica um usuário e todos aqueles que o mesmo segue.

    params:
        FILE* data_file => Arquivo do tipo 'pessoa'
        INDEX_ARR* idx_arr => Arquivo do tipo 'indice' em memória primária
        FOLLOW_ARR* follow_file => Arquivo do tipo 'segue', ordenado, em memória primária
    
    return:
        GRAPH* graph => Grafo resultante

*/
GRAPH* generate_graph(FILE* data_file, INDEX_ARR* idx_arr, FOLLOW_ARR* f_arr);

/*
    Busca a posição que um vértice deveria ser inserido

    params:
        GRAPH* graph => Grafo no qual a busca será feita
        const char* nomeUsuariovertice => Nome d usuário representado pelo vértice a ser buscado

    return:
        int pos => Indíce do vetor de vértices onde o referido nome deveria ser inserido
*/
int search_pos(GRAPH* graph, const char* nomeUsuarioVertice);

/*
    Gera o grafo transposto a partir de um outro grafo em memória primária

    params:
        GRAPH* graph => Grafo a transpor
    
    return:
        GRAPH* transposed_graph => Grafo transposto
*/
GRAPH* transpose_graph(GRAPH* graph);

/*
    Aloca a memória de uma path
    
    return:
        PATH* path => path alocado
*/
PATH* create_path();

/*
    Adiciona uma aresta no começo da lista de arestas de um caminho

    params:
        ADJ_NODE* node => nó de onde informações são puxadas
        VERTEX* vertex => vértice de onde informações são puxadas
        PATH* path => caminho aonde a aresta é inserida
    
    return:
        void
*/
void add_edge_to_path(ADJ_NODE* node, VERTEX* vertex, PATH* path);

/*
    Desaloca a memória usada no caminho

    params:
        PATH* path => caminho a ser desalocado
    
    return:
        void
*/
void destroy_path(PATH *path);

/*
    Copia um caminho

    params:
        PATH* source => caminho a ser copiado
        PATH* cloned => caminho copiado
    
    return:
        void
*/
void clone_path(PATH* source, PATH* cloned);

/*
    Recebe um grafo, um array de caminhos e um array de inteiros adcional, bem como um usuario de onde se parte a busca.
    Grava no array de caminhos o menor caminho de cada vértice até o vértice inicial.

    params:
        GRAPH* transposed_graph => grafo transposto onde a procura deve ser iniciada
        PATH** array_path => array de caminhos
        char* startpoint => nome do usuário do vértice
        int* was_visited => inteiros auxiliares na determinação se um vértice foi ou não visitado
    
    return:
        void
*/
void dijkstra(GRAPH* transposed_graph, PATH** array_path, char* startpoint, int* was_visited);

/*
    Printa na tela todos os caminhos de um vetor de caminhos.

    params:
        PATH** array_path => array de caminhos
        int size => tamanho do vetor de caminhos
    
    return:
        void
*/
void print_paths(PATH** array_path, int size);

/*
    Recebe um grafo, um array de caminhos e um array de inteiros adcional, bem como um usuario de onde se parte a busca.
    Grava no array de caminhos o menor caminho de cada vértice até o vértice inicial. Esta versão acha também o menor caminho
    da partida até ela mesma.

    params:
        GRAPH* transposed_graph => grafo transposto onde a procura deve ser iniciada
        PATH** array_path => array de caminhos
        char* startpoint => nome do usuário do vértice
        int* was_visited => inteiros auxiliares na determinação se um vértice foi ou não visitado
    
    return:
        void
*/

void dijkstra_to_itself(GRAPH* transposed_graph, PATH** array_path, char* startpoint, int* was_visited);

#endif