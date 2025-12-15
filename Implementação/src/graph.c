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
    graph->filled_vertices = 0;
    graph->vertices_array = (VERTEX**)calloc(vertices_n,sizeof(VERTEX*));
    return graph;
}

/*
    Libera a memória para a lista de adjacências
*/
void destroy_adj_list(ADJ_NODE* adj_head)
{
    if(adj_head == NULL){return;}           // Caso a a sub-lista esteja vazia, retorna
    else
    {destroy_adj_list(adj_head->next);}     // Avança para a pdóxima sub-lista

    free(adj_head->nomeUsuarioqueESeguido); // Libera a memória
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

    // Trata os casos de nulidade dos valores do nome
    if(va->nomeUsuarioqueSegue == NULL && vb->nomeUsuarioqueSegue == NULL){return 0;}
    else if(va->nomeUsuarioqueSegue == NULL){return 1;}
    else if(vb->nomeUsuarioqueSegue == NULL){return -1;}
    else{return strcmp(va->nomeUsuarioqueSegue,vb->nomeUsuarioqueSegue);}
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
    // Ordena baseado no número de vértices atualmente preenchidos
    qsort(graph->vertices_array,graph->filled_vertices,sizeof(graph->vertices_array[0]),compare_vertex);
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
        INDEX_ARR* idx_arr => Arquivo do tipo 'indice' em memória primária
        FOLLOW_ARR* follow_file => Arquivo do tipo 'segue', ordenado, em memória primária
    
    return:
        GRAPH* graph => Grafo resultante

*/
GRAPH* generate_graph(FILE* data_file, INDEX_ARR* idx_arr, FOLLOW_ARR* f_arr)
{    
    fseek(data_file,DF_HEAD_REG_LEN,SEEK_SET);          // Pula o registro de cabeçalho do arquivo para futuras leituras
    
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
            graph->filled_vertices++;
            i++;
        }
        else{destroy_vertex(vertex);}                   // Senão, libera a memória alocada
        
        idx += f_match_id->len;                         // Aproveita da ordenação do arquivo 'segue' para passar para o próximo usuário
        destroy_follow_array(f_match_id);               // Libera a memória do vetor com as relações do referido usuário
    }

    order_graph(graph);                                 // Ordena o grafo por ordem alfabética

    return graph;
}

/*
    Busca a posição que um vértice deveria ser inserido

    params:
        GRAPH* graph => Grafo no qual a busca será feita
        const char* nomeUsuariovertice => Nome d usuário representado pelo vértice a ser buscado

    return:
        int pos => Indíce do vetor de vértices onde o referido nome deveria ser inserido
*/
int search_pos(GRAPH* graph, const char* nomeUsuarioVertice)
{
    for(int i = 0; i < graph->filled_vertices; i++) // Percorre todos os vértices preenchidos
    {
        // Verifica se em algum deles o nome está presente, retornando a posição no caso afirmativo
        if(strcmp(graph->vertices_array[i]->nomeUsuarioqueSegue,nomeUsuarioVertice) == 0)   
        {return i;}
    }

    return graph->filled_vertices;                  // Caso não encontre o nome, retorna a próxima posição disponível no vetor
}

/*
    Gera o grafo transposto a partir de um outro grafo em memória primária

    params:
        GRAPH* graph => Grafo a transpor
    
    return:
        GRAPH* transposed_graph => Grafo transposto
*/
GRAPH* transpose_graph(GRAPH* graph)
{
    GRAPH* transp_graph = create_graph(graph->vertices_n);                      // Cria o grafo transposto com o número total de vértices

    for(int i = 0; i < graph->vertices_n ; i++)                                 // Percorre todo o grafo orginal
    {
        VERTEX* old_vertex = graph->vertices_array[i];                          // Renomeia para melhor manipulação
        ADJ_NODE* p = old_vertex->adj_head;

        while (p != NULL)                                                       // Percorre toda a lista de adjacências
        {
            int pos = search_pos(transp_graph,p->nomeUsuarioqueESeguido);       // Procura a posição onde o novo vértice deve ser inserido
            if(transp_graph->vertices_array[pos] == NULL)                       // Caso a posição esteja vazia, cria novo vértice e insere
            {
                VERTEX* new_vertex = create_vertex();                           // Cria novo vértice

                // Transpõe os dados (nomeUsuarioQueSegue troca de lugar com nomeUsuarioqueESeguido) entre o vértice antigo e seu adjacente, transformando uma adjacência em um vértice
                new_vertex->nomeUsuarioqueSegue = (char*)calloc(strlen(p->nomeUsuarioqueESeguido) + 1,sizeof(char));
                strcpy(new_vertex->nomeUsuarioqueSegue,p->nomeUsuarioqueESeguido);

                transp_graph->vertices_array[pos] = new_vertex;                 // Posiciona o novo vértice no vetor
                transp_graph->filled_vertices++;                
            }


            ADJ_NODE* adj = create_adj_node();                                  // Cria nova adjacência, com os dados do i-ésimo vértice do grafo original

            adj->nomeUsuarioqueESeguido = (char*)calloc(strlen(old_vertex->nomeUsuarioqueSegue) + 1,sizeof(char));
            strcpy(adj->nomeUsuarioqueESeguido,old_vertex->nomeUsuarioqueSegue);
            strcpy(adj->DataInicioQueSegue,p->DataInicioQueSegue);
            strcpy(adj->DataFimQueSegue,p->DataFimQueSegue);
            adj->grauAmizade = p->grauAmizade;

            insert_adjacence(transp_graph->vertices_array[pos],adj);            // Insere na lista de adjacências do vértice transposto

            p = p->next;                                                        // Avança nas adjacências do grafo original
        }
    }

    transp_graph->vertices_n = transp_graph->filled_vertices;                   // Rearranjo muda o n° de vpertice efetivos entre o original e o transposto

    return transp_graph;
}

