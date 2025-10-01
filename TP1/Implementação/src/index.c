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


// INDEX_DREG* indexate(DATA_DREG* ddreg, long int boffset)
// {
//     INDEX_DREG* index = create_index_dreg()
// }