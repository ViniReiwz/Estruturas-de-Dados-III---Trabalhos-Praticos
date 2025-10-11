/*
                                TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III
                                
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
    Libera a memória de um registro de dados do arquivo de índice na memória primária

    params:
        INDEX_DREG* idxdreg => Ponteiro para o registro de dados a ser liberado da memória
    
    return:
        void
*/
// void destroy_index_dreg(INDEX_DREG* idxdreg)
// {
//     free(idxdreg);
// }

/*
    Libera a memória de um vetor de registros de dados do arquivo de índice na memoria primária

    params:
        IDNEX_ARR* idxarr => Ponteiro para o vetor de registros a ser liberado
    
    return:
        void
*/
void destroy_index_arr(INDEX_ARR* idxarr)
{
    // int len = idxarr->len;                          // Grava o tamanho do vetor
    // for(int i = 0; i < len ; i ++)                  // Percorre todo o cetor
    // {
    //     destroy_index_dreg(&idxarr->idx_arr[i]);    // Destroi cada registro de dados presente no vetor
    // }

    free(idxarr->idx_arr);                          // Libera a memória do ponteiro propriamente dito
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
    fseek(index_file,IDX_HEAD_REG_LEN,SEEK_SET);                // Posiciona o ponteiro do arquivo depois do registro de cabeçalho

    for(int i = 0; i < idxarr->len; i++)                        // Varre todos os registros de índice, escrevendo um à um no arquivo
    {
        fwrite(&idxarr->idx_arr[i].idPessoa,4,1,index_file);
        fwrite(&idxarr->idx_arr[i].byteOffset,8,1,index_file);
    }

    
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


INDEX_ARR* save_index_in_mem(FILE* index_file)
{
    if(index_file == NULL)
    {
        print_error();
        exit(EXIT_FAILURE);
    }

    fseek(index_file, 0, SEEK_END);
    long index_end = ftell(index_file);
    fseek(index_file, IDX_HEAD_REG_LEN, SEEK_SET);

    int len = (index_end - 11)/12;

    INDEX_ARR* idx_array = create_index_arr(len);

    int i = 0;
    while (i < len)
    {
        fread(&(idx_array->idx_arr[i].idPessoa), 4, 1, index_file);
        fread(&(idx_array->idx_arr[i].byteOffset), 8, 1, index_file);

        i = i + 1;
    }
    
    return idx_array;
}