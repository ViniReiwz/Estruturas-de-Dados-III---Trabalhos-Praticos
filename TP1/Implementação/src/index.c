/*
                                TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                XXXXXXXX
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
        printf("\nFalha no processamento de arquivo\n");      // Exibe mensagem de erro
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
    Relaciona o ida ao byte offset e retorna o registro de dados do arquivo de índice em memória primária

    params:
        int id => id da pessoa cadastrada
        long int boffset => byte offser para o começo do registro de dados da pessoa com o referido id
    
        return INDEX_DREG* index => registro de dados do arquivo de indíce em memória primária
*/
INDEX_DREG* indexate(int id, long int boffset)
{
    INDEX_DREG* index = create_index_dreg();    // Cria um registro de indíce em memória primária
    index->idPessoa = id;                       // Atribui o id passado ao registro
    index->byteOffset = boffset;                // Atribui o byte offset passado ao registro
    return index;                               // Retorna o registro de dados
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
}

// void load_from_data_list(INDEX_ARR* idxarr, DATA_LIST* dlist)
// {
//     idxarr->len = dlist->header_reg->qtdPessoas;
//     DATA_DREG* p = dlist->head;
//     int i = 0;
//     while (p!=NULL)
//     {
//         idxarr->idx_arr[i] = indexate(p->idPessoa,)
//     }
// }

// void fill_index_file(FILE* index_file, DATA_LIST dlist)
// {

// }