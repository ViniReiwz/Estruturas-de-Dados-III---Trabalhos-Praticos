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

void destroy_adj_list(ADJ_NODE* adj_head)
{
    if(adj_head->next!=NULL)
    {
        destroy_adj_node(adj_head->next);
    }
    free(adj_head->nomeUsuarioqueESeguido);
    free(adj_head);
    return;
}

void destroy_vertex(VERTEX* vertex)
{
    destroy_adj_list(vertex->adj_head);
    free(vertex->nomeUsuarioqueSegue);
    free(vertex);
}

void destroy_graph(GRAPH* graph)
{
    for(int i = 0; i < graph->vertices_n; i ++)
    {
        destroy_vertex(&graph->vertices_array[i]);
    }
    free(graph);
}

int compare_nodes(ADJ_NODE* a, ADJ_NODE* b)
{
    int compare = strcmp(a->nomeUsuarioqueESeguido,b->nomeUsuarioqueESeguido);
    if(compare < 0){return 1;}
    else if (compare == 0)
    {
        compare = strcmp(a->DataInicioQueSegue,b->DataFimQueSegue);
        return compare < 0 ? 1 : 0;
    }
    return 0;
}

void insert_adjacence(VERTEX* vertex, ADJ_NODE* adj)
{
    if(vertex->adj_head == NULL)
    {
        vertex->adj_head = adj;
    }
    else
    {
        ADJ_NODE* p = vertex->adj_head;
        while (p->next!=NULL && !compare_nodes(adj,p))
        {
            p = p->next;
        }
        
        if(compare_nodes(adj,p))
        {
            adj->next = p;
            p->ant->next = adj;
            adj->ant = p->ant;
            p->ant = adj;
        }
        else
        {
            p->next = adj;
            adj->ant = p;
        }
    }
}

char* search_username(int idPessoa, INDEX_ARR* index_arr, FILE* data_file)
{
    long int b_offset = index_binary_search(index_arr,idPessoa);
    DATA_DREG* d_dreg = pull_reg_from_memory(b_offset,data_file);
    char* nomeUsuario = (char*)calloc(d_dreg->tamNomeUsuario + 1,sizeof(char));
    strcpy(nomeUsuario,d_dreg->nomeUsuario);
    destroy_data_dreg(d_dreg);
    return nomeUsuario;
}

ADJ_NODE* load_adjacence(FOLLOW_DREG* f_dreg)
{
    ADJ_NODE* adj = create_adj_node();
    strcpy(adj->DataFimQueSegue,f_dreg->dataFimQueSegue);
    strcpy(adj->DataInicioQueSegue,f_dreg->dataInicioQueSegue);
    adj->grauAmizade = f_dreg->grauAmizade;
    return adj;
}

VERTEX* load_vertex(FILE* data_file, INDEX_ARR* idx_arr, FOLLOW_ARR* f_match_arr)
{   
    VERTEX* vertex = create_vertex();
    vertex->nomeUsuarioqueSegue = search_username(f_match_arr->follow_arr[0]->idPessoaQueSegue,idx_arr,data_file);
    FOLLOW_DREG* f_arr = f_match_arr->follow_arr;
    for(int i = 0; i < f_match_arr->len; i ++)
    {
        ADJ_NODE* adj = load_adjacence(&f_arr[i]);
        adj = search_username(f_arr[i].idPessoaQueESeguida,idx_arr,data_file);
        insert_adjacence(vertex,adj);
    }
    return vertex;
}

GRAPH* generate_graph(FILE* data_file, FILE* index_file, FILE* follow_file)
{
    int vertices_n = 0;
    fseek(data_file,1,SEEK_SET); // Pula o campo 'status'
    fread(&vertices_n,4,1,data_file);

    GRAPH* graph = create_graph(vertices_n);



}