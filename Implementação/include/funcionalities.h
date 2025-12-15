
/*
                                TRABALHO PRÁTICO - ESTRUTURA DE DADOS III
                                
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

/*
    Recebe o número de updates a serem feitas e o nome do arquivo de dados,
    abre-o para leitura e escrita, caso exista. O usuário informa o valore
    e o campo do registro a ser buscado e a ser alterado.

    params:
        const char* data_filename => nome do arquivo de dados a ser aberto
        const char* index_filename => nome do arquivo de index caso seja nescessário
        int update_number => número de atualizações a serem feitas

    return:
        void

*/

void UPDATE_SET_WHERE(char* data_filename, char *index_filename, int update_number);

/*
    Lê do arquivo 'segue' e ordena seus registros de forma crescente, seguindo a hierarquia:
        idPessoaQueSegue -> idPessoaQueESeguida -> dataInicioQueSegue -> dataFimQueSegue
    
    params:
        const char* src_filename => Nome do arquivo que deseja-se ordenar os registros;
        const char* ord_dest_filename => Nome do arquivo que receberá os registros ordenados;
    
    return:
        void.
*/
void ORDER_BY(const char* src_filename, const char* ord_dest_filename);

/*
    Lê os dados de um arquivo fonte, grava num arquivo de dados "follow file"

    params:
        const char* csv_filename => Nome do arquivo fonte dos dados (já existente)
        const char* follow_filename => Nome do arquivo binário gerado

    return:
        void
*/
void CREATE_FOLLOW_TABLE(char* csv_filename, char* follow_filename);


/*
    Implementa a funcionalidade SELECT_FROM_JOIN_ON -> Imprime todos os registros do arquivo follow relacionados
    às pessoas com 'idPessoa' no arquivo do tipo 'pessoa' igual à 'idPessoaQueSegue' no arquivo do tio 'segue'

    params:
        const char* data_filename => Nome do arquivo do tipo 'pessoa'
        const char* index_filename => Nome do arquivo de índice
        const char* follow_filename => Nome do arquivo do tipo 'segue'
        const int search_number => Núemro de buscas a serem feitas no arquivo do tipo 'pessoa'

    return:
        void;
*/
void SELECT_FROM_JOIN_ON(const char* data_filename, const char* index_filename, const char* follow_filename, const int search_number);

/*
    Cria um grafo baseado nos arquivos do tipo 'pessoa','indice' e 'segue', exibindo-o de maneira formatada

    params:
        const char* data_filename => Nome do arquivo de dados
        const char* index_filename => Nome do arquivo de indíce
        const char* follow_filename => Nome do arquivo do tipo 'segue'
    
    return:
        void
*/
void GENERATE_GRAPH(const char* data_filename, const char* index_filename, const char* follow_filename);

/*
    Cria um grafo baseado nos arquivos do tipo 'pessoa','indice' e 'segue'. Depois, transpõe-no e exibe de maneira frmatada

    params:
        const char* data_filename => Nome do arquivo de dados
        const char* index_filename => Nome do arquivo de indíce
        const char* follow_filename => Nome do arquivo do tipo 'segue'
    
    return:
        void
*/
void GENERATE_TRANSPOSED(const char* data_filename, const char* index_filename, const char* follow_filename);

void SHORTEST_PATH_TO_USER(const char* data_filename, const char* index_filename, const char* follow_filename, char* username);

void SHORTEST_PATH_TO_ITSELF(const char* data_filename, const char* index_filename, const char* follow_filename, char* username);

#endif