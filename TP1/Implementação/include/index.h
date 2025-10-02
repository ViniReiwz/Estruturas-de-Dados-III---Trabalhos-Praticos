/*
                                TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                XXXXXXXX
                                Vinicius Reis Gonçalves             15491921
*/

//Arquivo 'index.h', contém as definições de função, tipos de dados e valores fixos relacionados ao arquivo de índice

#ifndef INDEX_H
#define INDEX_H

#define IDX_HEAD_REG_LEN 12  // Tamanho do registro de cabeçalho do arquivo de índice em bytes

#include "all.h"

typedef struct _index_dreg   // Registro de dados do arquivo de índice primário
{
    int idPessoa;               // Id da pessoa armazenada no arquivo de dados (4 bytes)
    long int byteOffset;        // Byte Offset do referido id   (8 bytes)

}INDEX_DREG;

typedef struct _index_dreg_arr  // Array de registros do arquivo de indíce primário
{
    int len;                    // Tamanho do array (n° de registros)
    INDEX_DREG* idx_arr;       // Array propriamente dito

}INDEX_ARR;

/*
    Cria um registro de dados de índice primário em memória primária

    return:
        INDEX_DREG* index => Ponteiro para o registro de dados do índice
*/
INDEX_DREG* create_index_dreg();

/*
    Cria um vetor de registros de dados do arquivo de índice

    params:
        int len => Tamanho do vetor (n° de registros de índice)
    
    return:
        INDEX_ARR* idx => Ponteiro para INDEX_ARR com memória alocada para o vetor e seu tamanho
*/
INDEX_ARR* create_index_arr(int len);

/*
    Libera a memória de um registro de dados do arquivo de índice na memória primária

    params:
        INDEX_DREG* idxdreg => Ponteiro para o registro de dados a ser liberado da memória
    
    return:
        void
*/
void destroy_index_dreg(INDEX_DREG* idxdreg);

/*
    Libera a memória de um vetor de registros de dados do arquivo de índice na memoria primária

    params:
        IDNEX_ARR* idxarr => Ponteiro para o vetor de registros a ser liberado
    
    return:
        void
*/
void destroy_index_arr(INDEX_ARR* idxarr);

/*
    Cria um arquivo de índice primário com seu registro de cabeçalho.

    params:
        const char* filename => Nome do arquivo à ser criado.

    return:
        FILE* index_file_p => Ponteiro para arquivo de índice primário.
*/
FILE* create_index_file(const char* filename);

/*
    Relaciona o ida ao byte offset e retorna o registro de dados do arquivo de índice em memória primária

    params:
        int id => id da pessoa cadastrada
        long int boffset => byte offser para o começo do registro de dados da pessoa com o referido id
    
        return INDEX_DREG* index => registro de dados do arquivo de indíce em memória primária
*/
INDEX_DREG indexate(int id, long int boffset);

/*
    Escreve os dados da memória primária para o arquivo de índice

    params:
        FILE* index_file => Ponteiro para o arquivo de índice
        INDEX_ARR* idxarr => Tipo de dados que armazea todos os registros de índice  em memória primária, bem como o número de registros de índice
    return:
        void
*/
void write_on_index_file(FILE* index_file, INDEX_ARR* idxarr);

/*
    Ordena os registros de índice utilizando o bubble sor aprimorado

    params:
        INDEX_ARR* index_arr => Endereço do tipo INDEX_ARR, que contém o vetor a ser ordenado

    return:
        void
*/
void order_index(INDEX_ARR* index_arr);



#endif