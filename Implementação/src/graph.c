/*
                                TRABALHO PRÁTICO - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
                                Vinicius Reis Gonçalves             15491921
*/

//Arquivo 'graph.c', contém funções que trabalham com a criação e manipulação do grafo

#include "all.h"

/*
    Aloca memória para um nó da lista de adjacências

    return:
        ADJ_NODE* adj => Elemento da lista de adjacências em memória primária
*/
ADJ_NODE* create_adj_node()
{
    ADJ_NODE* adj = (ADJ_NODE*)calloc(1,sizeof(ADJ_NODE));
    adj->grauAmizade = '$';
    adj->next = NULL;
    adj->nomeUsuarioqueESeguido = NULL;

    return adj;
}

/*
    Aloca memória para um vértice do grafo

    return:
        VERTEX* vertex => Vértice do grafo em memória primária
*/
VERTEX* create_vertex()
{
    VERTEX* vertex = (VERTEX*)calloc(1,sizeof(VERTEX));
    vertex->adj_head = NULL;
    vertex->nomeUsuarioqueSegue = NULL;

    return vertex;
}

/*
    Aloca memória para um grafo, com os valores inicializados em nulo

    params:
        int vertices_n => Número de vértices do grafo
    
    return:
        GRAPH* graph => Grafo em memória primária
*/
GRAPH* create_graph(int vertices_n)
{
    GRAPH* graph = (GRAPH*)calloc(1,sizeof(GRAPH));
    graph->vertices_n = vertices_n;
    graph->vertices_array = (VERTEX**)calloc(vertices_n,sizeof(VERTEX*));

    for(int i = 0; i < vertices_n; i ++)            // Aloca memória para todos os vértices necessários no grafo
    {
        graph->vertices_array[i] = create_vertex();
    }
    return graph;
}

/*
    Libera a memória para a lista de adjacências
*/
void destroy_adj_list(ADJ_NODE* adj_head)
{
    if(adj_head->next!=NULL && adj_head != NULL)
    {
        destroy_adj_list(adj_head->next);   // Percorre toda a lista recursivamente
    }
    free(adj_head->nomeUsuarioqueESeguido);
    free(adj_head);
    return;
}

/*
    Libera a memória de um vértice no grafo
*/
void destroy_vertex(VERTEX* vertex)
{
    destroy_adj_list(vertex->adj_head);
    free(vertex->nomeUsuarioqueSegue);
    free(vertex);
}

/*
    Libera a memória de um grafo
*/
void destroy_graph(GRAPH* graph)
{
    for(int i = 0; i < graph->vertices_n; i ++)
    {
        destroy_vertex(graph->vertices_array[i]);
    }
    free(graph->vertices_array);
    free(graph);
}

/*
    Compara vértices adjacentes para ordenar a lista, em ordem alfabética pelo campo 'nomeUsuario'
    Caso empata, desempata pelo campo 'dataInicioQueSegue'

    params:
        ADJ_NODE* a => Nó a ser inserido
        ADJ_NODE* b => Nó de comparação

    return:
        int compare => Valor 1 para a < b, 0 para o contrário
*/
int compare_nodes(ADJ_NODE* a, ADJ_NODE* b)
{
    int compare = strcmp(a->nomeUsuarioqueESeguido,b->nomeUsuarioqueESeguido);
    if(compare < 0){return 1;}
    else if (compare == 0)
    {
        compare = strcmp(a->DataInicioQueSegue,b->DataInicioQueSegue);
        return compare < 0 ? 1 : 0; // Retorna 1  se compare for menor que 0
    }
    return 0;
}

/*
    Compara os vértices do grafo, em ordem alfabética

    params:
        const void *a => Ponteiro genérico, convertido para VERTEX*
        const void *b => Ponteiro genérico, convertido para VERTEX*
    
    return:
        int => Valor que indica a posição do elemento (antes, se menor que 0, depois caso o contrário)
*/
int compare_vertex(const void *a, const void *b)
{
    VERTEX* va = *((VERTEX**)a);    // Faz o cast de um ponteiro genérico para um vértice
    VERTEX* vb = *((VERTEX**)b);

    return strcmp(va->nomeUsuarioqueSegue,vb->nomeUsuarioqueSegue);
}

/*
    Ordena um grafo utilizando o quiscksort implementado na stdlib

    params:
        GRAPH* graph => Grafo a ser ordenado
    
    return:
        void
*/
void order_graph(GRAPH* graph)
{
    qsort(graph->vertices_array,graph->vertices_n,sizeof(graph->vertices_array[0]),compare_vertex);
}

/*
    Insere um vértice adjacente na lista de adjacências

    params:
        VERTEX* vertex => Vértice ao qual o referido é adjacente
        ADJ_NODE* adj => Adjacência a ser inserida
    
    return:
        void
*/
void insert_adjacence(VERTEX* vertex, ADJ_NODE* adj)
{
    if(vertex->adj_head == NULL)                        // Caso a lista esteja vazia
    {
        vertex->adj_head = adj;
    }
    else
    {
        ADJ_NODE* p = vertex->adj_head;
        while (p->next!=NULL && !compare_nodes(adj,p))  // Percorre a lista até o fim ou encontrar a posição correta da adjacência na lista
        {
            p = p->next;
        }
        
        if(compare_nodes(adj,p))                        // Caso tenha encontrado uma posição válida que não a final
        {
            
            if(p == vertex->adj_head)                   // Trata o caso no qual a posição é no início da lista
            {
                p->ant = adj;
                adj->next = p;
                vertex->adj_head = adj;
            }
            else                                        // Caso geral
            {
                p->ant->next = adj;
                adj->ant = p->ant;
                adj->next = p;
                p->ant = adj;
            }
        }
        else                                            // Caso insira no final
        {
            p->next = adj;
            adj->ant = p;
        }
    }
}

