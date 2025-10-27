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
    follow_dreg->dataInicioQueSegue = (char*)calloc(DATE_LEN + 1,sizeof(char)); // Aloca 10 bytes para ambas as strings que representam os campos de data
    follow_dreg->dataFimQueSegue = (char*)calloc(DATE_LEN + 1,sizeof(char));    // Ver 'DATE_LEN' em include/follow.h para alterar o valor

    for(int i = 0; i < DATE_LEN; i ++)                                      // Inicializa ambas as strings com o valor '$' em cada caracter -> Representa lixo de memória / null
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
    follow_array->follow_hreg = (FOLLOW_HREG*)calloc(1,sizeof(FOLLOW_HREG));
    
    for(int i = 0; i < len; i++)
    {
        follow_array->follow_arr[i] = *create_follow_dreg();
    }
    
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
        free(follow_array->follow_arr[i].dataInicioQueSegue);
        free(follow_array->follow_arr[i].dataFimQueSegue);
    }
    free(follow_array->follow_arr);                         // Libera a memória do vetor em si
    free(follow_array->follow_hreg);
    free(follow_array);
}

/*
    Lê um arquivo binário do tipo 'segue' e coloca seus dados em memória primária

    params:
        FILE* follow_file => Ponteiro para o arquivo do tipo 'segue';
    
    return:
        FOLLOW ARR* f_arr => Estrutura de dados contendo o arquivo 'segue' inteiro e memória primária, com o número de registros de dados, o registro de cabeçalho e o registro de dados propriamente ditos, sendo estes organizados em um vetor.
*/
FOLLOW_ARR* read_follow_file(FILE* follow_file)
{
    fseek(follow_file,1,SEEK_SET);                                          // Pula o campo 'status' para leitura

    int len = 0;
    fread(&len,4,1,follow_file);                                            // Lê o número de registros de dados no arquivo (campo 'qtdPessoas', 4 bytes)

    FOLLOW_ARR* f_arr = create_follow_arr(len);                             // Aloca o espaço necessária para guardar  arquivo em memóira primária
    FOLLOW_HREG* f_hreg = f_arr->follow_hreg;                               // f_hreg = f_arr->follow_hreg para simplificar a sintaxe
    FOLLOW_DREG* f_dregvec = f_arr->follow_arr;                             // f_dregvec = f_arr->follow_arr para simplificação de sintaxe

    f_hreg->qtdPessoas  = len;                                              // Guarda o campo 'qtdPessoas' na estrutura de dados
    fread(&f_hreg->proxRRN,4,1,follow_file);                                // Lê o próximo registro disponível para inserção e o guarda em memória primária
    int file_size = FOLLOW_DATA_REG_LEN * f_hreg->qtdPessoas;               // Variável que guarda o tamanho total do arquivo (descontando o registro de cabeçalho)
    int file_pos = 0;                                                       // Variável auxiliar para detectar quando o arquivo fora completamente lido
    int i = 0;                                                              // Variável auxiliar para indicar a posição do vetor de registros em memória primária

    while (file_pos<file_size)                                              // Lê todo o arquivo
    {
        char removido;
        fread(&removido,1,1,follow_file);                                   // Verifica se o registro não foi removido
        if(removido == '0')                                                 // Caso não tenha sido removido, lê o registro completo
        {
            f_dregvec[i].removido = removido;                               // Guarda o campo 'removido' (1 byte)
            fread(&f_dregvec[i].idPessoaQueSegue,4,1,follow_file);          // Guarda o campo 'idPessoaQueSegue' (4 bytes)
            fread(&f_dregvec[i].idPessoaQueESeguida,4,1,follow_file);       // Guarda o campo 'idPessoaQueESeguida' (4 bytes)
            fread(f_dregvec[i].dataInicioQueSegue,10,1,follow_file);        // Guarda o campo 'dataInicioQueSegue' (10 bytes)
            fread(f_dregvec[i].dataFimQueSegue,10,1,follow_file);           // Guarda o campo 'dataFimQueSegue' (10 bytes)
            fread(&f_dregvec[i].grauAmizade,1,1,follow_file);               // Guarda o campo 'grauAmizade' (1 byte)
            
            f_dregvec[i].dataInicioQueSegue[10] = '\0';


            f_dregvec[i].dataFimQueSegue[10] = '\0';

            if(DEBUG)                                                       // Mensage de DEBUG que exibe o registro
            {
                print_follow_dreg(f_dregvec[i]);
            }

            i++;                                                            // Incrementa a posição do vetor
        }
        else                                                                // Caso o registro tenha qualquer valor diferente de '0' em 'removido', pula ele na leitura
        {
            fseek(follow_file,FOLLOW_DATA_REG_LEN,SEEK_CUR);
        }
        file_pos += FOLLOW_DATA_REG_LEN;                                    // Incrementa na variável auxiliar de posição
    }
    
    return f_arr;                                                           // Retorna o arquivo em memória primária
}

