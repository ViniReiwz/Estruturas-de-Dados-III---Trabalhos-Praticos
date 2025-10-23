/*
                                TRABALHO PRÁTICO - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
                                Vinicius Reis Gonçalves             15491921
*/

//Arquivo 'index.c', contém funções que trabalham com o arquivo de índice primário

#include "all.h"

/*
    Cria um registro de dados de índice primário em memória primária

    return:
        INDEX_DREG* index => Ponteiro para o registro de dados do índice
*/
INDEX_DREG* create_index_dreg()
{
    INDEX_DREG* index = (INDEX_DREG*)calloc(1,sizeof(INDEX_DREG));  // Aloca a memória
    index->byteOffset= -1;                                          // Inicializa byteOffset como -1 (null)
    index->idPessoa = -1;                                           // Inicializa idPessoa como -1 (null)
    return index;                                                   // Retorna o endereço da região de memória
}

/*
    Cria um vetor de registros de dados do arquivo de índice

    params:
        int len => Tamanho do vetor (n° de registros de índice)
    
    return:
        INDEX_ARR* idx => Ponteiro para INDEX_ARR com memória alocada para o vetor e seu tamanho
*/
INDEX_ARR* create_index_arr(int len)
{
    INDEX_ARR* idx = (INDEX_ARR*)calloc(1,sizeof(INDEX_ARR));   // Aloca memória para INDEX_ARR*
    idx->idx_arr = (INDEX_DREG*)calloc(len,sizeof(INDEX_DREG)); // Aloca a memória para o vetor de registros de dados do arquivo de índice
    idx->len = len;                                             // Salva o tamanho do vetor

    return idx;                                                 // Retorna o ponteiro para a região de memória alocada
}

/*
    Libera a memória de um vetor de registros de dados do arquivo de índice na memoria primária

    params:
        IDNEX_ARR* idxarr => Ponteiro para o vetor de registros a ser liberado
    
    return:
        void
*/

void destroy_index_arr(INDEX_ARR* idxarr)
{
    free(idxarr->idx_arr);                          // Libera a memória do array de registros
    free(idxarr);                                   // Libera a memória da estrutura
}

/*
    Cria um arquivo de índice primário com seu registro de cabeçalho.

    params:
        const char* filename => Nome do arquivo à ser criado.

    return:
        FILE* index_file_p => Ponteiro para arquivo de índice primário.
*/
FILE* create_index_file(const char* filename)
{
    char* file_path = get_file_path(filename);          // Encontra o caminho relativo do arquivo

    FILE* index_file_p = fopen(file_path,"wb");         // Abre o arquivo para escrita binária

    if(DEBUG)
    {
        printf("\nCaminho relativo do arquivo de índice: %s\n",file_path);    // Exibe o caminho relativo do arquivo de índice primário
    }

    free(file_path);                                    // Libera a memória do caminho do arquivo

    if(index_file_p == NULL)                            // Verifica se a abertura do arquivo não falhou
    {
        print_error();                                  // Exibe mensagem de erro
        if(DEBUG)
        {
            printf("\nArquivo não foi criado !\n");           // Exibe mensagem de debug caso DEBUG == 1 em own_utils.h
        }

        return NULL;                                        // Retorna NULL
    }


    char status = '0';                                  // Variável status para preencher o registro de cabeçalho

    char trash = '$';                                   // Variável trash para preencher o registro de cabeçalho

    fwrite(&status,1,1,index_file_p);                   // Escreve o status no primeiro byte offset do arquivo

    for(int i = 0 ; i < 11; i++)                        // Escreve lixo nos 11 bytes restantes do registro de cabeçalho
    {fwrite(&trash,1,1,index_file_p);}

    fseek(index_file_p,0,SEEK_SET);                     // Volta ao começo do arquivo

    return index_file_p;                                // Retorna o ponteiro para o arquvo
}

