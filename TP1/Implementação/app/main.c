/*
                                TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
                                Vinicius Reis Gonçalves             15491921
*/

//Arquivo 'main.c', arquivo principal que executa as funcionalidades selecionadas pelo usuário

#include "all.h"

int main()
{   

    char func_str [OPT_LEN];                // Variável que armazena as entradas da funcionalidade

    strcpy(func_str,"");                    // Inicializa como uma string vazia
    fgets(func_str,OPT_LEN,stdin);          // Lê as entradas do teclado, todas de uma vez 

    char **args = strip_by_delim(func_str,' '); // Variável que recebe os argumentos separados

    int n_args = atoi(args[0]);             // Pega o número de argumentos passados ao programa

    int opt = atoi(args[1]);                // Transforma a opção de char para int
    
    switch (opt)                            // Switch case para identificar de qual funcionalidade se trata
    {
        case 1:                             // Funcionalidade 1:
            {
                if(n_args < 2)                  // Verifica se o nome do arquivo de índice foi passado
                {
                    if(DEBUG)
                    {
                        printf("Poucos argumentos!!\n");
                    }
                }

                else
                {
                    CREATE_INDEX(args[2]);      // Executa a funcionalidade 1
                }
                break;
            }
        
        case 2:                             // Funcionalidade 2
            {
                if(n_args < 4)                  // Verifica se foram passados os nome do arquivo fonte, arquivo de dados a ser criado e arquivo de índice (já existente)           
                {
                    if(DEBUG)
                    {
                        printf("Poucos argumentos!!\n");
                    }
                }
                else
                {
                    CREATE_TABLE(args[2],args[3],args[4]);  // Executa a funcionalidade 2
                }
                break;
            }
            
        
        case 3:                             // Funcionalidade 3
            {
                if(atoi(args[0]) < 2)                       // Verifica se o nome do arquivo de dados foi passado
                {
                    if(DEBUG)
                    {
                        printf("Poucos argumentos!!\n");
                    }
                }

                else
                {
                    SELECT_FROM_TABLE(args[2]);             // Executa a funcionalidade 3
                }

                break;
            }

        case 4:                             // Funcionalidade 4
            {
                if(atoi(args[0]) < 4)                       // Verifica se foram passados: Nome d arquivo de dados, nome do arquivo de índice e número de buscas a serem feitas
                {
                    if(DEBUG)
                    {
                        printf("Poucos argumentos!!\n");
                    }
                }
                else
                {
                    SELECT_FROM_WHERE(args[2],args[3],atoi(args[4]));   // Executa a funcionalidade 4
                }
                
                break;
            }
            
        default:
            {
                break;                      // Caso não seja uma opção valida, apenas sai do programa
            }
    }

    destroy_strip_matrix(args);             // Libera a memória da matriz de strings
    return 0;                               // Encerra o programa
}