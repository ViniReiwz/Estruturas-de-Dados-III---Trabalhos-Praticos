#ifndef INDEX_H
#define INDEX_H

#define IDX_HEAD_REG_LEN 12  // Tamanho do registro de cabeçalho do arquivo de índice em bytes

#include "all.h"

typedef struct _index_dreg   // Registro de dados do arquivo de índice primário
{
    int idPessoa;               // Id da pessoa armazenada no arquivo de dados (4 bytes)
    long int byteOffset;        // Byte Offset do referido id   (8 bytes)
    struct _index_dreg* next;   // Ponteiro para o próximo nó
    struct _index_dreg* ant;    // Ponteiro para o nó anterior

}INDEX_DREG;

typedef struct _index_hreg   // Registro de cabeçalho do arquivo de índice primário
{
    char status;            // Campo status - Pode estar em '0' (está sendo manipualado) ou '1' (terminou de manipular)
}INDEX_HREG;

typedef struct index_list
{
    
    INDEX_DREG* head;
    INDEX_DREG* tail;

}INDEX_LIST;



/*
    Cria um arquivo de índice primário com seu registro de cabeçalho.

    params:
        const char* filename => Nome do arquivo à ser criado.

    return:
        FILE* index_file_p => Ponteiro para arquivo de índice primário.
*/
FILE* create_index_file(const char* filename);


#endif