/*
    Relaciona o id ao byte offset e retorna o registro de dados do arquivo de índice em memória primária

    params:
        int id => id da pessoa cadastrada
        long int boffset => byte offser para o começo do registro de dados da pessoa com o referido id
    
        return INDEX_DREG index => registro de dados do arquivo de indíce em memória primária
*/
INDEX_DREG indexate(int id, long int boffset)
{
    INDEX_DREG index;           // Cria um registro de indíce em memória primária
    index.idPessoa = id;        // Atribui o id passado ao registro
    index.byteOffset = boffset; // Atribui o byte offset passado ao registro
    return index;               // Retorna o registro de dados
}

/*
    Escreve os dados da memória primária para o arquivo de índice

    params:
        FILE* index_file => Ponteiro para o arquivo de índice
        INDEX_ARR* idxarr => Tipo de dados que armazea todos os registros de índice  em memória primária, bem como o número de registros de índice
    return:
        void
*/
void write_on_index_file(FILE* index_file, INDEX_ARR* idxarr)
{   
    if(index_file == NULL) //testa se de fato o arquivo de indice já esta aberto
    {
        exit(EXIT_FAILURE);
    }
    
    fseek(index_file,IDX_HEAD_REG_LEN,SEEK_SET);                // Posiciona o ponteiro do arquivo depois do registro de cabeçalho

    for(int i = 0; i < idxarr->len; i++)                        // Varre todos os registros de índice, escrevendo um à um no arquivo
    {
        fwrite(&idxarr->idx_arr[i].idPessoa,4,1,index_file);
        fwrite(&idxarr->idx_arr[i].byteOffset,8,1,index_file);
    }

    int file_descriptor = fileno(index_file);
    __off_t num = 12 + 12*idxarr->len;   
    ftruncate(file_descriptor, num);    // Termina o arquivo exatamente após o último registro de indice
    
    if(DEBUG){printf("\nOFFSET FINAL DE INDÍCE pelo ftell --> %li\n",ftell(index_file));}    // Exibe o offset em que o ponteiro de file parou
}

/*
    Ordena os registros de índice utilizando o bubble sor aprimorado

    params:
        INDEX_ARR* index_arr => Endereço do tipo INDEX_ARR, que contém o vetor a ser ordenado

    return:
        void
*/
void order_index(INDEX_ARR* index_arr)
{   
    //Bubble sort aprimorado:

    int aux = index_arr->len;                                                       // Variável auxiliar que carrega o tamanho do vetor
    int swapped = 1;                                                                // Variável auxiliar que guarda a informação se o vetor foi alterado ou não
    while (swapped)                                                                 // Atua enquanto o vetor estiver sendo alterado (ordenando-se)
    {
        swapped = 0;                                                                // Setado pra zero, logo sai do loop caso não seja atualizado
        for(int i = 0; i < aux - 1; i ++)                                           // Percorre todo o vetor até que i+1 seja o último elemento
        {
            if(index_arr->idx_arr[i].idPessoa > index_arr->idx_arr[i+1].idPessoa)   // Verifica se o elemento i é > do que i + 1 (desorndenado)
            {
                INDEX_DREG hold = index_arr->idx_arr[i];                            // Variável auxiliar que guarda o valor da posição i
                index_arr->idx_arr[i] = index_arr->idx_arr[i+1];                    // Troca o conteúdo das posições
                index_arr->idx_arr[i+1] = hold;                                     
                swapped = 1;                                                        // Indica que houve troca
            }
        }
        aux--;                                                                      // Decrementa no aux, pois o último elemento sempre estará ordenado ao fim de cada loop
    }
    
}

/*
    Recebe um arquivo de indice já aberto e puxa para memória primária seu conteúdp 

    params:
        FILE* index_file => Arquivo de indice já aberto

    return:
        INDEX_ARR* idx_array => endereço do array de indice
*/

