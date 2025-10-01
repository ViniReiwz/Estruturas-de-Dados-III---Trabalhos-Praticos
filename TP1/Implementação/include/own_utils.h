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
#define DEBUG 1             // Variável de deubg, habilita os prints nas funções
#define ERROR_MSG "\nFalha ao processar arquivo!\n" // Variável de mensagem de erro padrão

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
    Retorna o caminho relativo do arquivo

    params:
        const char* filename => Nome_do_arquivo.extensão (arquivo.bin, por exemplo);
    
    return:
        char* file_path => String que contém o caminho relativo do arquivo, terminada em '\0'
*/
char* get_file_path(const char* filename);



#endif