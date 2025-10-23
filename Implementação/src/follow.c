/*
                                TRABALHO PRÁTICO - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
                                Vinicius Reis Gonçalves             15491921
*/

//Arquivo 'follow.c', contém funções que trabalham com o arquivo de pessoas que seguem

#include "all.h"

/*
    Cria um registro de dados do arquivo 'segue' em memória primária

    return:
        FOLLOW_DREG* follow_dreg => Registro de dados do arquivo 'segue' em memória primária com valores inicializados
        como nulo seguindo as especificações do trabalho.

*/
FOLLOW_DREG* create_follow_dreg()
{
    FOLLOW_DREG* follow_dreg = (FOLLOW_DREG*)calloc(1,sizeof(FOLLOW_DREG)); // Aloca memória para uma variável do tipo FOLLOW_DREG
    follow_dreg->removido = '0';                                            // Inicia o campo 'removido' como '0' por padrão
    follow_dreg->idPessoaQueSegue = follow_dreg->idPessoaQueESeguida = -1;  // Inicia os campos relacionados à pessoa que segue/é seguida como -1 (valor null para inteiros)
    follow_dreg->grauAmizade = '$';                                         // Inicia o campo 'grauAmziade' como '$' (null para char)
    follow_dreg->dataInicioQueSegue = (char*)calloc(DATE_LEN,sizeof(char)); // Aloca 10 bytes para ambas as strings que representam os campos de data
    follow_dreg->dataFimQueSegue = (char*)calloc(DATE_LEN,sizeof(char));    // Ver 'DATE_LEN' EM include/follow.h para alterar o valor

    for(int i = 0; i < DATE_LEN; i ++)                                      // Inicializa ambas as strings com o valo '$' em cada caracter -> Representa lixo de memória / null
    {
        follow_dreg->dataInicioQueSegue[i] = '$';
        follow_dreg->dataFimQueSegue[i] = '$';
    }

    return follow_dreg;                                                     // Retorna o endereço de memória da região alocada
}

/*
    Cria um registro de cabeçalho do arquivo 'segue' em memória primária

    return:
        FOLLOW_HREG* follow_hreg => Registro de cabeçalho do arquivo 'segue' em memória primária com valores nulos segundo a especificação.
*/
FOLLOW_HREG* create_follow_hreg()
{
    FOLLOW_HREG* follow_hreg = (FOLLOW_HREG*)calloc(1,sizeof(FOLLOW_HREG)); // Aloca memória de uma variável do tipo FOLLOW_HREG
    follow_hreg->qtdPessoas = follow_hreg->proxRRN = -1;                    // Inicializa os valores do reegistro de cabeçalho como nulos
    return follow_hreg;                                                     // Retorna o endereço da região de memória alocada
}

/*
    Cria um vetor de registros de dados do arquivo 'segue'

    params:
        int len => Tamanho do vetor à ser criado;

    return:
        FOLLOW_ARR* follow_array => Endereço da região de memória alocada com o tamanho e o vetor deregistros inicialmente vazios.
*/
FOLLOW_ARR* create_follow_arr(int len)
{
    FOLLOW_ARR* follow_array = (FOLLOW_ARR*)calloc(1,sizeof(FOLLOW_ARR));       // Aloca a memória para uma variável do tipo FOLLO_ARR
    follow_array->len = len;                                                    // Atribui o tamanho à estrutura de dados
    follow_array->follow_arr = (FOLLOW_DREG*)calloc(len,sizeof(FOLLOW_DREG));   // Aloca a memória de um vetor com 'len' poisções que armazenam variáveis do tipo FOLLOW_DREG
    return follow_array;                                                        // Retorna o endereço de memória da região alocada

}

/*
    Libera a memória alocada para uma variável do tipo FOLLOW_DREG*

    params:
        FOLLOW_DREG* follow_dreg => Registro à ser destruído (memória liberada);
    
    return:
        void.
*/
void destroy_follow_dreg(FOLLOW_DREG* follow_dreg)
{
    free(follow_dreg->dataInicioQueSegue);  // Libera a memória de cada string que representam os campos de data (alocadas dinamicamente)
    free(follow_dreg->dataFimQueSegue);
    free(follow_dreg);
}

/*
    Libera a memória alocada de uma variável do tipo FOLLOW_HREG*

    params:
        FOLLOW_HREG* follow_hreg => Registro de cabeçalho a ter memória liberada;
    
    return:
        void.
*/
void destroy_follow_hreg(FOLLOW_HREG* follow_hreg)
{
    free(follow_hreg);
}

/*
    Libera a memória de uma variável do tipo FOLLOW_ARR*

    params:
        FOLLOW_ARR* follow_array => Vetor de registros de dados cuja memória quer ser liberada;
    
    return:
        void.
*/
void destroy_follow_array(FOLLOW_ARR* follow_array)
{
    for(int i = 0; i < follow_array->len; i++)              // Libera a memória para cada um dos registros de dados presentes no vetor
    {
        destroy_follow_dreg(&follow_array->follow_arr[i]);
    }
    free(follow_array->follow_arr);                         // Libera a memória do vetor em si
    free(follow_array);
}