INDEX_ARR* save_index_in_mem(FILE* index_file)
{
    //Termina o programa caso seja passaddo um arquivo de indice não aberto
    if(index_file == NULL)
    {
        print_error();
        exit(EXIT_FAILURE);
    }

    //Pega o tamanho do arquivo de indice em bytes
    fseek(index_file, 0, SEEK_END);
    long index_end = ftell(index_file);
    fseek(index_file, IDX_HEAD_REG_LEN, SEEK_SET);

    // Calculo quantos registro, sem cabeçalho, o arquivo de indice tem
    int len = (index_end - 11)/12;

    INDEX_ARR* idx_array = create_index_arr(len); //Cria um array de indice com o tamanho apropriado

    for(int i = 0; i < len; i++)
    {
        fread(&(idx_array->idx_arr[i].idPessoa), 4, 1, index_file);
        fread(&(idx_array->idx_arr[i].byteOffset), 8, 1, index_file);
    }
    
    return idx_array;
}

/*
    Recebe um array de indice e um ID, retornando a posição no array daquele ID. 

    params:
        INDEX_ARR* idx_array => array de índices
        int id => id a ser procurado

    return:
        int middle => posição do ID procurado
*/

int index_binary_search(INDEX_ARR* idx_array, int id)
{
    int len = idx_array->len;

    int left = 0;
    int right = len - 1;
    int middle;

    // BUSCA BINÀRIA

    while (1)
    {
        middle = left + (right - left) / 2; // calcula o meio

        if (left == right || idx_array->idx_arr[middle].idPessoa == id) // caso as extremidades sejam iguais sai do while
        {                                                               // ou caso o conteudo do meio seja igual ao id
            break;
        }
        // O meio mais ou menos um vira a nova esquerda ou direita, respectivamente
        else if (idx_array->idx_arr[middle].idPessoa > id)
        {
            right = middle - 1;
        }
        else
        {
            left = middle + 1;
        }
    }

    // Caso o meio não seja o id no fim do processo retorna -1
    if (idx_array->idx_arr[middle].idPessoa != id)
    {
        return -1;
    }
    else // Caso o contrario retorna o byteoffset desejado
    {
        return middle;
    }
}

/*
    Recebe um array de indice e um ID, removendo o registro com esse ID do array

    params:
        INDEX_ARR* idx_array => array de índices
        int id => id a ser procurado

    return:
        void
*/


void remove_id_array(INDEX_ARR* idx_array, int id)
{
    int pos = index_binary_search(idx_array, id);   // acha a posição desse ID no array
    int len = idx_array->len;

    for(int i = pos; i < len - 1; i++)  //Traz todos os registros a partir do ID uma posição
    {                                   //para frente apagando o registro do ID
        idx_array->idx_arr[i].byteOffset = idx_array->idx_arr[i + 1].byteOffset;
        idx_array->idx_arr[i].idPessoa = idx_array->idx_arr[i + 1].idPessoa;
    }

    idx_array->len--;
}

/*
    Recebe um array de indice, um ID e um byte_offset, adicionando no final
    o registro com esse ID e byte_offset no array

    params:
        INDEX_ARR* idx_array => array de índices
        int id => id do registro a ser adicionado
        long byte_offset => byte_offset do registro a ser adicionado

    return:
        void
*/

void add_id_array(INDEX_ARR** idx_array, int id, long byte_offset)
{
    int len = (*idx_array)->len;
    INDEX_ARR* new_index_array = create_index_arr(len + 1); //cria um index com um elemento a mais

    for(int i = 0; i < len; i++)  //copia item a item de um array para o outro
    {
        new_index_array->idx_arr[i].byteOffset = (*idx_array)->idx_arr[i].byteOffset;
        new_index_array->idx_arr[i].idPessoa = (*idx_array)->idx_arr[i].idPessoa ;
    }

    //adiciona o novo elemento no final
    new_index_array->idx_arr[len].byteOffset = byte_offset;
    new_index_array->idx_arr[len].idPessoa = id;

    destroy_index_arr(*idx_array);  //desaloca o antigo array de index
    *idx_array = new_index_array;   //passa por parametros o novo array para o ponteiro do antigo
}