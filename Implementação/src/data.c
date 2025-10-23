/*
                                TRABALHO PRÁTICO - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
                                Vinicius Reis Gonçalves             15491921
*/

//Arquivo 'data.c', contém funções que trabalham com o arquivo de dados

#include "all.h"

/*
    Cria um arquivo de dados e seu registro de cabeçalho

    params:
        const char* filename => Nome_do_arquivo.extensão (arquivo.bin, por exemplo)

    return:
        FILE* dlist => Ponteiro para a lista de dados emmemória primária
*/
DATA_LIST* create_data_list()
{
    DATA_LIST* dlist = (DATA_LIST*)calloc(1,sizeof(DATA_LIST)); // Aloca espaço na memória para a lista de dados
    dlist->head = dlist->tail = NULL;                           // Inicializa os ponteiros como NULL
    return dlist;                                               // Retorna o endereço para a região de memória alocada
}

/*
    Cria um nó que representa um registro de cabeçalho do arquivo de dados em memória primária, de tamanho total 17 bytes

    return:
        DATA_HREG* dhreg => Retorna um endereço para uma variável do tipo 'registro de cabeçalho' do arquivo de dados
*/
DATA_HREG* create_data_hreg()
{
    DATA_HREG* dhreg = (DATA_HREG*)calloc(1,sizeof(DATA_HREG)); // Aloca o tamanho de um nó em memória heap
    dhreg->qtdPessoas = 0;                                      // Quantidade de pessoas no arquivo de dados, iniciada em 0 (4 bytes)
    dhreg->qtdRemovidos = 0;                                    // Quantidade de registros logicamente removidos, iniciada em 0 (4 bytes)
    dhreg->proxByteOffset = 0;                                  // Determina o próximo byte offset disponível para inserção (8 bytes)

    return dhreg;                                               // Retorna o registro de cabeçalho
}

/*
    Cria um nó que representa um registro de dados no arquivo de dados

    return:
        DATA_DREG* ddreg => Retorna um endereço para uma variável do tipo 'registro de dados' do arquivo de dados
*/
DATA_DREG* create_data_dreg()
{
    DATA_DREG* ddreg = (DATA_DREG*)calloc(1,sizeof(DATA_DREG)); // Aloca o espaço para um elemento na memŕoia heap
    ddreg->idadePessoa = -1;                                    // Campo idade pessoa (4 bytes), iniciado em -1                                   
    ddreg->idPessoa = -1;                                       // Campo Id pessoa (4 bytes), iniciado em -1
    ddreg->removido = '0';                                      // Campo removido (1 byte), representa a remoção lógica do registro
    ddreg->tamNomePessoa = -1;                                   // Campo Tamanho do nomePessoa (4 bytes), iniciado em 0
    ddreg->nomePessoa = NULL;                                   // Campo nomePessoa, iniciado em NULL (tamanho variável)
    ddreg->tamNomeUsuario = -1;                                  // Campo tamNomeUsuario, iniciado em 0 (4 bytes)
    ddreg->nomeUsuario = NULL;                                  // Campo nomeUsuario, iniciado em NULL (tamanho variável)
    ddreg->tamReg = -1;                                         // Tamanho do registro, iniciado em 0 (4 bytes)

    return ddreg;                                               // Retorna o registro de dados em memória primária
}

/*
    Libera a memória de um registro de dados do arquivo de dados em memória primária

    params:
        DATA_DREG* ddreg => Registro a ser liberado
    
    return:
        void
*/
void destroy_data_dreg(DATA_DREG* ddreg)
{
    free(ddreg->nomePessoa);    // Libera memória do campo 'nomePessoa'
    free(ddreg->nomeUsuario);   // Libera memória do campo 'nomeUsuario'
    free(ddreg);
}

/*
    Libera a memória de um registro de cabeçalho do arquivo de dados em memória primária

    params:
        DATA_HREG* dhreg => Registro de cabeçalho cuja memória deve ser liberada
*/
void destroy_data_hreg(DATA_HREG* dhreg)
{
    free(dhreg);
}

/*
    Destrói uma lista de registros de dados do arquivo de dados em memória primária

    params:
        DATA_LIST* dlist => Lista cuja memória deve ser liberada

    return:
        void
*/
void destroy_data_list(DATA_LIST* dlist)
{
    destroy_data_hreg(dlist->header_reg);   // Libera a memória para o registro de cabeçalho
    while (dlist->head!=NULL)               // Percorre toda a lista
    {
        DATA_DREG* p = dlist->head->next;   // Armazena o endereço do próximo nó
        destroy_data_dreg(dlist->head);     // Libera a memória para o nó atual
        dlist->head = p;                    // Move a head para o próximo nó
    }       

    free(dlist);                            // Libera espaço da estrutura em si
}