/*
    Escreve os dados em um arquivo binário do tipo 'segue'

    params:
        FILE* follow_file => Ponteiro para o arquivo a ser escrito;
        FOLLOW_ARR* f_arr => Dados do arquivo em memória primária;
    
    return:
        void.
*/
void write_on_follow_file(FILE* follow_file,FOLLOW_ARR* f_arr)
{

    FOLLOW_HREG* f_hreg = f_arr->follow_hreg;                           // Simplificação de sintaxe
    FOLLOW_DREG* f_dregvec = f_arr->follow_arr;                         // Simplificação de sintaxe

    fseek(follow_file,0,SEEK_SET);                                      // Posiciona o ponteiro para o inicio do arquivo

    char status = '0';                                                  // Inicializa o arquivo com status '0' (inconsistente)
    fwrite(&status,1,1,follow_file);

    fwrite(&f_hreg->qtdPessoas,4,1,follow_file);                        // Escreve o campo 'qtdPessoas' (4 bytes)
    fwrite(&f_hreg->proxRRN,4,1,follow_file);                           // Escreve o campo 'proxRRN' (4 bytes)

    int file_size = FOLLOW_DATA_REG_LEN * f_hreg->qtdPessoas;           // Variável que guarda o tamanho do arquivo descontando o registro de cabeçalho
    int file_pos = 0;                                                   // Variável auxiliar para detectar o fim do arquivo
    int i = 0;                                                          // Variável que percorre as posições do vetor de registros

    while (file_pos < file_size)                                        // Atua enquanto o arquivo nao terminar
    {
        fwrite(&f_dregvec[i].removido,1,1,follow_file);                 // Escreve o campo 'removido' (1 byte)
        fwrite(&f_dregvec[i].idPessoaQueSegue,4,1,follow_file);         // Escreve o campo 'idPessoaQueSegue' (4 bytes)
        fwrite(&f_dregvec[i].idPessoaQueESeguida,4,1,follow_file);      // Escreve o campo 'idPessoaQueESeguida' (4 bytes)
        fwrite(f_dregvec[i].dataInicioQueSegue,10,1,follow_file);       // Escreve o campo 'dataInicioQUeSegue' (10 bytes)
        fwrite(f_dregvec[i].dataFimQueSegue,10,1,follow_file);          // Escreve o campo 'datFimQueSegue' (10 bytes)
        fwrite(&f_dregvec[i].grauAmizade,1,1,follow_file);              // Escreve o campo 'grauAmizade' (1 byte)

        i++;                                                            // Incrementa a posição do vetor
        file_pos += FOLLOW_DATA_REG_LEN;                                // Incrementa a posição do ponteiro do arquivo
    }
}

