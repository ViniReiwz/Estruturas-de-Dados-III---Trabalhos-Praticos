/*
                                TRABALHO PRÁTICO - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
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

/*
    Recebe um arquivo de indice já aberto e puxa para memória primária seu conteúdp 

    params:
        FILE* index_file => Arquivo de indice já aberto

    return:
        INDEX_ARR* idx_array => endereço do array de indice
*/

INDEX_ARR* save_index_in_mem(FILE* index_file);

/*
    Recebe um array de indice e um ID, retornando a posição no array daquele ID. 

    params:
        INDEX_ARR* idx_array => array de índices
        int id => id a ser procurado

    return:
        int middle => posição do ID procurado
*/

int index_binary_search(INDEX_ARR* idx_array, int id);

/*
    Recebe um array de indice e um ID, removendo o registro com esse ID do array

    params:
        INDEX_ARR* idx_array => array de índices
        int id => id a ser procurado

    return:
        void
*/


void remove_id_array(INDEX_ARR* idx_array, int id);

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

void add_id_array(INDEX_ARR** idx_array, int id, long byte_offset);

/*
    Recebe um array de indice, um ponteiro para um arquivo de indice e seu nome, caso
    o arquivo esteja fechado, abre-o para leitura e escrita e puxa o indice para memória primária

    params:
        FILE* index_file => arquivo de indice
        INDEX_ARR* idx_array => array de índices
        char* index_filename => nome do arquivo

    return:
        void
*/

void open_and_pull_index(FILE** index_file, INDEX_ARR** idx_array, char* index_filename);

#endif