/*
                                TRABALHO PRÁTICO - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
                                Vinicius Reis Gonçalves             15491921
*/

//Arquivo 'graph.c', contém funções que trabalham com a criação e manipulação do grafo

#include "all.h"

void printf_vertex(VERTEX* vertex)
{
    ADJ_NODE* p = vertex->adj_head;
    while(p!=NULL)
    {
        char DataInicio[DATE_LEN + 1];
        char DataFim[DATE_LEN + 1];
        
        strcpy(DataInicio,p->DataInicioQueSegue);
        strcpy(DataFim,p->DataFimQueSegue);

        if(DataInicio[0] == '$'){strcpy(DataInicio,"NULO");}
        if(DataFim[0] == '$'){strcpy(DataFim,"NULO");}

        printf("%s, ",vertex->nomeUsuarioqueSegue);
        printf("%s, ",p->nomeUsuarioqueESeguido);
        printf("%s, ",DataInicio);
        printf("%s, ",DataFim);
        if(p->grauAmizade != '$'){printf("%c",p->grauAmizade);}
        else{printf("NULO");}
        puts("");
        p = p->next;
    }
}

ADJ_NODE* create_adj_node()
{
    ADJ_NODE* adj = (ADJ_NODE*)calloc(1,sizeof(ADJ_NODE));
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
    graph->vertices_array = (VERTEX**)calloc(vertices_n,sizeof(VERTEX*));

    for(int i = 0; i < vertices_n; i ++)
    {
        graph->vertices_array[i] = create_vertex();
    }
    return graph;
}

void destroy_adj_list(ADJ_NODE* adj_head)
{
    if(adj_head->next!=NULL && adj_head != NULL)
    {
        destroy_adj_list(adj_head->next);
    }
    free(adj_head->nomeUsuarioqueESeguido);
    free(adj_head);
    return;
}

void destroy_vertex(VERTEX* vertex)
{
    // destroy_adj_list(vertex->adj_head);
    free(vertex->nomeUsuarioqueSegue);
    free(vertex);
}

void destroy_graph(GRAPH* graph)
{
    for(int i = 0; i < graph->vertices_n; i ++)
    {
        destroy_vertex(graph->vertices_array[i]);
    }
    free(graph->vertices_array);
    free(graph);
}

int compare_nodes(ADJ_NODE* a, ADJ_NODE* b)
{
    int compare = strcmp(a->nomeUsuarioqueESeguido,b->nomeUsuarioqueESeguido);
    if(compare < 0){return 1;}
    else if (compare == 0)
    {
        compare = strcmp(a->DataInicioQueSegue,b->DataInicioQueSegue);
        return compare < 0 ? 1 : 0;
    }
    return 0;
}

int compare_vertex(const void *a, const void *b)
{
    VERTEX* va = *((VERTEX**)a);
    VERTEX* vb = *((VERTEX**)b);

    return strcmp(va->nomeUsuarioqueSegue,vb->nomeUsuarioqueSegue);
}

void order_graph(GRAPH* graph)
{
    qsort(graph->vertices_array,graph->vertices_n,sizeof(graph->vertices_array[0]),compare_vertex);
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
            
            if(p == vertex->adj_head)
            {
                p->ant = adj;
                adj->next = p;
                vertex->adj_head = adj;
            }
            else
            {
                p->ant->next = adj;
                adj->ant = p->ant;
                adj->next = p;
                p->ant = adj;
            }
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
    int pos = index_binary_search(index_arr,idPessoa);
    if(pos == -1){return NULL;}
    DATA_DREG* d_dreg = pull_reg_from_memory(index_arr->idx_arr[pos].byteOffset,data_file);
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
    for(int i = 0; i < f_match_arr->len; i ++)
    {
        if(f_match_arr->follow_arr[i]->removido != '1')
        {
            if(vertex->nomeUsuarioqueSegue == NULL)
            {
                vertex->nomeUsuarioqueSegue = search_username(f_match_arr->follow_arr[i]->idPessoaQueSegue,idx_arr,data_file);
            }
            ADJ_NODE* adj = load_adjacence(f_match_arr->follow_arr[i]);
            adj->nomeUsuarioqueESeguido = search_username(f_match_arr->follow_arr[i]->idPessoaQueESeguida,idx_arr,data_file);
            if(adj->nomeUsuarioqueESeguido == NULL)
            {
                // destroy_adj_list(adj);
                // printf("Deu bolete");
            }
            else
            {insert_adjacence(vertex,adj);}
        }
    }
    return vertex;
}

GRAPH* generate_graph(FILE* data_file, FILE* index_file, FILE* follow_file)
{
    int vertices_n = 0;
    int pessoas = 0;
    int removidos = 0;
    fseek(data_file,1,SEEK_SET);                // Pula o campo 'status'
    fread(&pessoas,4,1,data_file);              // Número de vértices == número de pessoas no arquivo pessoa
    fread(&removidos,4,1,data_file);

    
    fseek(data_file,DF_HEAD_REG_LEN,SEEK_SET); // Pula o registro de cabeçalho do arquivo
    
    
    INDEX_ARR* idx_arr = save_index_in_mem(index_file);
    
    FOLLOW_ARR* f_arr = read_follow_file(follow_file);
    
    vertices_n = count_existing_ids(f_arr,idx_arr);
    
    GRAPH* graph = create_graph(vertices_n);

    int idx = 0;
    int i = 0;
    while(i < vertices_n)
    {
        FOLLOW_ARR* f_match_id = follow_match_reg(f_arr,f_arr->follow_arr[idx]->idPessoaQueSegue);
        VERTEX* vertex = load_vertex(data_file,idx_arr,f_match_id);
        if(vertex->nomeUsuarioqueSegue != NULL)
        {
            graph->vertices_array[i] = vertex;
            i++;
        }
        else{destroy_vertex(vertex);}
        
        idx += f_match_id->len;
        destroy_follow_array(f_match_id);
    }

    destroy_index_arr(idx_arr);
    destroy_follow_array(f_arr);

    order_graph(graph);

    return graph;
}



void printf_graph(GRAPH* graph)
{
    for(int i = 0; i < graph->vertices_n; i ++)
    {
        printf_vertex(graph->vertices_array[i]);
        printf("\n");
    }
}