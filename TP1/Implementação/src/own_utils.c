/*
                                TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                XXXXXXXX
                                Vinicius Reis Gonçalves             15491921
*/

//Arquivo 'own_utils.c', contém funções auxiliares para a execução do código

#include "all.h"

/*
    Recebe uma string como parâmetro e a separa de acordo com os delimitadores especificados.

    params:
        const char unstriped_str[] => String estática que contém as sub-strings separadas por 'delim'
        const char delim => Delimitador de sub-strings


    return:
        char** args => Retorna uma "matriz de strings", onde cada posição args[i] contém o conteúdo da string separada pelo espaço
*/
char** strip_by_delim(const char unstriped_str[], const char delim)
{   
    int len = strlen(unstriped_str);                    // Atribui o tamanho da string original à 'len'
    char **args = (char**)calloc(len,sizeof(char*));    // Aloca memória suficiente para a matri de strings
    char* aux = (char*)calloc(len,sizeof(char));        // Aloca memória para um a string auxiliar

    int mpos = 1;                                       // Índice da matriz, iniciado em 1 pois matriz[0] contém o n° de strings
    int n_str = 0;                                      // Número de strings separadas

    args[0] = (char*)calloc(4,sizeof(char));            // Aloca 4 bytes (tamanho de inteiro) para guardar o número de strings

    int idx = 0;                                        // Indíce auxiliar

    for(int i = 0; i < len; i ++)                       // Loop para percorrer toda a string original (unstriped)
    {
        if(unstriped_str[i] != '\0' && unstriped_str[i] != '\n' && unstriped_str[i] != delim)   // Verifica se encontrou algum destes demarcadores de 'fim da string', incluindo o delimitador
        {
            aux[idx] = unstriped_str[i];                // Enquanto não encontrar, copia caracter à caracter
            idx++;                                      // Incrementa o indíce auxiliar
        }

        else                                            // Se encontrar:
        {
            args[mpos] = (char*)calloc(strlen(aux) + 1,sizeof(char));   // Aloca na posição mpos o sufuciente para receber a string separada

            strcpy(args[mpos],aux);                     // Copia a string

            memset(aux,0,strlen(aux));                  // Reseta aux para '\0'

            mpos ++;                                    // Incrementa a posição na matriz
            idx = 0;                                    // Reseta o índice auxiliar
            n_str ++;                                   // Incrementa o número de strings separadas
        }
        
    }

    args[0][0] = (n_str) + '0';                         // Transforma em char o valor de n_str e atribui à posição 0,0 da matriz
    int bn = strcspn(args[mpos-1],"\n");                // Encontra na última string o último '\n'
    args[mpos-1][bn] = '\0';                            // Atribui '\0' à posição onde encontrou '\n'

    return args;                                        // Retorna a matriz
}

/*
    Exibe o menu com as funcionalidades e seus respectivos argumentos
*/
void print_menu()
{   
    puts(""); //Exibe nova linha
    puts("1 -- CREATE INDEX --> arquivoIndicePrimario.bin");
    puts("2 -- CREATE TABLE --> arquivoEntrada.csv arquivoSaida.bin arquivoIndicePrimario.bin");
    puts("3 -- SELECT FROM --> arquivoEntrada.bin");
    puts("4 -- SELECT FROM WHERE --> arquivoEntrada.bin arquivoIndicePrimario.bin n");
    puts(""); //Exibe nova linha
}

/*
    Exibe a mensagem de erro padrão, definida em ERROR_MSG em own_utils.h
*/
void print_error()
{
    printf("%s",ERROR_MSG);
}

/*
    Exibe um registro carregado em memória primária
    
    params:
        DATA_DREG* ddreg => Registro de dados em memória primária a ser exibido
    
    return:
        void
*/
void print_ddreg(DATA_DREG* ddreg)
{
    printf("\'removido\' no registro: %c\n",ddreg->removido);
    printf("\'tamanhORegistro\' no registro: %i\n",ddreg->tamReg);
    printf("\'idPessoa\' no registro: %i\n",ddreg->idPessoa);
    printf("\'idadePessoa\' no registro: %i\n",ddreg->idadePessoa);
    printf("\'tamanhoNomePessoa\' no registro: %i\n",ddreg->tamNomePessoa);
    printf("\'nomePessoa\' no registro: %s\n",ddreg->nomePessoa);
    printf("\'tamanhoNomeUsuário\' no registro: %i\n",ddreg->tamNomeUsuario);
    printf("\'nomeUsuário\' no registro: %s\n",ddreg->nomeUsuario);
}

/*
    Retorna o caminho relativo do arquivo

    params:
        const char* filename => Nome_do_arquivo.extensão (arquivo.bin, por exemplo);
    
    return:
        char* file_path => String que contém o caminho relativo do arquivo, terminada em '\0'
*/
char* get_file_path(const char* filename)
{
    int path_len = strlen(FILE_DIR) + strlen(filename) + 1; // Calcula o tamanho do caminho (Diretório + Nome do arquivo + '\0') 
    char* file_path = (char*)calloc(path_len,sizeof(char)); // Aloca espaço para o caminho relativo

    sprintf(file_path,"%s%s",FILE_DIR,filename);                   // Concatena as strings do Diretório e Nome

    return file_path;                                       // Retorna o caminho relativo

}