/*
    Função que compara dois registros de dados do arquivo 'segue', em função dos ids e, se necessário, das datas

    params:
        FOLLOW_DREG a,b => Registros a serem comparados;
    
    return:
        int comparison => Valor menor ou maior que 0, que representa a 'posição' na ordenação que 'a' está em relação à 'b'.
*/
int compare_follow_dreg(const void* a, const void* b)
{

    const FOLLOW_DREG* ca = (const FOLLOW_DREG *)a;                                     // Faz o cast do ponteiro para o tipo de dados desejado
    const FOLLOW_DREG* cb = (const FOLLOW_DREG *)b;

    if(ca->idPessoaQueSegue == cb->idPessoaQueSegue)                                    // Verifica se o campo 'idPessoaQueSegue' é igual em ambos os registros
    {
        if(ca->idPessoaQueESeguida == cb->idPessoaQueESeguida)                          // Verifica se o campo 'idPessoaQueESeguida' é igual em ambos os registros
        {   
            char* reversed_dataInicio_a= reverse_date_string(ca->dataInicioQueSegue);   // Inverte a string 'dataInicioQueSegue' para comparar mais facilmente
            char* reversed_dataInicio_b= reverse_date_string(cb->dataInicioQueSegue);

            int comparison = strcmp(reversed_dataInicio_a,reversed_dataInicio_b);       // Valor do strcmp

            if(comparison == 0)                                                         // Verifica se ambas as strings anteriores são iguais
            {
                return strcmp(ca->dataFimQueSegue,cb->dataFimQueSegue);                 // Ordena por 'dataFimQueSegue'
            }

            return comparison;                                                          // Ordena por 'dataInicioQueSegue'
        }

        return ca->idPessoaQueESeguida - cb->idPessoaQueESeguida;                       // Ordena por 'idPessoaQueESeguida'
    }
    return ca->idPessoaQueSegue - cb->idPessoaQueSegue;                                 // Ordena por 'idPessoaQueSegue'
}

/*
    Ordena um vetor de registros do arquivo 'segue', seguindo as especificações do trabalho prático:
        idPessoaQueSegue -> idPessoaQueESeguido -> dataInicioSegue -> dataFimSegue

    params:
        FOLLOW_ARR* f_arr => Vetor à ser ordenado
*/
void ordenate_follow_dreg(FOLLOW_ARR* f_arr)
{
    qsort(f_arr->follow_arr,f_arr->len,sizeof(f_arr->follow_arr[0]),compare_follow_dreg);   // Chama a função de quicksort já implementada na stdlib.h do C (Utiliza a função comapre_follow_hreg para comparar os registros)
}


/*
    Exibe o registro de dados do arquivo do tipo 'segue' formatado para a saída desejada

    params:
        FOLLOW_DREG* f_dreg => Registro a ser impresso, em memória primária;
    
    return:
        void.
*/
void printf_fdreg(FOLLOW_DREG* f_dreg)
{
    printf("Segue a pessoa de codigo: %i\n",f_dreg->idPessoaQueESeguida);   // Imprime 'idPessoaQueESeguida'
    
    char date_start[11];
    date_start[10] = '\0';
    f_dreg->dataInicioQueSegue[0] == '$' ? strcpy(date_start,"-") : strcpy(date_start,f_dreg->dataInicioQueSegue);

    char date_end[11];
    date_end[10] = '\0';
    f_dreg->dataFimQueSegue[0] == '$' ? strcpy(date_end,"-") : strcpy(date_end,f_dreg->dataFimQueSegue);
    
    char follow_why[50];                                                    // String para guardar motivo de seguir
    switch (f_dreg->grauAmizade)                                            // Varia para cada grau de amizade
    {
        case '0':
        {
            strcpy(follow_why,"celebridade");
            break;
        }
        
        case '1':
        {
            strcpy(follow_why,"amiga de minha amiga");
            break;
        }

        case '2':
        {
            strcpy(follow_why,"minha amiga");
            break;
        }

        default:
        {
            strcpy(follow_why,"-");
            break;
        }
    }

    printf("Justificativa para seguir: %s\n",follow_why);                   // Imprime a justificativa pra seguir e um caracter de 'nova linha'

    printf("Começou a seguir em: %s\n",date_start);                         // Imprime a data em que começou a seguir
    printf("Parou de seguir em: %s\n\n",date_end);                          // Imprime a data em que deixou de seguir
}

