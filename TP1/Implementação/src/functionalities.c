
/*
                                TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                XXXXXXXX
                                Vinicius Reis Gonçalves             15491921
*/

//Arquivo 'functionalitites.c', contém a implementação das funcionalidades

#include "all.h"

/*
    Cria um arquivo de índice como especificado, atualizando o status quando necessário

    params:
        const char* index_filename => Nome do arquivo de índice a ser criado
    
    return:
        void
*/
void CREATE_INDEX(const char* index_filename)
{
    FILE* index_file = create_index_file(index_filename);   // Cria o arquivo de índice

    char* index_filepath = get_file_path(index_filename);   // Pega o caminho do arquivo

    update_file_status(index_file,'1'); // Atualiza o status para '1' (consistente)

    fclose(index_file); // Fecha o arquivo

    binarioNaTela(index_filepath);

    free(index_filepath);   // Libera a memória da string contendo o caminho do arquivo
}

/*
    Lê os dados de um arquivo fonte, grava num arquivo de dados 'output_filename' no formato binária e o indexa à um arquivo de índice primário 'index_filename' no formato binário, já existente

    params:
        const char* src_filename => Nome do arquivo fonte dos dados (já existente)
        const char* output_filename => Nome do arquivo binário gerado
        const cahr* index_filename => Nome do arquivo de índice (já existente)
    
    return:
        void
*/
void CREATE_TABLE(const char* src_filename, const char* output_filename, const char* index_filename)
{
    char* src_path = get_file_path(src_filename);           // Pega o caminho do arquivo fonte
    FILE* src_file = fopen(src_path,"r");                   // Tenta abrir o arquivo no formato de leitura
    if(src_file == NULL)
    {
        print_error();                                      // Se o arquivo for NULL (não existe), exibe uma mensagem de erro e sai da funcionalidade
        return;
    }
    free(src_path);                                         // Libera a memória da string do caminho

    FILE* output_file = create_data_file(output_filename);  // Cria o arquivo de dados
    update_file_status(output_file,'0');                    // Atualiza o 'status' do arquivo de dados para '0'

    char* index_path = get_file_path(index_filename);       // Pega o caminho do arquivo de índice
    FILE* index_file = fopen(index_path,"rb+");             // Abre o arquivo para escrita (sem apagar os dados já existentes) binária
    if(index_file == NULL)
    {
        print_error();                                      // Se o ponteiro para o arquivo for NULL (arquivo não existe), exibe mensagem de erro e sai da funcionalidade
        return;
    }
    update_file_status(index_file,'0');                    // Atualiza o 'status' do arquivo de índice para '0'
    
    INDEX_ARR* idx = fill_data_file(src_file,output_file);  // Preenche o arquivo de dados e retorna o de índice ordenado em memória primária
    
    fclose(src_file);                                       // Fecha o arquivo fonte
    
    update_file_status(output_file,'1');                    // Atualiza 'status' do arquivo de dados para '1'
    
    fclose(output_file);                                    // Fecha o arquivo de dados
    
    write_on_index_file(index_file,idx);                    // Grava as informações d índice no arquivo de índice
    update_file_status(index_file,'1');                     // Atualiza o 'status' do arquivo de índice para '1'
    fclose(index_file);                                     // Fecha o arquivo de índice

    char* output_filepath = get_file_path(output_filename); // Pega o caminho do arquivo de dados

    destroy_index_arr(idx);                                 // Libera a memória do vetor de tegistros do arquivo de indíce
    
    binarioNaTela(output_filepath);
    binarioNaTela(index_path);

    free(index_path);                                       // Libera a memória da string do caminho do arquivo de índice
    free(output_filepath);                                  // Libera a memória da string do caminho de dados

}

/* 
    Recebe um arquivo já aberto e puxa para a memória primária o conteúdo de um registro,
    printanto-o se ele não estiver removido. Retorna quantos bytes foram lidos.

    params:
        const FILE* data_file => Arquivo fonte dos dados (já existente)
        const long byte_offset => Byte offset do registro

    return:
        int size_selected => Número de bytes lidos

*/

