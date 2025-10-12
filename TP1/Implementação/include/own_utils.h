/*
                                TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                XXXXXXXX
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
        char unstriped_str[] => String estática que contém as sub-strings separadas por 'delim';
        char delim => Delimitador de sub-strings.

    return:
        char** args => Retorna uma "matriz de strings", onde cada posição args[i] contém o conteúdo da string separada pelo espaço.
*/
char** strip_by_delim(const char unstriped_str[],const char delim);

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
void end_string_on_mark(char* str, const char mark);

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

char* remove_quotes(char* str);


#endif