/*
Atualiza o registro de cabeçalho do arquivo de dados

params:
DATA_HREG* d_hreg => Registro de cabeçalho em memória primária, contendo os dados corretos para atualizar
FILE* file => Ponteiro para o arquivo ao qual deseja atualizar o registro de cabeçalho

return:
FILE* old => Posição original do ponteiro 'file'
*/
void  update_dheader_reg(DATA_HREG* d_hreg, FILE* file)
{
    if(file == NULL)                                
    {
        printf("%s",ERROR_MSG);                 // Verifica se o arquivo existe e exibe uma mensagem de erro caso contrário
        if(DEBUG)
        {
            printf("Arquivo == NULL ao tetar atualizar registro de cabeçalho");
        }
        return;
    }
    
    
    fseek(file,1,SEEK_SET);                     // Posiciona o ponteiro 'file' no começo do arquivo, pulando 'status'
    
    fwrite(&d_hreg->qtdPessoas,4,1,file);       // Atualiza o campo 'qtdPessoas' (4 bytes)
    
    fwrite(&d_hreg->qtdRemovidos,4,1,file);     // Atualiza o campo 'qtdRemovidos' (4 bytes)
    
    fwrite(&d_hreg->proxByteOffset,8,1,file);   // Atualiza o campo 'proxByteOffset (8 bytes)

}

/*
    Cria um novo arquivo de dados

    params:
        const char* filename => Nome do arquivo a ser criado
    
    return:
        FILE* data_file_p => Retorna um ponteiro para o início do arquivo de dados
*/
FILE* create_data_file(const char* filename)
{
    char* file_path = get_file_path(filename);      // Encontra o caminho relativo para o arquivo

    if(DEBUG)
    {
        printf("Caminho do arquivo de dados: %s\n",file_path);
    }

    FILE* data_file_p = fopen(file_path,"wb");      // Abre o arquivo para escrita binária

    if(data_file_p == NULL)
    {
        print_error();
        if(DEBUG)
        {
            printf("\nImpossível criar arquivo de dados !\n");  // Exibe mensagem de debug caso DEBUG == 1 em own_utils.h
        }
        return NULL;
    }

    free(file_path);                                // Libera a memória do caminho relativo do arquivo

    DATA_HREG* dhreg = create_data_hreg();          // Cria em memória primária o registro de cabeçalho
    
    update_dheader_reg(dhreg,data_file_p);          // Atualiza o registro de cabeçalho
    
    destroy_data_hreg(dhreg);                       // Libera a memória do registro de cabeçalho criado

    fseek(data_file_p,0,SEEK_SET);                  // Volta ao início do arquivo de dados

    return data_file_p;                             // Retorna o ponteiro para o arquivo
}