int SELECT(FILE* const data_file, long const byte_offset)
{
    if(data_file ==  NULL)  //testa se o arquivo existe
    {
        print_error();
        exit(EXIT_FAILURE);
    }
    
    fseek(data_file, byte_offset, SEEK_SET); //posiciona o cursor do arquivo no primeiro byte do registro
    
    DATA_DREG* data_register = create_data_dreg(); //cria uma variável de registro

    fread(&(data_register->removido), 1, 1, data_file); //Começa a ler os parâmetros
    fread(&(data_register->tamReg), 4, 1, data_file);

    int size_selected = data_register->tamReg + 5;  //Quantidade de bytes lidos
    if(data_register->removido == '1')  // Caso o arquivo seja removido, sai da função
    {
        return size_selected;
    }

    fread(&(data_register->idPessoa), 4, 1, data_file);
    fread(&(data_register->idadePessoa), 4, 1, data_file);
        
    fread(&(data_register->tamNomePessoa), 4, 1, data_file);
    if(data_register->tamNomePessoa > 0) //Caso a pessoa tenha nome ele será lido
    {
        data_register->nomePessoa = (char*)calloc(1, data_register->tamNomePessoa);
        fread(data_register->nomePessoa, data_register->tamNomePessoa, 1, data_file);
    }

    fread(&(data_register->tamNomeUsuario), 4, 1, data_file);
    if(data_register->tamNomeUsuario > 0) //Caso o usuário tenha nome ele será lido
    {
        data_register->nomeUsuario = (char*)calloc(1, data_register->tamNomeUsuario);
        fread(data_register->nomeUsuario, data_register->tamNomeUsuario, 1, data_file);
    }

    print_data_register(*data_register); //printa o registro
    destroy_data_dreg(data_register);   //Destroi e apaga o registro da memória primária

    return size_selected;
}

/* 
    Recebe o nome do arquivo de dados, abre-o para leitura, caso exista, e printa seus registros 

    params:
        const char* data_filename => nome do arquivo a ser aberto

    return:
        void

*/

void SELECT_FROM_TABLE(const char* data_filename)
{
    char* data_path = get_file_path(data_filename); //Puxa o caminho do arquivo
    FILE* data_file = fopen(data_path, "rb");   //Abre o arquivo para leitura

    if(data_file == NULL)   //Testa se a abertura foi bem sucedida
    {
        print_error();  //Printa a mensagem de erro e retorna
        return;
    }
    free(data_path);

    fseek(data_file, 9, SEEK_SET);  //Coloca o cursor de arquivos de dados no byte offset
    long final_byte;                // do cabeçalho contendo próximo byte offset livre
    fread(&final_byte, 8, 1, data_file);    //Pega o próximo byte offset livre (fim do arquivo)

    long current_byte = DF_HEAD_REG_LEN;    // coloca byte atual logo após o cabeçalho
    fseek(data_file, DF_HEAD_REG_LEN, SEEK_SET);    //coloca o cursor logo após o cabeçalho

    if(current_byte == final_byte)  // caso não houver nenhum registro, sai da função
    {
        printf("Registro inexistente.");
        return;
    }

    while(current_byte < final_byte) //enquanto ainda há registro, printa o registro atual
    {
        int register_size = SELECT(data_file, current_byte);
        current_byte = current_byte + register_size;
    }

    fclose(data_file); 
}

/* 
    Recebe o número de pesquisas a serem feitas e o nome do arquivo de dados,
    abre-o para leitura, caso exista, e recebe o tipo do campo e o valor do campo
    para usar na pesquisa, caso seja feita a pesquisa por ID, usa o arquivo de indice

    params:
        const char* data_filename => nome do arquivo de dados a ser aberto
        const char* index_filename => nome do arquivo de index caso seja nescessário
        int search number => número de pesquisas a serem feitas

    return:
        void

*/

