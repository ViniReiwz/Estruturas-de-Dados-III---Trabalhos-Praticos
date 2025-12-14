/*
                                TRABALHO PRÁTICO - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
                                Vinicius Reis Gonçalves             15491921
*/

//Arquivo 'index.c', contém funções que trabalham com o arquivo de índice primário

#include "all.h"

int main()
{   

    char func_str [OPT_LEN];                // Variável que armazena as entradas da funcionalidade

    strcpy(func_str,"");                    // Inicializa como uma string vazia
    fgets(func_str,OPT_LEN,stdin);          // Lê as entradas do teclado, todas de uma vez 

    char **args = strip_by_delim(func_str,' ',0); // Variável que recebe os argumentos separados

    int n_args = atoi(args[0]);             // Pega o número de argumentos passados ao programa

    int opt = atoi(args[1]);                // Transforma a opção de char para int
    
    switch (opt)
    {
    case 1:
        {
            if(n_args < 2)
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
            if(n_args < 4)
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
            if(atoi(args[0]) < 2)
            {
                if(DEBUG)
                {
                    printf("Poucos argumentos!!\n");
                }
            }

            else
            {
                SELECT_FROM_TABLE(args[2]);
            }

            break;
        }

    case 4:
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
                SELECT_FROM_WHERE(args[2],args[3],atoi(args[4]));
            }
            
            break;
        }
    
    case 5:
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
                DELETE_FROM_WHERE(args[2],args[3],atoi(args[4]));
            }
            
            break;
        }
    case 6:
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
                INSERT_INTO(args[2],args[3],atoi(args[4]));
            }
            
            break;
        }
    case 7:
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
                UPDATE_SET_WHERE(args[2],args[3],atoi(args[4]));
            }
            
            break;
        }
    case 8:
          {
            if(atoi(args[0]) < 3)
            {
                if(DEBUG)
                {
                    printf("Poucos argumentos!!\n");
                }
            }
            else
            {

                CREATE_FOLLOW_TABLE(args[2],args[3]);
            }
            
            break;
          }
      case 9:
        {
          if(atoi(args[0]) < 3)
          {
            if(DEBUG)
            {
              printf("Poucos argumentos!!\n");
            }
          }
          else
          {
            ORDER_BY(args[2],args[3]);
          }
          break;
        }
    
        case 10:
        {
            if(atoi(args[0]) < 5)
            {
                if(DEBUG)
                {
                    printf("Poucos argumentos!!\n");
                }
            }
            else
            {
                SELECT_FROM_JOIN_ON(args[2],args[3],args[4],atoi(args[5]));
            }
            break;
        }

        case 11:
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
                GENERATE_GRAPH(args[2],args[3],args[4]);
            }
            break;
        }
    
        case 12:
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
                GENERATE_TRANSPOSED(args[2],args[3],args[4]);
            }
            break;
        }
        
    default:
        {   
            break;
        }
    }

    destroy_strip_matrix(args);
    return 0;
}
