#include "all.h"

/*
    Cria um arquivo de dados e seu registro de cabeçalho

    params:
        const char* filename => Nome_do_arquivo.extensão (arquivo.bin, por exemplo)

    return:
        FILE* data_file_p => Ponteiro para o arquivo de dados
*/

DATA_HREG* create_data_hreg()
{
    DATA_HREG* dhreg = (DATA_HREG*)calloc(1,sizeof(DATA_HREG));
    dhreg->status = '1';
    dhreg->proxByteOffset = 0;
    dhreg->qtdPessoas = 0;
    dhreg->qtdRemovidos = 0;

    return dhreg;
}

DATA_DREG* create_data_dreg()
{
    DATA_DREG* ddreg = (DATA_DREG*)calloc(1,sizeof(DATA_DREG));
    ddreg->idadePessoa=0;
    ddreg->idPessoa=0;
    ddreg->removido = '0';
    ddreg->tamNomePessoa = 0;
    ddreg->nomePessoa = NULL;
    ddreg->tamNomeUsuário = 0;
    ddreg->nomeUsuario = NULL;
    ddreg->tamReg = 0;

    return ddreg;
}

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
    
    fwrite(&dhreg->status,1,1,data_file_p);         // Inicializa o campo status com '0' (1 byte)

    fwrite(&dhreg->qtdPessoas,4,1,data_file_p);     // Inicializa o campo quantidadePessoas com 0 (4 bytes)

    fwrite(&dhreg->qtdRemovidos,4,1,data_file_p);   // Inicializa o campo quantidadeRemovidos com 0 (4 bytes)

    fwrite(&dhreg->proxByteOffset,8,1,data_file_p); // Inicializa o campo proxByteOffset com 0 (8 bytes)
    
    fseek(data_file_p,0,SEEK_SET);                  // Volta ao início do arquivo de dados

    return data_file_p;                             // Retorna o ponteiro para o arquivo
}


void fill_data_file(const char* dest_filename, const char* src_filename)
{   
    char* src_path = get_file_path(src_filename);
    char* dest_path = get_file_path(dest_filename);

    FILE* src_file  = fopen(src_path,"r");
    if(src_file == NULL)
    {
        print_error();
        if (DEBUG)
        {
            printf("Erro ao abir csv ! -> Caminho relativo : %s\n",src_path);
        }
        return;
    }

    FILE* dest_file = fopen(dest_path,"ab");
    if(dest_file == NULL)
    {
        dest_file = create_data_file(dest_filename);
        if(dest_file == NULL && DEBUG)
        {
            printf("Erro ao criar arquivo!\n");
            return;
        }
    }

    free(src_path);
    free(dest_path);

    char* src_str = (char*)calloc(100,sizeof(char));
    fgets(src_str,100,src_file);
    while (!feof(src_file))
    {
        fgets(src_str,100,src_file);
        char** data = strip_by_delim(src_str,',');
        int n_data = atoi(data[0]);
       
        // if(DEBUG)
        // {   
        //     printf("\nSTRIP_BYDELIM:\n\n");
        //     for(int i = 0; i <= n_data; i++)
        //     {
        //         printf("%s ",data[i]);
        //     }
        //     puts("");
        // }

        if(n_data < 4)
        {
            print_error();
            return;
        }

        DATA_DREG* data_reg = create_data_dreg();

        data_reg->idPessoa = atoi(data[1]);
        
        data_reg->tamNomePessoa = strlen(data[2]);

        if(data_reg->tamNomePessoa > 0)
        {
            data_reg->nomePessoa = (char*)calloc(data_reg->tamNomePessoa,sizeof(char));
        
            strcpy(data_reg->nomePessoa,data[2]);
        }

        data_reg->idadePessoa = atoi(data[3]);
        data_reg->tamNomeUsuário = strlen(data[4]);
        if(data_reg->tamNomePessoa > 0)
        {
            data_reg->nomeUsuario = (char*)calloc(data_reg->tamNomePessoa,sizeof(char));
            
        
            strcpy(data_reg->nomeUsuario,data[4]);
        }

        data_reg->tamReg = 21 + data_reg->tamNomePessoa + data_reg->tamNomeUsuário;

        if(DEBUG)
        {
            printf("Em memória primária:\n\n");
            print_ddreg(data_reg);
        }

        
    }
    
    
    close_file(dest_file);
}