/*
    Encontra a primeira ocorrẽncia de idPessoaQueSegue no arquivo do tipo 'segueOrdenado', através de busca binária

    params:
        FOLLOW_DREG* f_dregvec => Vetor com registros do arquivo do tipo 'segue' a serem verificados
        int len => Tamanho do referido vetor
        int idPessoaQueSegue => Valor a ser buscado
    
    return:
        int occur => Indíce da primeira ocorrência
*/
int f_first_ocurr(FOLLOW_DREG* f_dregcev, int len, int idPessoaQueSegue)
{
    int start = 0; int end = len - 1;                                       // Começa no indíce 0 e termina no índice len-1
    int ocurr = -1;                                                         // Valor 'nulo' para a ocorrência

    while (start<=end)                                                      // Atua enquanto os indíces não forem coincidentes
    {  
        int half = start + (end - start)/2;                                         // Divide o vetor na metade (busca binária)

        if(f_dregcev[half].idPessoaQueSegue == idPessoaQueSegue)            // Pela ordenação do arquivo, caso encontre o valor desejado, aproxima o indíce de fim e com o começo, visando encontrar a primeira aparoçaõ do 'idPessoaQueSegue'
        {
            ocurr = half;                                                   // Valor do índice de ocorrência == half
            end = half - 1;                                                 // Aproxima end de start
        }

        else if(f_dregcev[half].idPessoaQueSegue < idPessoaQueSegue)        // Caso o valor atual seja menor que o desejado, pegua a metade à direita do vetor
        {
            start = half + 1;
        }

        else if(f_dregcev[half].idPessoaQueSegue > idPessoaQueSegue)        // Caso o vaor atual seja maior que o desejado , pega a metade à esquerda do vetor
        {
            end = half - 1;
        }
    }

    return ocurr;                                                           // Retorna o indíce com a primeira ocorrência do valor
}

/*
    Encontra a última ocorrẽncia de idPessoaQueSegue no arquivo do tipo 'segueOrdenado', através de busca binária

    params:
        FOLLOW_DREG* f_dregvec => Vetor com registros do arquivo do tipo 'segue' a serem verificados
        int len => Tamanho do referido vetor
        int idPessoaQueSegue => Valor a ser buscado
    
    return:
        int occur => Indíce da última ocorrência
*/
int f_last_ocurr(FOLLOW_DREG* f_dregcev,int len,int idPessoaQueSegue)
{
    int start = 0; int end = len - 1;                                   // Começa no indíce 0 e termina no índice len-1
    int ocurr = -1;                                                     // Valor 'nulo' para a ocorrência

    while (start<=end)                                                  // Atua enquanto os indíces não forem coincidentes
    {
        int half = start + (end - start)/2;                                     // Divide o vetor na metade (busca binária)

        if(f_dregcev[half].idPessoaQueSegue == idPessoaQueSegue)        // Pela ordenação do arquivo, caso encontre o valor desejado, aproxima o indíce de fim com o começo, visando encontrar a primeira apariçaõ do 'idPessoaQueSegue'
        {
            ocurr = half;                                               // Valor do índice de ocorrência == half
            start = half + 1;                                           // Aproxima start de end  
        }

        else if(f_dregcev[half].idPessoaQueSegue < idPessoaQueSegue)    // Caso o valor atual seja menor que o desejado, pegua a metade à direita do vetor
        {
            start = half + 1;
        }

        else if(f_dregcev[half].idPessoaQueSegue > idPessoaQueSegue)    // Caso o valor atual seja maior que o desejado, pegua a metade à esquerda do vetor
        {
            end = half - 1;
        }
    }

    return ocurr;                                                       // Retorna o índice da ultima ocorrência
}