/*
    Carrega os dados do arquivo fonte para a memória primária

    params:
        FILE* file => Ponteiro para arquivo fonte
        DATA_LIST* data_list => Lista de dados para qual deseja carregar o conteúdo

    return:
        void
*/
void load_csvfile_to_mem(FILE* file, DATA_LIST* data_list)
{
    if(file == NULL)                                                                // Verifica se o arquivo existe no momento de carregar para a memória
    {   
        print_error();
        if(DEBUG)
        {
            printf("Arquivo == NULL ao tentar carregar para memória");
        }
        return;
    }
    
    fseek(file,0,SEEK_SET);                                                         // Posiciona o ponteiro no oinício do arquivo de src

    char src_str[100];                                // Aloca 100 bytes de memória para ler do arquivo de fonte

    fgets(src_str,100,file);                                                        // Pula o registro de cabeçalho na leitura

    while (fgets(src_str,100,file) != NULL)                                         // Lê 100 bytes do arquivo (ou até '\n') enquanto não terminar
    {   
        char** data = strip_by_delim(src_str,',');                                  // Separa as strings delimitadas por ','

        int n_data = atoi(data[0]);                                                 // Vê o número final de strings
        if(n_data < 4)                                                              // Verifica se leu todos os dados (id,nome,idade e usuário)
        {
            print_error();                                                          // Exibe mensagem de erro e sai da função caso não leia ao menos 4 dados
            return;
        }

        DATA_DREG* data_reg = create_data_dreg();                                   // Cria um registro na memória primária

        data_reg->idPessoa = atoi(data[1]);                                         // Lê o idPessoa e atribui ao nó
        
        if(strcmp(data[2],"") != 0)                                                 // Verifica se o nome é != NULL
        {
            data_reg->tamNomePessoa = strlen(data[2]);
            data_reg->nomePessoa = (char*)calloc(data_reg->tamNomePessoa + 1,sizeof(char));     // Se for, grava o tamanho, aloca a memória e copia a string
            strcpy(data_reg->nomePessoa,data[2]);
        }
        else{data_reg->tamNomePessoa = 0;}                                       // Se não, atribui o tamanho  como 0

        if(strcmp(data[3],"") != 0)
        {data_reg->idadePessoa = atoi(data[3]);}                                // Lê o idadePessoa e atribui ao nó

        if(strcmp(data[4],"") != 0)                                             // Verifica se o usuário é != NULL
        {
            data_reg->tamNomeUsuario = strlen(data[4]) -1;
            data_reg->nomeUsuario = (char*)calloc(data_reg->tamNomeUsuario + 1,sizeof(char));   // Se for, aloca a memmória e copia a string
            strcpy(data_reg->nomeUsuario,data[4]);
        }
        else{data_reg->tamNomeUsuario = 0;}                                         // Se não, mantém null e seta o tamanho como 0

        data_reg->tamReg = 16 + data_reg->tamNomePessoa + data_reg->tamNomeUsuario; // Calcula o tamanho do registro (21 bytes fixos + tamanho dos campos variáveis)

        if(data_list->head == NULL)                                                 // Verifica se a lista está vazia
        {
            data_list->head = data_list->tail = data_reg;                           // Atribui os ponteiros corretamentes ao primeiro nó
        }
        else                                                                        // Caso contrário:
        {           
            data_list->tail->next = data_reg;                                       // O último nó recebe o novo nó como próximo
            data_list->tail = data_reg;                                             // O nó cauda recebe o novo nó
        }
        
        data_list->tail->next = NULL;                                               // Atribui o próximo ao ultimo nó vomo NULL
        data_list->header_reg->qtdPessoas++;                                        // Aumenta o campo quantidade de pessoas no registro de cabeçalho
        
        if(DEBUG)                                                                   // Exibe os registros carregados em memória primária
        {
            printf("\nEm memória primária:\n\n");
            print_ddreg(data_list->tail);
        }

        destroy_strip_matrix(data);                                                 // Libera a memória da matriz com os dados do arquivo fonte
    }
    
}