/*
    Carrega a adjacência a partir de um registro de dados do arquivo segue

    params:
        FOLLOW_DREG* f_dreg => Registro de dados a ser utilizado
    
    return:
        ADJ_NODE* adj => Adjacência em memória primária
*/
ADJ_NODE* load_adjacence(FOLLOW_DREG* f_dreg)
{
    ADJ_NODE* adj = create_adj_node();
    strcpy(adj->DataFimQueSegue,f_dreg->dataFimQueSegue);
    strcpy(adj->DataInicioQueSegue,f_dreg->dataInicioQueSegue);
    adj->grauAmizade = f_dreg->grauAmizade;
    return adj;
}

/*
    Carrega um vértice para a memória, junto com sua lista de adjacências

    params:
        FILE* data_file => Arquivo de dados onde persistem as informações
        INDEX_ARR* idx_arr => Arquivo de índice em memória primária
        FOLLOW_ARR* f_match-arr => Registros que contém todas as pessoas seguidas por um determinado usuário
    
    return:
        VERTEX* vertex => Vértice carregado com todas as informações
*/
VERTEX* load_vertex(FILE* data_file, INDEX_ARR* idx_arr, FOLLOW_ARR* f_match_arr)
{   
    VERTEX* vertex = create_vertex();
    for(int i = 0; i < f_match_arr->len; i ++)                          // Percorre todos os registros relacionados à um usuário
    {
        if(f_match_arr->follow_arr[i]->removido != '1')                 // Carrega somente se o registro for válido
        {
            if(vertex->nomeUsuarioqueSegue == NULL)                     // Recupera o nome do usuário representado pelo vértice
            {
                vertex->nomeUsuarioqueSegue = search_username(f_match_arr->follow_arr[i]->idPessoaQueSegue,idx_arr,data_file);
            }

            ADJ_NODE* adj = load_adjacence(f_match_arr->follow_arr[i]); // Carrega as informações  de id da adjacência

            // Recupera o nome do usuário seguido
            adj->nomeUsuarioqueESeguido = search_username(f_match_arr->follow_arr[i]->idPessoaQueESeguida,idx_arr,data_file);

            if(adj->nomeUsuarioqueESeguido == NULL)                     // Caso a adjacêcnia seja inválida, libera memória
            {
                destroy_adj_list(adj);
            }
            else{insert_adjacence(vertex,adj);}                         // Insere a adjacência na lista de adjacências do vértices
        }
    }
    return vertex;
}

/*
    Gera um grafo a partir dos arquivos do tipo 'pessoa', 'indice' e 'segue, que indica um usuário e todos aqueles que o mesmo segue.

    params:
        FILE* data_file => Arquivo do tipo 'pessoa'
        FILE* index_file => Arquivo do tipo 'indice'
        FILE* follow_file => Arquivo do tipo 'segue', ordenado.
    
    return:
        GRAPH* graph => Grafo resultante

*/
GRAPH* generate_graph(FILE* data_file, FILE* index_file, FILE* follow_file)
{    
    fseek(data_file,DF_HEAD_REG_LEN,SEEK_SET);          // Pula o registro de cabeçalho do arquivo para futuras leituras
    
    INDEX_ARR* idx_arr = save_index_in_mem(index_file); //  Salva o arquivo 'indice' em memória primária
    
    FOLLOW_ARR* f_arr = read_follow_file(follow_file);  // Salva o arquivo 'segue' em memória primária
    
    int vertices_n = count_existing_ids(f_arr,idx_arr); // Descobre o número de vértices necessários para a montagem do grafo
    
    GRAPH* graph = create_graph(vertices_n);            // Aloa memória para o grafo

    int idx = 0;                                        // Variáveis de indíce
    int i = 0;

    while(i < vertices_n)
    {
        FOLLOW_ARR* f_match_id = follow_match_reg(f_arr,f_arr->follow_arr[idx]->idPessoaQueSegue);  // Carrega o vetor que contém todas as relações para um determinado usuário
        VERTEX* vertex = load_vertex(data_file,idx_arr,f_match_id);                                 // Carrega o vértice que representa o referente usuário

        if(vertex->nomeUsuarioqueSegue != NULL)         // Caso seja um vértice válido, o coloca no grafo
        {
            graph->vertices_array[i] = vertex;
            i++;
        }
        else{destroy_vertex(vertex);}                   // Senão, libera a memória alocada
        
        idx += f_match_id->len;                         // Aproveita da ordenação do arquivo 'segue' para passar para o próximo usuário
        destroy_follow_array(f_match_id);               // Libera a memória do vetor com as relações do referido usuário
    }

    destroy_index_arr(idx_arr);                         // Libera os arquivos da memória primária                                         
    destroy_follow_array(f_arr);

    order_graph(graph);                                 // Ordena o grafo por ordem alfabética

    return graph;
}