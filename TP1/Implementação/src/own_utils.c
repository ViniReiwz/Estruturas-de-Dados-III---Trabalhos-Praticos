/*
                                TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                XXXXXXXX
                                Vinicius Reis Gonçalves             15491921
*/

//Arquivo 'own_utils.c', contém funções auxiliares para a execução do trabalho

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
    int len = strlen(unstriped_str) + 1;
    char **args = (char**)calloc(len,sizeof(char*));
    char* aux = (char*)calloc(len,sizeof(char));
    int mpos = 1;
    int start = 0;
    int n_str = 0;
    args[0] = (char*)calloc(1,sizeof(char));
    int idx = 0;
    for(int i = 0; i < len - 1; i ++)
    {
        if(unstriped_str[i] != '\0' && unstriped_str[i] != '\n' && unstriped_str[i] != delim)
        {
            aux[idx] = unstriped_str[i];
            idx++;
        }
        else
        {
            args[mpos] = (char*)calloc(strlen(aux) + 1,sizeof(char));
            strcpy(args[mpos],aux);
            args[mpos][strlen(aux)] = '\0';
            // if(DEBUG)
            // {
            //     printf("String resultante: %s", args[mpos]);
            // }
            memset(aux,0,strlen(aux));
            mpos ++;
            idx = 0;
            n_str ++;
        }
        
    }

    args[0][0] = (n_str) + '0';

    return args;
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

void print_error()
{
    printf("%s",ERROR_MSG);
}

void print_ddreg(DATA_DREG* ddreg)
{
    printf("\'removido\' no registro: %c\n",ddreg->removido);
    printf("\'tamanhORegistro\' no registro: %i\n",ddreg->tamReg);
    printf("\'idPessoa\' no registro: %i\n",ddreg->idPessoa);
    printf("\'idadePessoa\' no registro: %i\n",ddreg->idadePessoa);
    printf("\'tamanhoNomePessoa\' no registro: %i\n",ddreg->tamNomePessoa);
    printf("\'nomePessoa\' no registro: %s\n",ddreg->nomePessoa);
    printf("\'tamanhoNomeUsuário\' no registro: %i\n",ddreg->tamNomeUsuário);
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

/*
    Fecha arquivos, atualizando o campo 'status' no registro de cabeçalho
*/
void close_file(FILE* file)
{
    fseek(file,0,0);            // Posiciona o ponteiro do arquivo na posição inicial (byte offset 0)

    char status = '1';

    fwrite(&status,1,1,file);   // Atualiza o campo 'status' para '1', indicando que já foi manipulado e fechado

    fclose(file);               // Fecha o arquivo
}