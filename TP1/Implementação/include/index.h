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
    INDEX_DREG** idx_arr;       // Array propriamente dito

}INDEX_ARR;

/*
    Cria um registro de dados de índice primário em memória primária

    return:
        INDEX_DREG* index => Ponteiro para o registro de dados do índice
*/
INDEX_DREG* create_index_dreg();

/*
    Cria um arquivo de índice primário com seu registro de cabeçalho.

    params:
        const char* filename => Nome do arquivo à ser criado.

    return:
        FILE* index_file_p => Ponteiro para arquivo de índice primário.
*/
FILE* create_index_file(const char* filename);



#endif