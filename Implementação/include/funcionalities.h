
/*
                                TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
                                Vinicius Reis Gonçalves             15491921
*/

// Arquivo 'functionalities.h', contém a definição das funcionalidades do trabalho

#ifndef FUNCTIONALITIES_H
#define FUNCTIONALITIES_H

#include "all.h"

/*
    Cria um arquivo de índice como especificado, atualizando o status quando necessário

    params:
        const char* index_filename => Nome do arquivo de índice a ser criado
    
    return:
        void
*/
void CREATE_INDEX(const char* index_filename);

/*
    Lê os dados de um arquivo fonte, grava num arquivo de dados 'output_filename' no formato binária e o indexa à um arquivo de índice primário 'index_filename' no formato binário, já existente

    params:
        const char* src_filename => Nome do arquivo fonte dos dados (já existente)
        const char* output_filename => Nome do arquivo binário gerado
        const cahr* index_filename => Nome do arquivo de índice (já existente)
    
    return:
        void
*/
void CREATE_TABLE(const char* src_filename, const char* output_filename, const char* index_filename);

/* 
    Recebe o nome do arquivo de dados, abre-o para leitura, caso exista, e printa seus registros 

    params:
        const char* data_filename => nome do arquivo a ser aberto

    return:
        void

*/

void SELECT_FROM_TABLE(const char* data_filename);

/* 
    Recebe o número de pesquisas a serem feitas e o nome do arquivo de dados,
    abre-o para leitura, caso exista, e recebe o tipo do campo e o valor do campo
    para usar na pesquisa, caso seja feita a pesquisa por ID, usa o arquivo de indice

    params:
        const char* data_filename => nome do arquivo de dados a ser aberto
        const char* index_filename => nome do arquivo de index caso seja nescessário
        int search number => número de pesquisas a serem feitas

    return:
        void

*/

void SELECT_FROM_WHERE(const char* data_filename, const char* index_filename, int search_number);

/*
    Recebe o número de pesquisas a serem feitas e o nome do arquivo de dados,
    abre-o para leitura e escrita, caso exista, e recebe do usuário 
    tipo do campo e o valor do campo
    para usar na pesquisa, caso seja feita a pesquisa por ID, usa o arquivo de indice.
    Marca os registros encontrado em cada pesquisa como lógicamente removidos.

    params:
        const char* data_filename => nome do arquivo de dados a ser aberto
        const char* index_filename => nome do arquivo de index
        int delete number => número de deleções a serem feitas

    return:
        void

*/

void DELETE_FROM_WHERE (char *data_filename, char *index_filename, int delete_number);

/*
    Recebe o número de pesquisas a serem feitas e o nome do arquivo de dados,
    abre-o para leitura e escrita, caso exista. O usuário informa os valores
    dos campos do registro a ser inserido, e a função insere-o no final

    params:
        const char* data_filename => nome do arquivo de dados a ser aberto
        const char* index_filename => nome do arquivo de index caso seja nescessário
        int insert_number => número de inserções a serem feitas

    return:
        void

*/

void INSERT_INTO(char* data_filename, char *index_filename, int insert_number);

#endif