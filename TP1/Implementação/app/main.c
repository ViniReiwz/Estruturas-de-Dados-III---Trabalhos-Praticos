/*
                                TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                XXXXXXXX
                                Vinicius Reis Gonçalves             15491921
*/

//Arquivo 'index.c', contém funções que trabalham com o arquivo de índice primário

#include "all.h"

int main()
{   

    char func_str [OPT_LEN];                // Variável que armazena as entradas da funcionalidade

    strcpy(func_str,"");                    // Inicializa como uma string vazia
    fgets(func_str,OPT_LEN,stdin);          // Lê as entradas do teclado, todas de uma vez 

    int rmn = strcspn(func_str,"\n");       // Verifica a primeira ocorrência de\n na string (último caracter por conta do fgets) 
    func_str[rmn] = '\0';                   // Substitui o '\n' por '\0'
    printf("%s",func_str);
    char **args = strip_by_delim(func_str,' '); // Variável que recebe os argumentos separados

    int opt = atoi(args[1]);                // Transforma a opção de char para int
    
    switch (opt)
    {
    case 1:
        {
            if(atoi(args[0]) < 2)
            {
                if(DEBUG)
                {
                    printf("Poucos argumentos!!\n");
                }
            }

            else
            {
                CREATE_INDEX(args[2]);
            }
            break;
        }
    
    case 2:
        {
            if(atoi(args[0]) < 4)
            {
                if(DEBUG)
                {
                    printf("Poucos argumentos!!\n");
                }
            }
            else
            {
                CREATE_TABLE(args[2],args[3],args[4]);
            }
            break;
        }
        
    
    case 3:
        {
            break;
        }

    case 4:
        {
            break;
        }
        
    default:
        {
            break;
        }
    }
    return 0;
}