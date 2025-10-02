#include "all.h"


int main()
{   

    char func_str [OPT_LEN];                    // Variável que armazena as entradas da funcionalidade

    while (1)
    {
        // print_menu();                           // Exibe o menu

        strcpy(func_str,"");                    // Inicializa como uma string vazia
        fgets(func_str,OPT_LEN,stdin);          // Lê as entradas do teclado, todas de uma vez       
        
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
                    break;
                }

                else
                {
                    CREATE_INDEX(args[2]);
                    break;
                }
            }
        
        case 2:
            {
                printf("\nFuncionalidade 2\n");
                CREATE_TABLE("dados.csv","saida.bin","index.bin");
                break;
            }
        
        case 3:
            {
                printf("\nFuncionalidade 3\n");
                break;
            }

        case 4:
            {
                printf("\nFuncionalidade 4\n");
                break;
            }
        
        case -1:
            {
                printf("\nSaindo...\n");
                exit(0);
            }
            
        default:
            {
                printf("\nSelecione uma opção  válida !\n");
                break;
            }
        }
    }
    

    return 0;
}