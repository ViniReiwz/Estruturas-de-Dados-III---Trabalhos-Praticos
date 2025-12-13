/*
                                TRABALHO PRÁTICO - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
                                Vinicius Reis Gonçalves             15491921
*/

//Arquivo 'own_utils.h', contém as definições de funções e valores fixos auxiliares para a execução do código

#ifndef OWN_UTILS_H
#define OWN_UTILS_H

#define OPT_LEN 100         // Tamanho máximo da string que contém os argumentos => (opc arquivo1 ... arquivoN)
#define MAX_ARGS 10         // Número máximo de argumentos para cada opção
#define ARG_LEN 50          // Tamanho médio de cada argumento
#define FILE_DIR "./"       // Pasta em que todos os arquivos devem ser procurados ou salvos
#define DEBUG 0             // Variável de deubg, habilita os prints nas funções
#define ERROR_MSG "Falha no processamento do arquivo." // Variável de mensagem de erro padrão

#include "all.h"

/*
    Recebe uma string como parâmetro e a separa de acordo com os delimitadores especificados.

    params:
        char unstriped_str[] => String estática que contém as sub-strings separadas por 'delim'
        char delim => Delimitador de sub-strings
        const int stop_on_first => Variável que indica se deve parar na primeira ocorrência do delimitador


    return:
        char** args => Retorna uma "matriz de strings", onde cada posição args[i] contém o conteúdo da string separada pelo espaço.
*/
char** strip_by_delim(const char unstriped_str[],const char delim,int stop_on_first);

/*
    Libera a memória da matriz com as strings separadas por algum delimitador

    params:
        char** strip_m => Matriz cuja memória deseja ser liberada
    
    return:
        void
*/
void destroy_strip_matrix(char** strip_m);

/*
    Termina a string (substitui por '\0') quano encontrar a primeira ocorrência de um marcador

    params:
        char* str => String a ser terminada;
        char mark => Marcador a ser substituído por '\0'
        
    
    return:
        void
*/
void end_string_on_mark(char* str, const char *mark);

/*
    Exibe o menu com as funcionalidades e seus respectiovs argumentos.

    return:
        void.
*/
void print_menu();


/*
Exibe a mensagem de erro padrão, definida em ERROR_MSG em own_utils.h
*/
void print_error();

/*
    Exibe um registro carregado em memória primária
    
    params:
        DATA_DREG* ddreg => Registro de dados em memória primária a ser exibido
    
    return:
        void
*/
void print_ddreg(DATA_DREG* ddreg);

/*
    Exibe um registro de dadoos do arquivo de índice em memória primária

    INDEX_DREG idx => Registro a ser exibido

    return:
        void
*/
void print_index(INDEX_DREG idx);

/*
    Exibe um registro de dados do arquivo binário do tipo 'segue' carregado em memória primária

    params:
        FOLLOW_DREG flw_dreg => Registro a ser exibido;
    
    return:
        void.
*/
void print_follow_dreg(FOLLOW_DREG flw_dreg);

/*
    Retorna o caminho relativo do arquivo

    params:
        const char* filename => Nome_do_arquivo.extensão (arquivo.bin, por exemplo);
    
    return:
        char* file_path => String que contém o caminho relativo do arquivo, terminada em '\0'
*/
char* get_file_path(const char* filename);

/*
    Atualiza o campo 'status' do arquivo de cabeçaho do registro de índice

    params:
        FILE* file => Ponteiro do arquivo que deseja-se atualizar o status
        char status => char do status '1', consistente '0', inconsistente
    
    return:
        void
*/
void update_file_status(FILE* file, char status);

/*
    Remove as aspas de uma string passada, caso tenha aspas

    params:
        char* str => string com aspas a serem retiradas
    
    return:
        void
*/

void remove_quotes(char* str);

/*
    Remove todos os caracteres da string até achar o primeiro espaço, remove o espaço também.

    params:
        char* str => string de entrada
    
    return:
        void
*/
void remove_everychar_until_space(char *str);

/*
    Inverte a string de data do formato dd/mm/aaaa para aaaa/mm/dd

    params:
        char* date => String a ser invertida;
    
    return:
        char* reversed_date => String já invertida.
*/
char* reverse_date_string(char* date);

/*
    Lê o valor a ser buscado seguindo a formatação especificada na descrição do trabalho

    return:
        char** type_and_val => Matriz com o tipo do campo na posição 1 e o valor na posição 2
*/
char** read_for_search();


/*
    Recupera o nome de usuário de uma pessoa através de seu id

    params:
        int idPessoa => Id a ser referenciado
        INDEX_ARR* index_arr => Arquivo de 'indíce' em memória primária
        FILE* data_file => Arquivo do tipo 'pessoa'

    return:
        char* nomeUsuario => Valor NULL para usuário inválido
*/
char* search_username(int idPessoa, INDEX_ARR* index_arr, FILE* data_file);

/*
    Conta o número de usuários válidos

    params:
        FOLLOW_ARR* f_arr => Arquivo do tipo 'segue' em memória primária
        INDEX_ARR* idx_arr => Arquivo do tipo 'indicce' em memória primária
    
    return:
        int count => Número de ids (usuários) válidos
*/
int count_existing_ids(FOLLOW_ARR* f_arr,INDEX_ARR* idx_arr);

#endif