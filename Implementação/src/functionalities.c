
/*
                                TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III

                                Pedro Avelar Machado                15497396
                                Vinicius Reis Gonçalves             15491921
*/

// Arquivo 'functionalitites.c', contém a implementação das funcionalidades

#include "all.h"

/*
    Cria um arquivo de índice como especificado, atualizando o status quando necessário

    params:
        const char* index_filename => Nome do arquivo de índice a ser criado

    return:
        void
*/
void CREATE_INDEX(const char *index_filename)
{
    FILE *index_file = create_index_file(index_filename); // Cria o arquivo de índice

    char *index_filepath = get_file_path(index_filename); // Pega o caminho do arquivo

    update_file_status(index_file, '1'); // Atualiza o status para '1' (consistente)

    fclose(index_file); // Fecha o arquivo

    binarioNaTela(index_filepath);

    free(index_filepath); // Libera a memória da string contendo o caminho do arquivo
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
void CREATE_TABLE(const char *src_filename, const char *output_filename, const char *index_filename)
{
    char *src_path = get_file_path(src_filename); // Pega o caminho do arquivo fonte
    FILE *src_file = fopen(src_path, "r");        // Tenta abrir o arquivo no formato de leitura
    if (src_file == NULL)
    {
        print_error(); // Se o arquivo for NULL (não existe), exibe uma mensagem de erro e sai da funcionalidade
        return;
    }
    free(src_path); // Libera a memória da string do caminho

    FILE *output_file = create_data_file(output_filename); // Cria o arquivo de dados
    update_file_status(output_file, '0');                  // Atualiza o 'status' do arquivo de dados para '0'

    char *index_path = get_file_path(index_filename); // Pega o caminho do arquivo de índice
    FILE *index_file = fopen(index_path, "rb+");      // Abre o arquivo para escrita (sem apagar os dados já existentes) binária
    if (index_file == NULL)
    {
        print_error(); // Se o ponteiro para o arquivo for NULL (arquivo não existe), exibe mensagem de erro e sai da funcionalidade
        return;
    }
    update_file_status(index_file, '0'); // Atualiza o 'status' do arquivo de índice para '0'

    INDEX_ARR *idx = fill_data_file(src_file, output_file); // Preenche o arquivo de dados e retorna o de índice ordenado em memória primária

    fclose(src_file); // Fecha o arquivo fonte

    update_file_status(output_file, '1'); // Atualiza 'status' do arquivo de dados para '1'

    fclose(output_file); // Fecha o arquivo de dados

    write_on_index_file(index_file, idx); // Grava as informações d índice no arquivo de índice
    update_file_status(index_file, '1');  // Atualiza o 'status' do arquivo de índice para '1'
    fclose(index_file);                   // Fecha o arquivo de índice

    char *output_filepath = get_file_path(output_filename); // Pega o caminho do arquivo de dados

    destroy_index_arr(idx); // Libera a memória do vetor de tegistros do arquivo de indíce

    binarioNaTela(output_filepath);
    binarioNaTela(index_path);

    free(index_path);      // Libera a memória da string do caminho do arquivo de índice
    free(output_filepath); // Libera a memória da string do caminho de dados
}

/*
    Recebe um arquivo já aberto e puxa para a memória primária o conteúdo de um registro,
    printanto-o se ele não estiver removido. Retorna quantos bytes foram lidos.

    params:
        const FILE* data_file => Arquivo fonte dos dados (já existente)
        const long byte_offset => Byte offset do registro
        int* NO_REGISTER => Zero caso o registro não esteja removido

    return:
        int size_selected => Número de bytes lidos

*/

int SELECT(FILE *const data_file, long const byte_offset, int *NO_REGISTER)
{
    if (data_file == NULL) // testa se o arquivo existe
    {
        print_error();
        exit(EXIT_FAILURE);
    }

    DATA_DREG *data_register = pull_reg_to_memory(byte_offset, data_file);  // Puxa o registro para memória

    int size_selected = data_register->tamReg + 5; // Quantidade de bytes lidos
    if (data_register->removido == '1')            // Caso o arquivo seja removido, sai da função
    {
        destroy_data_dreg(data_register);
        return size_selected;
    }

    *NO_REGISTER = 0;

    print_data_register(*data_register); // printa o registro
    destroy_data_dreg(data_register);    // Destroi e apaga o registro da memória primária

    return size_selected;
}

/*
    Recebe o nome do arquivo de dados, abre-o para leitura, caso exista, e printa seus registros

    params:
        const char* data_filename => nome do arquivo a ser aberto

    return:
        void

*/

void SELECT_FROM_TABLE(const char *data_filename)
{
    char *data_path = get_file_path(data_filename); // Puxa o caminho do arquivo
    FILE *data_file = fopen(data_path, "rb");       // Abre o arquivo para leitura

    if (data_file == NULL) // Testa se a abertura foi bem sucedida
    {
        print_error(); // Printa a mensagem de erro e retorna
        return;
    }
    free(data_path);

    fseek(data_file, 9, SEEK_SET);       // Coloca o cursor de arquivos de dados no byte offset
    long final_byte;                     // do cabeçalho contendo próximo byte offset livre
    fread(&final_byte, 8, 1, data_file); // Pega o próximo byte offset livre (fim do arquivo)

    long current_byte = DF_HEAD_REG_LEN;         // coloca byte atual logo após o cabeçalho
    fseek(data_file, DF_HEAD_REG_LEN, SEEK_SET); // coloca o cursor logo após o cabeçalho

    int NO_REGISTER = 1;

    if (current_byte == final_byte) // caso não houver nenhum registro, sai da função
    {
        printf("Registro inexistente.");
        return;
    }

    while (current_byte < final_byte) // enquanto ainda há registro, printa o registro atual
    {
        int register_size = SELECT(data_file, current_byte, &NO_REGISTER);
        current_byte = current_byte + register_size;
    }

    if (NO_REGISTER) // caso houver registros, mas estiverem removidos
    {
        printf("Registro inexistente.");
        return;
    }

    fclose(data_file);
}

/*
    Recebe o arquivo de dados já aberto, seu final e byte atual, um campo e um valor e encontra
    o primeiro registro após o byte atual cujo o campo tenha esse valor. Usa, abrindo caso
    necessário o arquivo de indice no caso de procurar por ID. Caso não exista, retorna o último
    byte.

    params:
        FILE* data_file => arquivo de dados
        FILE* index_file => arquivo de índice
        const char* index_filename => nome do arquivo de indice (para abrir se estiver fechado)
        char* type => campo
        char* value => valor
        long current_byte => byte atual
        long final_byte => byte final

    return:
        long current_byte => retorna o byte atual quando o registro é encontrado

*/

long WHERE(FILE *data_file, FILE *index_file, const char* index_filename, char *type, char *value, long current_byte, long final_byte)
{
    fseek(data_file, current_byte, SEEK_SET);   //Coloca o cursor do arquivo no byte atual

    if (strcmp("idadePessoa", type) == 0)
    {
        if(strcmp("NULO", value) == 0)
        {
            value = "-1";
        }

        while (current_byte < final_byte)
        {
            int age;
            int size;

            fseek(data_file, current_byte + 1, SEEK_SET); // Pega o tamanho do registro
            fread(&size, 4, 1, data_file);
            size = size + 5; // adiciona 5, pois o tamanho do arquivo não conta os 5
                             // primeiros bytes (removido e tam_registro)

            fseek(data_file, current_byte + 9, SEEK_SET); // Pega a idade da pessoa no registro
            fread(&age, 4, 1, data_file);

            if (age == atoi(value)) // Retorna o primeiro registro desejado desde o byte atual
            {
                return current_byte;
            }

            current_byte = current_byte + size;
        }
    }
    else if (strcmp("nomePessoa", type) == 0)
    {
        while (current_byte < final_byte)
        {
            char *name;
            int size_name;
            int size;

            fseek(data_file, current_byte + 1, SEEK_SET); // Pega o tamanho de registro
            fread(&size, 4, 1, data_file);                // adiciona 5, pois o tamanho do arquivo não conta os 5
            size = size + 5;                              // primeiros bytes (removido e tam_registro)

            fseek(data_file, current_byte + 13, SEEK_SET); // Pega o tamanho do campo nomePessoa
            fread(&size_name, 4, 1, data_file);
            
            if(size_name != 0)
            {
                name = (char *)calloc(1, size_name); // Pega o nomePessoa
                fseek(data_file, current_byte + 17, SEEK_SET);
                fread(name, size_name, 1, data_file);
            }
            else
            {
                name = (char*)calloc(1, 5);
                name = "NULO";
            }

            if (strcmp(name, value) == 0) // Retorna o primeiro registro desejado desde o byte atual 
            {
                return current_byte;
            }

            current_byte = current_byte + size;
        }
    }
    else if (strcmp("nomeUsuario", type) == 0)
    {
        while (current_byte < final_byte)
        {
            char *name;
            int size_name;
            int size_name_person;
            int size;

            fseek(data_file, current_byte + 1, SEEK_SET); // Pega o tamanho do registro
            fread(&size, 4, 1, data_file);
            size = size + 5;

            fseek(data_file, current_byte + 13, SEEK_SET); // Pega o tamanho do nome Pessoa
            fread(&size_name_person, 4, 1, data_file);     // faz isso para poder navegar pelo registro

            fseek(data_file, current_byte + 17 + size_name_person, SEEK_SET); // pega o tamanho do nome Usuario
            fread(&size_name, 4, 1, data_file);

            if(size_name != 0)
            {
                name = (char *)calloc(1, size_name); // pega o nomeUsuario
                fseek(data_file, current_byte + 21 + size_name_person, SEEK_SET);
                fread(name, size_name, 1, data_file);
            }
            else   
            {
                name = (char*)calloc(1, 5);
                name = "NULO";
            }

            if (strcmp(name, value) == 0)
            {
                return current_byte;
            }

            current_byte = current_byte + size;
        }
    }
    else if (strcmp("idPessoa", type) == 0)
    {
        if(current_byte != DF_HEAD_REG_LEN || strcmp("NULO", value) == 0) 
                                    // Como todo usuário possui um ID único, apenas adimite
        {                           // a busca quando no começo do registros de dados
            return final_byte;      // e o ID é válido (diferente de "NULO")
        }
        
        if (index_file == NULL) // abre o arquivo de indice caso ele não estivesse aberto ainda
        {
            char *index_path = get_file_path(index_filename);
            index_file = fopen(index_path, "rb");
            free(index_path);
        }

        int id = atoi(value);

        INDEX_ARR *idx_array = save_index_in_mem(index_file); // puxa o indice para memória primária
        int len = idx_array->len;

        int pos = index_binary_search(idx_array, id);

        // Pos == -1 => não há ID igual ao ID desejado
        if (pos == -1)
        {
            return final_byte;
        }
        else // Caso o contrario retorna o byteoffset desejado
        {
            return idx_array->idx_arr[pos].byteOffset;
        }

        destroy_index_arr(idx_array); // Desaloca memória
    }
    else
    {
        printf("Opção Inválida");
        exit(EXIT_FAILURE);
    }

    return final_byte;
}

/*
    Recebe o número de pesquisas a serem feitas e o nome do arquivo de dados,
    abre-o para leitura, caso exista, e recebe o tipo do campo e o valor do campo
    para usar na pesquisa, caso seja feita a pesquisa por ID, usa o arquivo de indice.
    Printa os registros encontrados em cada pesquisa.

    params:
        const char* data_filename => nome do arquivo de dados a ser aberto
        const char* index_filename => nome do arquivo de index caso seja nescessário
        int search number => número de pesquisas a serem feitas

    return:
        void

*/

void SELECT_FROM_WHERE(const char *data_filename, const char *index_filename, int search_number)
{
    char *data_path = get_file_path(data_filename); // Pega o caminho do arquivo de dados e
    FILE *data_file = fopen(data_path, "rb");       // abre-o para leitura
    free(data_path);

    if (data_file == NULL) // caso o arquivo não seja aberto, printa a mensagem de erro e sai da função
    {
        print_error();
        return;
    }

    FILE *index_file = NULL;

    for (int i = 1; i <= search_number; i++)
    {
        char str_in[50];          // Interage com usuário, pegando uma string do tipo
        fgets(str_in, 50, stdin); //"n tipoCampo=Valor"

        char **type_and_value = strip_by_delim(str_in, '=');         // separa o tipo e valor
        char **number_type = strip_by_delim(type_and_value[1], ' '); // separa o tipo do número

        remove_quotes(type_and_value[2]); // remove as aspas do valor

        end_string_on_mark(type_and_value[2], "\n"); // retira o '\n' e o '\r' das strings
        end_string_on_mark(type_and_value[2], "\r"); // lidas

        long final_byte;                     // Coloca o cursor de arquivos de dados no byte offset
        fseek(data_file, 9, SEEK_SET);       // do cabeçalho contendo próximo byte offset livre
        fread(&final_byte, 8, 1, data_file); // Pega o próximo byte offset livre (fim do arquivo)

        long current_byte = DF_HEAD_REG_LEN;         // coloca byte atual logo após o cabeçalho
        fseek(data_file, DF_HEAD_REG_LEN, SEEK_SET); // coloca o cursor logo após o cabeçalho

        int no_register = 1; // FLAG para saber se a pesquisa achou ou não algum registro válido

        while(1)
        {
            // Procura o primeiro byte offset desejado depois do byte_atual
            current_byte = WHERE(data_file, index_file, index_filename,
                           number_type[2], type_and_value[2], current_byte, final_byte);
            
            if(current_byte >= final_byte) //Caso o byte atual chegue no final para a busca
            {
                break;
            }

            int size = SELECT(data_file, current_byte, &no_register); //printa o registro encontrado
            current_byte = current_byte + size;
        }

        if(no_register)    //Caso não encontre nenhum registro não removido
        {
            printf("Registro inexistente.\n");
        }

        // Desaloca memória
        destroy_strip_matrix(type_and_value);
        destroy_strip_matrix(number_type);
    }

    fclose(data_file);

    if (index_file != NULL)
    {
        fclose(index_file);
    }
}

/*
    Recebe o número de pesquisas a serem feitas e o nome do arquivo de dados,
    abre-o para leitura e escrita, caso exista, e recebe do usuário 
    tipo do campo e o valor do campo
    para usar na pesquisa, caso seja feita a pesquisa por ID, usa o arquivo de indice.
    Marca os registros encontrado em cada pesquisa como lógicamente removidos.

    params:
        const char* data_filename => nome do arquivo de dados a ser aberto
        const char* index_filename => nome do arquivo de index
        int delete number => número de deleções a serem feitas

    return:
        void

*/

void DELETE_FROM_WHERE (char *data_filename, char *index_filename, int delete_number)
{
    char *data_path = get_file_path(data_filename); // Pega o caminho do arquivo de dados e
    FILE *data_file = fopen(data_path, "r+b");       // abre-o para leitura e escrita
    free(data_path);

    if (data_file == NULL) // caso o arquivo não seja aberto, printa a mensagem de erro e sai da função
    {
        print_error();
        return;
    }

    update_file_status(data_file, '0'); // atualiza o status para inconsistente

    char *index_path = get_file_path(index_filename);
    FILE *index_file = fopen(index_path, "r+b");    // abre-o para leitura e escrita
    free(index_path);
    
    if (index_file == NULL) // caso o arquivo não seja aberto, printa a mensagem de erro e sai da função
    {
        print_error();
        return;
    }

    update_file_status(index_file, '0'); // atualiza o status para inconsistente

    int num_people;
    int num_removed;

    fread(&num_people, 4, 1, data_file);    // Le do cabeçalho o número de pessoas e o número
    fread(&num_removed, 4, 1, data_file);   // de registros removidos

    INDEX_ARR* idx_array = save_index_in_mem(index_file);
    const char removed = '1';

    for (int i = 1; i <= delete_number; i++)
    {
        char str_in[50];          // Interage com usuário, pegando uma string do tipo
        fgets(str_in, 50, stdin); //"n tipoCampo=Valor"

        char **type_and_value = strip_by_delim(str_in, '=');         // separa o tipo e valor
        char **number_type = strip_by_delim(type_and_value[1], ' '); // separa o tipo do número

        remove_quotes(type_and_value[2]); // remove as aspas do valor

        end_string_on_mark(type_and_value[2], "\n"); // retira o '\n' e o '\r' das strings
        end_string_on_mark(type_and_value[2], "\r"); // lidas

        long final_byte;                     // Coloca o cursor de arquivos de dados no byte offset
        fseek(data_file, 9, SEEK_SET);       // do cabeçalho contendo próximo byte offset livre
        fread(&final_byte, 8, 1, data_file); // Pega o próximo byte offset livre (fim do arquivo)

        long current_byte = DF_HEAD_REG_LEN;         // coloca byte atual logo após o cabeçalho
        fseek(data_file, DF_HEAD_REG_LEN, SEEK_SET); // coloca o cursor logo após o cabeçalho

        while(1)
        {
            // Procura o primeiro byte offset desejado depois do byte_atual
            current_byte = WHERE(data_file, index_file, index_filename,
                           number_type[2], type_and_value[2], current_byte, final_byte);
            
            if(current_byte >= final_byte) //Caso o byte atual chegue no final para a busca
            {
                break;
            }

            fseek(data_file, current_byte, SEEK_SET);
            fwrite(&removed, 1, 1, data_file); // Marca como logicamente removido o registro encontrado
            
            int size;
            fread(&size, 4, 1, data_file);  // Pega o campo "Tamanho_registro"
            
            current_byte = current_byte + size + 5; // Próxima pesquisa começa a partir do 
                                                    // próximo campo

            int id;
            fread(&id, 4, 1, data_file);    // Pega o campo "id"

            remove_id_array(idx_array, id); // Retira o ID achado do indice

            num_people--;   //Diminui o número de pesssoas e aumenta o número de removidos
            num_removed++;
        }

        // Desaloca memória
        destroy_strip_matrix(type_and_value);
        destroy_strip_matrix(number_type);
    }

    write_on_index_file(index_file, idx_array); // Reescreve o arquivo de indice;

    destroy_index_arr(idx_array);

    update_file_status(data_file, '1'); //atualiza o status do arquivo para consistente
    fwrite(&num_people, 4, 1, data_file);   //atualiza o cabeçalho
    fwrite(&num_removed, 4, 1, data_file);

    update_file_status(index_file, '1'); //atualiza o status do arquivo para consistente

    fclose(data_file);
    fclose(index_file);

    binarioNaTela(data_filename);
    binarioNaTela(index_filename);
}

/*
    Recebe o número de pesquisas a serem feitas e o nome do arquivo de dados,
    abre-o para leitura e escrita, caso exista. O usuário informa os valores
    dos campos do registro a ser inserido, e a função insere-o no final

    params:
        const char* data_filename => nome do arquivo de dados a ser aberto
        const char* index_filename => nome do arquivo de index caso seja nescessário
        int insert_number => número de inserções a serem feitas

    return:
        void

*/

void INSERT_INTO(char* data_filename, char *index_filename, int insert_number)
{
    char *data_path = get_file_path(data_filename); // Pega o caminho do arquivo de dados e
    FILE *data_file = fopen(data_path, "r+b");       // abre-o para leitura e escrita
    free(data_path);

    if (data_file == NULL) // caso o arquivo não seja aberto, printa a mensagem de erro e sai da função
    {
        print_error();
        return;
    }

    update_file_status(data_file, '0'); // atualiza o status para inconsistente

    char *index_path = get_file_path(index_filename);
    FILE *index_file = fopen(index_path, "r+b");    // abre-o para leitura e escrita
    free(index_path);
    
    if (index_file == NULL) // caso o arquivo não seja aberto, printa a mensagem de erro e sai da função
    {
        print_error();
        return;
    }

    update_file_status(index_file, '0'); // atualiza o status para inconsistente

    int num_people;

    fread(&num_people, 4, 1, data_file);    // Le do cabeçalho o número de pessoas

    INDEX_ARR* idx_array = save_index_in_mem(index_file);  

    fseek(data_file, 9, SEEK_SET);  // Posição do final byte no cabeçalho
    long final_byte;
    fread(&final_byte, 8, 1, data_file);

    for(int i = 0; i < insert_number; i++)
    {
        char str_in[100];
        fgets(str_in, 100, stdin);

        char** values = strip_by_delim(str_in, ',');
        for(int j = 1; j < 5; j++)
        {
            remove_everychar_until_space(values[j]);    //Retira o espaço de cada campo  e o número da pesquisa do primeiro campo
            remove_quotes(values[j]);
            end_string_on_mark(values[j], "\n");
            end_string_on_mark(values[j], "\r");
        }

        //CRIA O REGISTRO DE PESSOA COM AS INFORMAÇÕES DISPOSTAS

        DATA_DREG* new_data_reg = create_data_dreg();  
        
        //idPessoa
        new_data_reg->idPessoa = atoi(values[1]); 
        
        //nomePessoa
        if(strcmp(values[2], "NULO") != 0)  
        {
            new_data_reg->tamNomePessoa = strlen(values[2]);
            new_data_reg->nomePessoa = (char*)calloc(strlen(values[2]), 1);
            strcpy(new_data_reg->nomePessoa, values[2]);
        }
        else
        {
            new_data_reg->tamNomePessoa = 0;
        }
        
        //idadePessoa
        if(strcmp(values[3], "NULO") != 0)
        {
            new_data_reg->idadePessoa = atoi(values[3]);
        }
        else
        {
            new_data_reg->idadePessoa = -1;
        }

        //nomeUsuário
        new_data_reg->tamNomeUsuario = strlen(values[4]);
        new_data_reg->nomeUsuario = (char*)calloc(strlen(values[4]), 1);
        strcpy(new_data_reg->nomeUsuario, values[4]);

        //Tamanho e status
        new_data_reg->removido = '0';
        new_data_reg->tamReg = 16 + new_data_reg->tamNomePessoa + new_data_reg->tamNomeUsuario;

        //TERMINO DA CRIAÇÂO

        push_reg_to_memory(final_byte, data_file, new_data_reg);    //Salva o registro no fim do arquivo
        add_id_array(&idx_array, new_data_reg->idPessoa, final_byte);
        final_byte = final_byte + 5 + new_data_reg->tamReg;
        
        num_people++;

        destroy_strip_matrix(values);
        destroy_data_dreg(new_data_reg);
    }

    order_index(idx_array);
    write_on_index_file(index_file, idx_array);
    destroy_index_arr(idx_array);

    update_file_status(data_file, '1'); //atualiza o status do arquivo para consistente
    fwrite(&num_people, 4, 1, data_file);   //atualiza o cabeçalho
    fseek(data_file, 9, SEEK_SET);  // Pula para a parte do cabeçalho do final_byte
    fwrite(&final_byte, 8, 1, data_file);

    update_file_status(index_file, '1'); //atualiza o status do arquivo para consistente

    fclose(data_file);
    fclose(index_file);

    binarioNaTela(data_filename);
    binarioNaTela(index_filename);
}
