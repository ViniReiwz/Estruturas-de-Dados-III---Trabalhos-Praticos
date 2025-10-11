
/*
                                TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
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

int SELECT(FILE* data_file, long byte_offset)
{
    fseek(data_file, byte_offset, SEEK_SET);
    
    DATA_DREG* data_register = create_data_dreg();

    fread(&(data_register->removido), 1, 1, data_file);
    fread(&(data_register->tamReg), 4, 1, data_file);

    int size_selected = data_register->tamReg + 5;
    if(data_register->removido == '1')
    {
        return size_selected;
    }

    fread(&(data_register->idPessoa), 4, 1, data_file);
    fread(&(data_register->idadePessoa), 4, 1, data_file);
        
    fread(&(data_register->tamNomePessoa), 4, 1, data_file);
    if(data_register->tamNomePessoa > 0)
    {
        data_register->nomePessoa = (char*)calloc(1, data_register->tamNomePessoa);
        fread(data_register->nomePessoa, data_register->tamNomePessoa, 1, data_file);
    }

    fread(&(data_register->tamNomeUsuario), 4, 1, data_file);
    if(data_register->tamNomeUsuario > 0)
    {
        data_register->nomeUsuario = (char*)calloc(1, data_register->tamNomeUsuario);
        fread(data_register->nomeUsuario, data_register->tamNomeUsuario, 1, data_file);
    }

    print_data_register(*data_register);
    destroy_data_dreg(data_register);

    return size_selected;
}

void SELECT_FROM_TABLE(const char* data_filename)
{
    char* data_path = get_file_path(data_filename);
    FILE* data_file = fopen(data_path, "rb");

    if(data_file == NULL)
    {
        print_error();
        return;
    }
    free(data_path);

    fseek(data_file, 9, SEEK_SET);

    long final_byte;
    fread(&final_byte, 8, 1, data_file);

    long current_byte = DF_HEAD_REG_LEN;
    fseek(data_file, DF_HEAD_REG_LEN, SEEK_SET);

    if(current_byte == final_byte)
    {
        printf("Registro inexistente.");
        return;
    }

    while(current_byte < final_byte)
    {
        int register_size = SELECT(data_file, current_byte);
        current_byte = current_byte + register_size;
    }

    fclose(data_file); 
}

void SELECT_FROM_WHERE(const char* data_filename, const char* index_filename, int search_number)
{
    char *data_path = get_file_path(data_filename);  
    FILE* data_file = fopen(data_path, "rb");
    free(data_path);

    if(data_file == NULL)
    {
        print_error();
        return;
    }

    FILE* index_file = NULL;

    for(int i = 1; i <= search_number; i++)
    {   
        char str_in[50];
        fgets(str_in , 50, stdin);

        char** type_and_value = strip_by_delim(str_in, '=');
        char** number_type = strip_by_delim(type_and_value[1], ' ');

        type_and_value[2] = remove_quotes(type_and_value[2]);

        long final_byte;
        fseek(data_file, 9, SEEK_SET);
        fread(&final_byte, 8, 1, data_file);

        long current_byte = DF_HEAD_REG_LEN;
        fseek(data_file, DF_HEAD_REG_LEN, SEEK_SET);

        int FLAG = 1;

        if(strcmp("idadePessoa", number_type[2]) == 0)
        {   
            while (current_byte < final_byte)
            {
                int age;
                int size;

                fseek(data_file, current_byte + 1, SEEK_SET);
                fread(&size, 4, 1, data_file);
                size = size + 5;

                fseek(data_file, current_byte + 9, SEEK_SET);
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
                end_string_on_mark(type_and_value[2],'\n');
                end_string_on_mark(type_and_value[2],'\r');
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
            puts("Entrou nome usuario");
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