/*
    Escreve os itens da lista no arquivo de daados

    params:
        DATA_LIST* dlist => Lista onde estão presentes os dados a serem escritos
        FILE* file => Arquivo onde os dados devem ser escritos
    
    return:
        INDEX_ARR* idx => Arquivo de índice *desordenado* em memória primária
*/
INDEX_ARR* write_on_data_file(DATA_LIST* dlist, FILE* file)
{   

    if(file == NULL)                                                    // Verifica se o arquivo de dados existe ou está aberto
    {
        print_error();                                                  // Exibe mensagem de erro
        if(DEBUG)
        {
            printf("Arquivo == NULL ao tentar escrever");
        }
        return NULL;
    }

    fseek(file,DF_HEAD_REG_LEN,SEEK_SET);                               // Pula o registro de cabeçalho para começar a escrever

    INDEX_ARR* idx = create_index_arr(dlist->header_reg->qtdPessoas);   // Cria um vetor com registros de dados do arquivo de índice em memória primária com tamanho igual ao número de registros no arquivo daddos
    int i = 0;                                                          // Variável auxiliar para percorrer o vetor acima                           

    long int currboffset = DF_HEAD_REG_LEN;                             // Guarda o byte offset inicial (posição 17, visto que o registro de cabeçalho começa em 0 e vai até o 16)

    DATA_DREG* d_reg = dlist->head;                                     // Cria um ponteiro de registros de dados do arquivo de dados para percorrer a lista em memória primária
    
    while (d_reg!=NULL)                                                 // Percorre toda a lista
    {     
        if(DEBUG)
        {
            printf("\n\nOFFSET PELO FTELL --> %li\n OFFSET PELA VARIAVEL --> %li\n\n\n",ftell(file),currboffset);
        }
        
        dlist->header_reg->proxByteOffset = currboffset;                // Atualiza o próximo byte offset disponível para a escrita
        idx->idx_arr[i] = indexate(d_reg->idPessoa,currboffset);        // Indexa o id com o byteoffset atual

        if(DEBUG){printf("Inserindo: \n");print_ddreg(d_reg);}          // Exibe o registro atual na lista

        fwrite(&d_reg->removido,1,1,file);                              // Salva o campo 'removido' no arquivo (char : 1 byte)

        fwrite(&d_reg->tamReg,4,1,file);                                // Salva o tamanho do registro no arquivo (int : 4 bytes)

        fwrite(&d_reg->idPessoa,4,1,file);                              // Salva 'idPessoa' no arquivo (int : 4 bytes)

        fwrite(&d_reg->idadePessoa,4,1,file);                           // Salva a 'idadePessoa' no arquivo (int : 4 bytes)

        fwrite(&d_reg->tamNomePessoa,4,1,file);                         // Salva 'tamNomePessoa' no arquivo (int : 4 bytes)
        if(d_reg->tamNomePessoa > 0)
        {fwrite(d_reg->nomePessoa,1,d_reg->tamNomePessoa,file);}        // Salva 'nomePessoa' se o tamanho for maior que 0 (variável)

        fwrite(&d_reg->tamNomeUsuario,4,1,file);                        // Salva 'tamNomeUsuario' no arquivo (int : 4 bytes)
        if(d_reg->tamNomeUsuario > 0)
        {fwrite(d_reg->nomeUsuario,1,d_reg->tamNomeUsuario,file);}      // Salva 'nomeUsuario' se o tamanho for maior que 0 (variável)

        currboffset += d_reg->tamReg + 5;                               // Incrementa o byte offset pelo tamanho do registro

        d_reg = d_reg->next;                                            // Avança na lista de registros de dados
        
        i++;                                                            // Incrementa no índice do vetor de índices
    }
    dlist->header_reg->proxByteOffset = currboffset;                    // Atualiza pra saber a última posição do ponteiro do arquivo

    return idx;                                                         // Retorna o vetor de reigstros de dados do arquivo de índice
}

/*
    Preenche arquivo de dados e cria um arquivo de índice em memória primária

    params:
        const char* dest_filename => Arquivo de destino, o qual deseja-se preencher
        const char* src_filename => Arquivo donte, do qual os dados seram lidos
    
    return:
        INDEX_ARR* idx => Vetor contendo os registros de dados do arquivo de indíce relacionado ao arquivo de dados, já ordenado
*/
INDEX_ARR* fill_data_file(FILE* src_file, FILE* dest_file)
{   

    DATA_LIST* dlist = create_data_list();                  // Cria uma lista de dados do arquivo de dados
    DATA_HREG* d_hreg = create_data_hreg();                 // Cria um registro de cabeçalho em memória primária

    dlist->header_reg = d_hreg;                             // Linka o registro de dados à lista

    load_csvfile_to_mem(src_file,dlist);                    // Carrega do arquivo fonte para a memória primária

    INDEX_ARR* idx = write_on_data_file(dlist,dest_file);   // Escreve os dados no arquivo de destino e indexa o byte offset ao id em memória primária

    order_index(idx);                                       // Ordena o arquivo de índice em memória primária
    
    if(DEBUG)                                               // Exibe os registros de índice ordenados
    {
        for(int i = 0; i < idx->len; i ++)
        {
            printf("ÍNDICE %i\n",i);
            print_index(idx->idx_arr[i]);
        }
    }

    update_dheader_reg(d_hreg,dest_file);                   // Atualiza o cabeçalho do arquivo de dados, exceto status

    destroy_data_list(dlist);                               // Libera a memória da lista primária

    return idx;                                             // Retorna ponteiro para arquivo de índice em memória primária
}

/*
    Printa um registro de dados para o usuário assim como foi pedido

    params:
        DATA_DREG data_register => registro de dados a ser printado
    
    return:
        void
*/