/*
    Aloca a memória de uma path
    
    return:
        PATH* path => path alocado
*/
PATH* create_path()
{
    PATH* path = (PATH*)calloc(1, sizeof(PATH));
    path->lengh = -1;
    path->startpoint = NULL;

    return path;
}

/*
    Adiciona uma aresta no começo da lista de arestas de um caminho

    params:
        ADJ_NODE* node => nó de onde informações são puxadas
        VERTEX* vertex => vértice de onde informações são puxadas
        PATH* path => caminho aonde a aresta é inserida
    
    return:
        void
*/
void add_edge_to_path(ADJ_NODE* node, VERTEX* vertex, PATH* path)
{
    EDGE* edge = (EDGE*)calloc(1, sizeof(EDGE));
    
    //Atribuição ao contrário por se tratar de um grafo transposto
    edge->nomeUsuarioqueESeguido = vertex->nomeUsuarioqueSegue;
    edge->nomeUsuarioqueSegue = node->nomeUsuarioqueESeguido;

    edge->DataInicioQueSegue = node->DataInicioQueSegue;
    edge->DataFimQueSegue = node->DataFimQueSegue;
    edge->grauAmizade = node->grauAmizade;

    if(path->startpoint == NULL)    //Adiciona na lista vazia
    {
        path->startpoint = edge;
        edge->next = NULL;
        edge->ant = NULL;
    }
    else    //Adiciona em uma lista não vazia
    {
        edge->ant = NULL;
        path->startpoint->ant = edge;
        edge->next = path->startpoint;
        path->startpoint = edge;
    }
    
    path->lengh++;
}

/*
    Desaloca a memória usada no caminho

    params:
        PATH* path => caminho a ser desalocado
    
    return:
        void
*/
void destroy_path(PATH *path)
{
    EDGE *p = path->startpoint;
    EDGE *p_ant;

    while(p != NULL)
    {   
        p_ant = p;
        p = p->next;
        free(p_ant);
    }

    free(path);
}