void SELECT_FROM_WHERE(const char* data_filename, const char* index_filename, int search_number)
{
    char *data_path = get_file_path(data_filename);  // Pega o caminho do arquivo de dados e
    FILE* data_file = fopen(data_path, "rb");        // abre-o para leitura
    free(data_path);

    if(data_file == NULL)   //caso o arquivo não seja aberto, printa a mensagem de erro e sai da função
    {
        print_error();
        return;
    }

    FILE* index_file = NULL;

    for(int i = 1; i <= search_number; i++)
    {   
        char str_in[50];    //Interage com usuário, pegando uma string do tipo
        fgets(str_in , 50, stdin);  //"n tipoCampo=Valor"

        char** type_and_value = strip_by_delim(str_in, '=');    //separa o tipo e valor
        char** number_type = strip_by_delim(type_and_value[1], ' ');    //separa o tipo do número

        type_and_value[2] = remove_quotes(type_and_value[2]); //remove as aspas do valor

        long final_byte;                //Coloca o cursor de arquivos de dados no byte offset
        fseek(data_file, 9, SEEK_SET); // do cabeçalho contendo próximo byte offset livre
        fread(&final_byte, 8, 1, data_file);   //Pega o próximo byte offset livre (fim do arquivo)

        long current_byte = DF_HEAD_REG_LEN;     // coloca byte atual logo após o cabeçalho
        fseek(data_file, DF_HEAD_REG_LEN, SEEK_SET);     //coloca o cursor logo após o cabeçalho

        int FLAG = 1; //FLAG para saber se a pesquisa achou ou não algum registro válido

        if(strcmp("idadePessoa", number_type[2]) == 0)
        {   
            while (current_byte < final_byte)
            {
                int age;
                int size;

                fseek(data_file, current_byte + 1, SEEK_SET);   //Pega o tamanho do registro
                fread(&size, 4, 1, data_file);
                size = size + 5;    //adiciona 5, pois o tamanho do arquivo não conta os 5
                                    //primeiros bytes (removido e tam_registro)

                fseek(data_file, current_byte + 9, SEEK_SET);   //Pega a idade da pessoa no registro
                fread(&age, 4, 1, data_file);

                if(age == atoi(type_and_value[2]))  
                {
                    SELECT(data_file, current_byte);
                    FLAG = 0;
                }

                current_byte = current_byte + size;
            }

            if(FLAG)
            {
                printf("Registro inexistente.\n\n");
            }
            //printf("\n");
        }
        else if(strcmp("nomePessoa", number_type[2]) == 0)
        {
            while (current_byte < final_byte)
            {
                char* name;
                int size_name;
                int size;

                fseek(data_file, current_byte + 1, SEEK_SET);
                fread(&size, 4, 1, data_file);
                size = size + 5;

                fseek(data_file, current_byte + 13, SEEK_SET);
                fread(&size_name, 4, 1, data_file);

                name = (char*)calloc(1, size_name);
                fseek(data_file, current_byte + 17, SEEK_SET);
                fread(name, size_name, 1, data_file);

                end_string_on_mark(type_and_value[2], '\n');
                end_string_on_mark(type_and_value[2], '\r');

                if(strcmp(name, type_and_value[2]) == 0)
                {
                    SELECT(data_file, current_byte);
                    FLAG = 0;
                }

                current_byte = current_byte + size;
            }

            if(FLAG)
            {
                printf("Registro inexistente.\n\n");
            }
            //printf("\n");
        }
        else if(strcmp("nomeUsuario", number_type[2]) == 0)
        {
            while (current_byte < final_byte)
            {
                char* name;
                int size_name;
                int size_name_person;
                int size;

                fseek(data_file, current_byte + 1, SEEK_SET);
                fread(&size, 4, 1, data_file);
                size = size + 5;

                fseek(data_file, current_byte + 13, SEEK_SET);
                fread(&size_name_person, 4, 1, data_file);

                fseek(data_file, current_byte + 17 + size_name_person, SEEK_SET);
                fread(&size_name, 4, 1, data_file);

                name = (char*)calloc(1, size_name);
                fseek(data_file, current_byte + 21 + size_name_person, SEEK_SET);
                fread(name, size_name, 1, data_file);

                end_string_on_mark(type_and_value[2], '\n');
                end_string_on_mark(type_and_value[2], '\r');

                if(strcmp(name, type_and_value[2]) == 0)
                {
                    SELECT(data_file, current_byte);
                    FLAG = 0;
                }

                current_byte = current_byte + size;
            }

            if(FLAG)
            {
                printf("Registro inexistente.\n\n");
            }
            //printf("\n");
        }
        else if(strcmp("idPessoa", number_type[2]) == 0)
        {
            char *index_path = get_file_path(index_filename);  
            index_file = fopen(index_path, "rb");
            free(index_path);

            int id = atoi(type_and_value[2]);

            INDEX_ARR *idx_array = save_index_in_mem(index_file);
            int len = idx_array->len;

            int left = 0;
            int right = len - 1;
            int middle;

            while(1)
            {
                middle = left + (right - left)/2;

                if(left == right)
                {
                    break;
                }

                if(idx_array->idx_arr[middle].idPessoa == id)
                {
                    break;
                }
                else if(idx_array->idx_arr[middle].idPessoa > id)
                {
                    right = middle - 1;
                }
                else
                {
                    left = middle + 1;
                }
            }

            if(idx_array->idx_arr[middle].idPessoa != id)
            {
                printf("Registro inexistente.\n");
            }
            else
            {
                long byte_offset = idx_array->idx_arr[middle].byteOffset;
                SELECT(data_file, byte_offset);
            }

            destroy_index_arr(idx_array);
        }
        else
        {
            printf("Opção Inválida");
            return;
        }
    }

    fclose(data_file);

    if(index_file != NULL)
    {
        fclose(index_file);
    }
}