void print_data_register(DATA_DREG data_register)
{
    if(data_register.removido == '0')    //Testa se está removido, se não, sai da função
    {
        if(data_register.idPessoa == 0)
        {
            printf("Dados da pessoa de codigo -\n");
        }
        else
        {
            printf("Dados da pessoa de codigo %i\n", data_register.idPessoa);
        }
        
        printf("Nome: ");
        if(data_register.tamNomePessoa > 0) //Testa se há nome
        {    
            for(int i = 0; i < data_register.tamNomePessoa; i++)
            {
                if(data_register.nomeUsuario[i] != '$') //Testa se é lixo
                {
                    printf("%c", data_register.nomePessoa[i]);
                }
            }
        }
        else
        {
            printf("-");
        }
        printf("\n");

        if(data_register.idadePessoa == -1)
        {
            printf("Idade: -\n");
        }
        else
        {
            printf("Idade: %i\n", data_register.idadePessoa);
        }
        
        printf("Usuario: ");
        if(data_register.tamNomeUsuario > 0)
        {   
            for(int i = 0; i < data_register.tamNomeUsuario; i++)
            {
                if(data_register.nomeUsuario[i] != '$') //Testa se é lixo
                {
                    printf("%c", data_register.nomeUsuario[i]);
                }
            }
        }
        else
        {
            printf("-");
        }
        printf("\n\n");
    }    
}

/*
    Recebe um arquivo já aberto e puxa para a memória primária o conteúdo de um registro,

    params:
        const FILE* data_file => Arquivo fonte dos dados (já existente)
        const long byte_offset => Byte offset do registro

    return:
        DATA_DREG* data_register => registro puxado da memória

*/

DATA_DREG* pull_reg_to_memory(long byte_offset, FILE* data_file)
{
   if (data_file == NULL) // testa se o arquivo existe
    {
        print_error();
        exit(EXIT_FAILURE);
    }

    fseek(data_file, byte_offset, SEEK_SET); // posiciona o cursor do arquivo no primeiro byte do registro

    DATA_DREG *data_register = create_data_dreg(); // cria uma variável de registro

    fread(&(data_register->removido), 1, 1, data_file); // Começa a ler os parâmetros
    fread(&(data_register->tamReg), 4, 1, data_file);
    fread(&(data_register->idPessoa), 4, 1, data_file);
    fread(&(data_register->idadePessoa), 4, 1, data_file);

    fread(&(data_register->tamNomePessoa), 4, 1, data_file);
    if (data_register->tamNomePessoa > 0) // Caso a pessoa tenha nome ele será lido
    {
        data_register->nomePessoa = (char *)calloc(1, data_register->tamNomePessoa);
        fread(data_register->nomePessoa, data_register->tamNomePessoa, 1, data_file);
    }

    fread(&(data_register->tamNomeUsuario), 4, 1, data_file);
    if (data_register->tamNomeUsuario > 0) // Caso o usuário tenha nome ele será lido
    {
        data_register->nomeUsuario = (char *)calloc(1, data_register->tamNomeUsuario);
        fread(data_register->nomeUsuario, data_register->tamNomeUsuario, 1, data_file);
    }
 
    return data_register;
}

/*
    Recebe um arquivo já aberto e escreve da memória primária um registro no arquivo.

    params:
        const FILE* data_file => Arquivo fonte dos dados (já existente)
        const long byte_offset => byte offset onde deve ser escrito o registro
        DATA_DREG* data_register => registro a ser escrito

    return:
        void

*/

void push_reg_to_memory(long byte_offset, FILE* data_file, DATA_DREG *data_register)
{
   if (data_file == NULL || data_register == NULL) // testa se o arquivo e o registro existem
    {
        print_error();
        exit(EXIT_FAILURE);
    }

    fseek(data_file, byte_offset, SEEK_SET); // posiciona o cursor do arquivo no primeiro byte do registro

    fwrite(&(data_register->removido), 1, 1, data_file); // Começa a ler os parâmetros
    fwrite(&(data_register->tamReg), 4, 1, data_file);
    fwrite(&(data_register->idPessoa), 4, 1, data_file);
    fwrite(&(data_register->idadePessoa), 4, 1, data_file);

    fwrite(&(data_register->tamNomePessoa), 4, 1, data_file);
    if (data_register->tamNomePessoa > 0) // Caso a pessoa tenha nome ele será lido
    {
        fwrite(data_register->nomePessoa, data_register->tamNomePessoa, 1, data_file);
    }

    fwrite(&(data_register->tamNomeUsuario), 4, 1, data_file);
    if (data_register->tamNomeUsuario > 0) // Caso o usuário tenha nome ele será lido
    {
        fwrite(data_register->nomeUsuario, data_register->tamNomeUsuario, 1, data_file);
    }
}
