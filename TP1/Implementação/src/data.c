/*
                                TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                XXXXXXXX
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
    dhreg->status = '1';                                        // Seta o campo 'status' como '1' por padrão (1 byte)
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
    ddreg->idadePessoa = -1;                                     // Campo idade pessoa (4 bytes), iniciado em -1                                   
    ddreg->idPessoa = -1;                                        // Campo Id pessoa (4 bytes), iniciado em -1
    ddreg->removido = '0';                                      // Campo removido (1 byte), representa a remoção lógica do registro
    ddreg->tamNomePessoa = 0;                                   // Campo Tamanho do nomePessoa (4 bytes), iniciado em 0
    ddreg->nomePessoa = NULL;                                   // Campo nomePessoa, iniciado em NULL (tamanho variável)
    ddreg->tamNomeUsuario = 0;                                  // Campo tamNomeUsuario, iniciado em 0 (4 bytes)
    ddreg->nomeUsuario = NULL;                                  // Campo nomeUsuario, iniciado em NULL (tamanho variável)
    ddreg->tamReg = 0;                                          // Tamanho do registro, iniciado em 0 (4 bytes)

    return ddreg;                                               // Retorna o registro de dados em memória primária
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
    
    long int old_pos = ftell(file);             // Armazena a posição atual do  ponteiro em 'old_pos'
    
    fseek(file,0,SEEK_SET);                     // Posiciona o ponteiro 'file' no começo do arquivo
    
    fwrite(&d_hreg->status,1,1,file);           // Atualiza o campo 'satus' (1 byte)
    
    fwrite(&d_hreg->qtdPessoas,4,1,file);       // Atualiza o campo 'qtdPessoas' (4 bytes)
    
    fwrite(&d_hreg->qtdRemovidos,4,1,file);     // Atualiza o campo 'qtdRemovidos' (4 bytes)
    
    fwrite(&d_hreg->proxByteOffset,8,1,file);   // Atualiza o campo 'proxByteOffset (8 bytes)
    
    fseek(file,old_pos,SEEK_SET);               // Retorna à posição original de 'file' em 'oldf'
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

    char* src_str = (char*)calloc(100,sizeof(char));                                // Aloca 100 bytes de memória para ler do arquivo de fonte

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
        
        if(data[2] != NULL)
        {
            data_reg->tamNomePessoa = strlen(data[2]);
            data_reg->nomePessoa = (char*)calloc(data_reg->tamNomePessoa + 1,sizeof(char));
            strcpy(data_reg->nomePessoa,data[2]);
        }
        else{data_reg->tamNomePessoa = 0;}

        data_reg->idadePessoa = atoi(data[3]);                                      // Lê o idadePessoa e atribui ao nó

        if(data[4] != NULL)
        {
            data_reg->tamNomeUsuario = strlen(data[4]) - 1;
            data_reg->nomeUsuario = (char*)calloc(data_reg->tamNomeUsuario + 1,sizeof(char));
            strcpy(data_reg->nomeUsuario,data[4]);
        }
        else{data_reg->tamNomeUsuario = 0;}

        data_reg->tamReg = 21 + data_reg->tamNomePessoa + data_reg->tamNomeUsuario; // Calcula o tamanho do registro (21 bytes fixos + tamanho dos campos variáveis)

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
        
        if(DEBUG)
        {
            printf("\nEm memória primária:\n\n");
            print_ddreg(data_list->tail);
        }
    }
}

/*
    Escreve os itens da lista no arquivo de daados

    params:
        DATA_LIST* dlist => Lista onde estão presentes os dados a serem escritos
        FILE* file => Arquivo onde os dados devem ser escritos
    
    return:
        void
*/
void write_on_data_file(DATA_LIST* dlist, FILE* file)
{
    if(file == NULL)
    {
        print_error();
        if(DEBUG)
        {
            printf("Arquivo == NULL ao tentar escrever");
        }
        return;
    }

    fseek(file,DF_HEAD_REG_LEN,SEEK_SET);

    DATA_DREG* d_reg;
    d_reg = dlist->head;

    while (d_reg!=NULL)
    {   
        if(DEBUG){printf("Inserindo: \n");print_ddreg(d_reg);}
        fwrite(&d_reg->removido,1,1,file);

        fwrite(&d_reg->tamReg,4,1,file);

        fwrite(&d_reg->idPessoa,4,1,file);

        fwrite(&d_reg->tamNomePessoa,4,1,file);
        fwrite(&d_reg->nomePessoa,1,d_reg->tamNomePessoa,file);

        fwrite(&d_reg->tamNomeUsuario,4,1,file);
        fwrite(&d_reg->nomeUsuario,1,d_reg->tamNomeUsuario,file);

        dlist->header_reg->qtdPessoas++;

        d_reg = d_reg->next;
        dlist->header_reg->proxByteOffset = ftell(file);
    }
}

/*
    Preenche arquivo de dados

    params:
        const char* dest_filename => Arquivo de destino, o qual deseja-se preencher
        const char* src_filename => Arquivo donte, do qual os dados seram lidos
    
    return:
        void
*/
void fill_data_file(const char* dest_filename, const char* src_filename)
{   
    char* src_path = get_file_path(src_filename);                               // Pega o caminho correto para o arquivo font
    char* dest_path = get_file_path(dest_filename);                             // Pega o caminho correto para o arquivo destino

    FILE* src_file  = fopen(src_path,"r");                                      // Abre o arquivo de fonte para leitura
    if(src_file == NULL)                                                        // Verifica se abertura ocorreu corretamente
    {
        print_error();
        if (DEBUG)
        {
            printf("Erro ao abir csv ! -> Caminho relativo : %s\n",src_path);
        }
        return;
    }

    FILE* dest_file = fopen(dest_path,"ab");                                    // Abre o arquivo destino para 'append binary'
    if(dest_file == NULL)                                                       // Verifica se o arquivo existe no caminho especificado
    {
        dest_file = create_data_file(dest_filename);                            // Cria o arquivo se necessário  
        if(dest_file == NULL && DEBUG)
        {
            printf("Erro ao criar arquivo!\n");
            return;
        }
    }

    free(src_path);                                                             // Libera o espaço das strings dos caminhos
    free(dest_path);

    DATA_LIST* dlist = create_data_list();                                      // Cria uma lista de dados do arquivo de dados
    DATA_HREG* d_hreg = create_data_hreg();                                     // Cria um registro de cabeçalho em memória primária
    d_hreg->status = '0';                                                       // Indica que o arquivo de dados está sendo manipulado

    update_dheader_reg(d_hreg,dest_file);                                       // Atualiza o registro de cabeçalho com status '0'

    dlist->header_reg = d_hreg;                                                 // Linka o registro de dados à lista

    load_csvfile_to_mem(src_file,dlist);                                           // Carrega do arquivo fonte para a memória primária
    
    write_on_data_file(dlist,dest_file);

    d_hreg->status = '1';

    printf("DIRETO DO D_HREG :: status: %c; qtdpessoas:%i; proxbtyte offset: %li\n",d_hreg->status,d_hreg->qtdPessoas,d_hreg->proxByteOffset);
    printf("DIRETO DA LISTA :: status: %c; qtdpessoas:%i; proxbtyte offset: %li\n",dlist->header_reg->status,dlist->header_reg->qtdPessoas,dlist->header_reg->proxByteOffset);

    update_dheader_reg(d_hreg,dest_file);

    fclose(src_file);
    fclose(dest_file);                                                      // Fecha arquivo de destino
}