/*
    Faz uma busca binária e retorna todos os registros que possuem 'idPessoaQueSegue'

    params:
        FOLLOW_ARR* f_arr => Arquivo do tipo 'segueOrdenado' em memória primária
        int idPessoaQueSegue => Valor a ser usado como condição na busca
    
    return:
        FOLLOW_ARR* found_f_arr => Vetor de registros que satsifazem as condições
*/
FOLLOW_ARR* follow_match_reg(FOLLOW_ARR* f_arr, int idPessoaQueSegue)
{
    int first_index = f_first_ocurr(f_arr->follow_arr,f_arr->len,idPessoaQueSegue); // Encontra o indíce da primeira e ultima ocorrência do valor 'idPessoaQueSegue'

    int last_index = f_last_ocurr(f_arr->follow_arr,f_arr->len,idPessoaQueSegue);

    if(first_index == -1 || last_index == -1)                                       // Caso não e encontre, retorna NULL
    {
        return NULL;
    }
    int idx = 0;                                                                    // Variável auxiliar para copiar os elementos do vetor principal
    FOLLOW_ARR* found_f_arr = create_follow_arr(last_index - first_index + 1);
    for(int i = first_index; i <= last_index; i++)                                  // Atua entre os indíces encontrados
    {
        found_f_arr->follow_arr[idx] = f_arr->follow_arr[i];
        idx++;
    }

    return found_f_arr;                                                             // Retorna o vetor com os registros correspondentes à busca
}

/*
    Exibe todos os registros do arquivo de dados do tipo 'segueOrdenado' que satisfazem 'idPessoaqueSegue' == 'idPessoa'

    params:
        FOLLOW_ARR* f_arr => Arquivo do tipo 'segueOrdenado' em memória primária
        int idPessoa => Valor a ser usado como parâmetro na busca
    
    return:
        void
*/
void SELECT_WHERE_FOLLOW(FOLLOW_ARR* f_arr, int idPessoa)
{
    FOLLOW_ARR* match_f_arr = follow_match_reg(f_arr,idPessoa); // Gera o vetor com todos os registros que satisfazem a condição de busca

    if(match_f_arr==NULL)                                       // Caso for NULL, exibe a mensagem de erro e sai da função
    {printf("Registro inexistente. \n"); return;}

    for(int i = 0; i < match_f_arr->len; i++)                   // Atua por todo o vetor de registros
    {
        printf_fdreg(&match_f_arr->follow_arr[i]);              // Exibe cada registro correspondente de maneira formatada;
    }
}

/*
    Recebe o arquivo csv com as informações sobre as relações entre seguidores e passa para memória
    primária o seu conteúdo em formato de array de registros follow

    params:
        FILE* follow_csv => arquivo .csv já aberto
    
    return:
        FOLLOW_ARR* => array de registros follow
*/
FOLLOW_ARR* load_follow_csv_into_array(FILE* follow_csv)
{
    if(follow_csv == NULL)
    {
        print_error();
        exit(EXIT_FAILURE);
    }
    
    char str_in[100];
    int len = 0;
    
    fgets(str_in, 100, follow_csv); //Pula o cabeçalho

    while (fgets(str_in, 100, follow_csv) != NULL)  //Pega quantos registros o arquivo csv possui
    {
        len++;
    }

    fseek(follow_csv, 0, SEEK_SET);

    FOLLOW_ARR* follow_arr = create_follow_arr(len);    //Cria o array de follow
    
    fgets(str_in, 100, follow_csv); //Pula o cabeçalho

    for(int i = 0; i < len; i++)
    {
        fgets(str_in, 100, follow_csv);
        char** fields = strip_by_delim(str_in, ',',0);
        
        //Passa pro array campo a campo

        follow_arr->follow_arr[i].idPessoaQueSegue = atoi(fields[1]);
        follow_arr->follow_arr[i].idPessoaQueESeguida = atoi(fields[2]);
        
        end_string_on_mark(fields[5],"\n");

        strcpy(follow_arr->follow_arr[i].dataInicioQueSegue, fields[3]);

        if(strcmp(fields[4],"") != 0)    //Testa se a dataFimQueSegue é nula
        {
            strcpy(follow_arr->follow_arr[i].dataFimQueSegue, fields[4]);
        }

        if(strcmp(fields[5],"") != 0)    //Testa se o grauAmizade é nulo
        {
            follow_arr->follow_arr[i].grauAmizade = fields[5][0];
        }

        destroy_strip_matrix(fields);
    }
    
    follow_arr->follow_hreg->proxRRN = 9 + 30*len;
    follow_arr->follow_hreg->qtdPessoas = len;
    printf("len -> %i\n",len);
    printf("proxrrn -> %i\n",follow_arr->follow_hreg->proxRRN);

    return follow_arr;
}