/*
    Copia um caminho

    params:
        PATH* source => caminho a ser copiado
        PATH* cloned => caminho copiado
    
    return:
        void
*/
void clone_path(PATH* source, PATH* cloned)
{
    destroy_path(cloned);   //Destroi o antigo caminho
    cloned = create_path();

    EDGE *p = source->startpoint;
    while(p != NULL)
    {
        EDGE* edge = (EDGE*)calloc(1, sizeof(EDGE));    //Vai adicionando no final da lista cada um dos nós do caminho "source"

        edge->nomeUsuarioqueESeguido = p->nomeUsuarioqueESeguido;   //Copia uma aresta do caminho de origem
        edge->nomeUsuarioqueSegue = p->nomeUsuarioqueSegue;
        edge->DataInicioQueSegue = p->DataInicioQueSegue;
        edge->DataFimQueSegue = p->DataFimQueSegue;
        edge->grauAmizade = p->grauAmizade;

        if(cloned->startpoint == NULL)  //Adiciona caso a lista esteja vazia
        {
            edge->next = NULL;
            edge->ant = NULL;
            cloned->startpoint = edge;
        }
        else    //Lista não vazia
        {
            EDGE* pos = cloned->startpoint;
            while(pos->next !=  NULL)
            {
                pos = pos->next;
            }

            pos->next = edge;
            edge->ant = pos;
            edge->next = NULL;    
        }

        p = p->next;
    }

    cloned->lengh = source->lengh;
}

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
void dijkstra(GRAPH* transposed_graph, PATH** array_path, char* startpoint, int* was_visited)
{
    int pos = search_pos(transposed_graph, startpoint);
    if(array_path[pos]->lengh == -1)    //Caso seja o começo da busca
    {
        array_path[pos]->lengh = 0;     //Caminho a ele mesmo é zero
        was_visited[pos] = 1;
    }

    ADJ_NODE* p = transposed_graph->vertices_array[pos]->adj_head;  //Pega os vértices adjacentes
    while(p != NULL)
    {
        int next_vert_pos = search_pos(transposed_graph, p->nomeUsuarioqueESeguido);    //Pega a posição do vértice
        int still_following = strcmp(p->DataFimQueSegue, "$$$$$$$$$$");     //Flag para acusar que ainda segue (sem data de fim)
        
        //Caso o caminho até um vértice adjacente seja menor que o caminho ao vértice atual mais um, substitui o caminho
        //Testa também se há algum caminho (é igual a menos 1) e se é valido (ainda segue)
        if(still_following == 0 &&(array_path[next_vert_pos]->lengh == -1 || array_path[next_vert_pos]->lengh > array_path[pos]->lengh + 1))
        {
            clone_path(array_path[pos], array_path[next_vert_pos]);
            add_edge_to_path(p ,transposed_graph->vertices_array[pos], array_path[next_vert_pos]);
        }

        p = p->next;
    }

    int smallest = 100000000;   //Pega o vértice não visitado cujo o caminho é o menor
    int smallest_pos = -1;
    for(int i = 0; i < transposed_graph->vertices_n; i++)
    {   
        if(array_path[i]->lengh != -1 && array_path[i]->lengh < smallest && was_visited[i] == 0)
        {
            smallest = array_path[i]->lengh;
            smallest_pos = i;
        }
    }

    if(smallest_pos != -1)  //Repete o processo no vértice não visitado de menor caminho
    {
        was_visited[smallest_pos] = 1;
        dijkstra(transposed_graph, array_path, array_path[smallest_pos]->startpoint->nomeUsuarioqueSegue , was_visited);
    }
}
/*
    Printa na tela todos os caminhos de um vetor de caminhos.

    params:
        PATH** array_path => array de caminhos
        int size => tamanho do vetor de caminhos
    
    return:
        void
*/
void print_paths(PATH** array_path, int size)
{
    for(int i = 0; i < size; i++)
    {
        EDGE* p = array_path[i]->startpoint;

        if(array_path[i]->lengh == 0)   // Pula a celebridade
        {
            continue;
        }
        else if(array_path[i]->lengh == -1) //Não há um caminho da celebridade até o usuario
        {
            printf("NAO SEGUE A CELEBRIDADE\n\n");
            continue;
        }

        while (p != NULL)
        {
            printf("%s, %s, %s, ", p->nomeUsuarioqueSegue, p->nomeUsuarioqueESeguido, p->DataInicioQueSegue);
            if(strcmp("$$$$$$$$$$", p->DataFimQueSegue) == 0)
            {
                printf("NULO, ");
            }
            else
            {
                printf("%s, ", p->DataFimQueSegue);
            }
            if(p->grauAmizade == '$')
            {
                printf("NULO\n");
            }
            else
            {
                printf("%c\n", p->grauAmizade);
            }
            p = p->next;
        }

        printf("\n");
    }
}

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

void dijkstra_to_itself(GRAPH* transposed_graph, PATH** array_path, char* startpoint, int* was_visited)
{
    int pos = search_pos(transposed_graph, startpoint);
    if(array_path[pos]->lengh == -1)
    {
        was_visited[pos] = 1;   // Não seta o caminho até ele mesmo como zero
    }

    ADJ_NODE* p = transposed_graph->vertices_array[pos]->adj_head;
    while(p != NULL)
    {
        int next_vert_pos = search_pos(transposed_graph, p->nomeUsuarioqueESeguido);
        int still_following = strcmp(p->DataFimQueSegue, "$$$$$$$$$$");
        if(still_following == 0 &&(array_path[next_vert_pos]->lengh == -1 || array_path[next_vert_pos]->lengh > array_path[pos]->lengh + 1))
        {
            clone_path(array_path[pos], array_path[next_vert_pos]);
            if(array_path[next_vert_pos]->lengh == -1)  array_path[next_vert_pos]->lengh = 0;   //Para compensar, seta como zero caso o copiado tenha tamanho -1
            add_edge_to_path(p ,transposed_graph->vertices_array[pos], array_path[next_vert_pos]);
        }

        p = p->next;
    }

    int smallest = 100000000;
    int smallest_pos = -1;
    for(int i = 0; i < transposed_graph->vertices_n; i++)
    {   
        if(array_path[i]->lengh != -1 && array_path[i]->lengh < smallest && was_visited[i] == 0)
        {
            smallest = array_path[i]->lengh;
            smallest_pos = i;
        }
    }

    if(smallest_pos != -1)
    {
        was_visited[smallest_pos] = 1;
        dijkstra(transposed_graph, array_path, array_path[smallest_pos]->startpoint->nomeUsuarioqueSegue , was_visited);
    }
}