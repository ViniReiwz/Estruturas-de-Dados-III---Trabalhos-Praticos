/*
                                TRABALHO PRÁTICO - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
                                Vinicius Reis Gonçalves             15491921
*/

// Arquivo 'follow.h', contém a definição das funções que lidam com o arquivo 'segue'

#ifndef FOLLOW_H
#define FOLLOW_H

#define DATE_LEN 10
#define FOLLOW_DATA_REG_LEN 30

#include "all.h"

typedef struct _follow_hreg // Registro de cabeçalho do arquivo 'segue'
{
    int qtdPessoas;         // Campo 'qtdPessoas' que representam a quantidade de registros do arquivo de dados
    int proxRRN;

}FOLLOW_HREG;


typedef struct _follow_dreg
{
    char removido;
    int idPessoaQueSegue;
    int idPessoaQueESeguida;
    char* dataInicioQueSegue;
    char* dataFimQueSegue;
    char grauAmizade;

}FOLLOW_DREG;

typedef struct _follow_dreg_array
{

    int len;
    FOLLOW_HREG* follow_hreg;
    FOLLOW_DREG* follow_arr;

}FOLLOW_ARR;

/*
    Cria um registro de dados do arquivo 'segue' em memória primária

    return:
        FOLLOW_DREG* follow_dreg => Registro de dados do arquivo 'segue' em memória primária com valores inicializados
        como nulo seguindo as especificações do trabalho.

*/
FOLLOW_DREG* create_follow_dreg();

/*
    Cria um registro de cabeçalho do arquivo 'segue' em memória primária

    return:
        FOLLOW_HREG* follow_hreg => Registro de cabeçalho do arquivo 'segue' em memória primária com valores nulos segundo a especificação.
*/
FOLLOW_HREG* create_follow_hreg();

/*
    Cria um vetor de registros de dados do arquivo 'segue'

    params:
        int len => Tamanho do vetor à ser criado;

    return:
        FOLLOW_ARR* follow_array => Endereço da região de memória alocada com o tamanho e o vetor deregistros inicialmente vazios.
*/
FOLLOW_ARR* create_follow_arr(int len);

/*
    Libera a memória alocada para uma variável do tipo FOLLOW_DREG*

    params:
        FOLLOW_DREG* follow_dreg => Registro à ser destruído (memória liberada);
    
    return:
        void.
*/
void destroy_follow_dreg(FOLLOW_DREG* follow_dreg);

/*
    Libera a memória alocada de uma variável do tipo FOLLOW_HREG*

    params:
        FOLLOW_HREG* follow_hreg => Registro de cabeçalho a ter memória liberada;
    
    return:
        void.
*/
void destroy_follow_hreg(FOLLOW_HREG* follow_hreg);

/*
    Libera a memória de uma variável do tipo FOLLOW_ARR*

    params:
        FOLLOW_ARR* follow_array => Vetor de registros de dados cuja memória quer ser liberada;
    
    return:
        void.
*/
void destroy_follow_array(FOLLOW_ARR* follow_array);

/*
    Lê um arquivo binário do tipo 'segue' e coloca seus dados em memória primária

    params:
        FILE* follow_file => Ponteiro para o arquivo do tipo 'segue';
    
    return:
        FOLLOW ARR* f_arr => Estrutura de dados contendo o arquivo 'segue' inteiro e memória primária, com o número de registros de dados, o registro de cabeçalho e o registro de dados propriamente ditos, sendo estes organizados em um vetor.
*/
FOLLOW_ARR* read_follow_file(FILE* follow_file);

/*
    Escreve os dados em um arquivo binário do tipo 'segue'

    params:
        FILE* follow_file => Ponteiro para o arquivo a ser escrito;
        FOLLOW_ARR* f_arr => Dados do arquivo em memória primária;
    
    return:
        void.
*/
void write_on_follow_file(FILE* follow_file,FOLLOW_ARR* f_arr);

/*
    Função que compara dois registros de dados do arquivo 'segue', em função dos ids e, se necessário, das datas

    params:
        FOLLOW_DREG a,b => Registros a serem comparados;
    
    return:
        int comparison => Valor menor ou maior que 0, que representa a 'posição' na ordenação que 'a' está em relação à 'b'.
*/
int compare_follow_dreg(const void* a, const void* b);

/*
    Ordena um vetor de registros do arquivo 'segue', seguindo as especificações do trabalho prático:
        idPessoaQueSegue -> idPessoaQueESeguido -> dataInicioSegue -> dataFimSegue

    params:
        FOLLOW_ARR* f_arr => Vetor à ser ordenado
*/
void ordenate_follow_dreg(FOLLOW_ARR* f_arr);

#endif