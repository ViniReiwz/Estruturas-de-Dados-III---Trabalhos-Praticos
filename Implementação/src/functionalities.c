
/*
                                TRABALHO PRÁTICO - ESTRUTURA DE DADOS III

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

    DATA_DREG *data_register = pull_reg_from_memory(byte_offset, data_file);  // Puxa o registro para memória

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

long WHERE_PESSOA(FILE *data_file, FILE *index_file, const char* index_filename, char *type, char *value, long current_byte, long final_byte)
{
    fseek(data_file, current_byte, SEEK_SET);                               // Coloca o cursor do arquivo no byte atual

    long int start_curr_byte = current_byte;                                // Variável auxiliar para saber se já encontrou alguma pessoa

    while(current_byte < final_byte)                                        // Atua durante todo o arquivo
    {
        DATA_DREG* d_dreg = pull_reg_from_memory(current_byte,data_file);   // Puxa o registro do byte atual para a memória primária

        int null_flag = strcmp("NULO",value);                               // Flag para saber se o valor desejado é nulo

        if (d_dreg->removido != '1')                                        // Verifica se o registro não está logicamente removido
        {   
            if(strcmp(type,"idPessoa") == 0)                                // Caso procure por 'idPessoa'
            {
                if(current_byte != DF_HEAD_REG_LEN || strcmp("NULO",value) == 0)       
                {
                    return final_byte;                                      // Retorna o fim do arquivo caso seja nulo ou ja tenha encontrao o primeiro registro
                }
                
                if(index_file == NULL)                                      // Abre o arquivo de indíce se necessário
                {
                    char* index_filepath = get_file_path(index_filename);
                    index_file = fopen(index_filepath,"rb");
                    free(index_filepath);
                }

                INDEX_ARR* idx_arr = save_index_in_mem(index_file);         // Puxa o arquivo de índice para memória primária
                int pos = index_binary_search(idx_arr,atoi(value));         // Faz uma busca binária para o 'idPessoa' desejado

                if(pos == -1)                                               // Retorna o fim do arquivo caso não encontre
                {
                    destroy_index_arr(idx_arr);
                    return final_byte;
                }
                else                                                        // Retorna o byteoffset do arquivo 'pessoa' caso encontre
                {
                    int byteoffset = idx_arr->idx_arr[pos].byteOffset;
                    destroy_index_arr(idx_arr);
                    return byteoffset;
                }
            }

            else if(strcmp(type,"idadePessoa") == 0)                        // Caso procure por 'idadePessoa'
            {   
                int val = -1;
                if(null_flag != 0){val = atoi(value);}                      // Caso seja diferente de nulo, atribui o valor da idade a val
                if(val == d_dreg->idadePessoa){return current_byte;}        // Retorna o offset atual caso as idades coincidam
            }

            else if(strcmp(type,"nomePessoa") == 0)                         // Caso busque pelo 'nomePessoa', retorna o byte offset se o valor do campo coincidir
            {
                if(d_dreg->nomePessoa == NULL)
                {
                    if(null_flag == 0){return current_byte;}                
                }
                else
                {
                    if(strcmp(d_dreg->nomePessoa,value) == 0){return current_byte;}
                }
            }

            else if(strcmp(type,"nomeUsuario") == 0)                        // Caso busque pelo campo 'nomeUsuario', retorna o byte offset se o valor do campo coincidir    
            {
                if(start_curr_byte != DF_HEAD_REG_LEN){return final_byte;}  // Como o campo é único, caso já tenha encontrado alguma pessoa coincidente, retorna o fim do arquivo
                else
                    {
                        if(d_dreg->nomeUsuario == NULL)
                        {
                            if(null_flag == 0){return current_byte;}
                        }
                        else
                        {
                            if(strcmp(d_dreg->nomeUsuario,value) == 0){return current_byte;}
                        }
                    }
            }
        }
        current_byte += d_dreg->tamReg + 5;                                 // Passa para o próximo registro
        destroy_data_dreg(d_dreg);                                          // Libera a memória do registro carregado anteriormente
    }
    
    return final_byte;                                                      // Caso nada seja encontrado, retorna o fim do arquivo
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

        char** type_and_value = read_for_search();

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
            current_byte = WHERE_PESSOA(data_file, index_file, index_filename,
                           type_and_value[1], type_and_value[2], current_byte, final_byte);
            
            if(current_byte >= final_byte) //Caso o byte atual chegue no final para a busca
            {
                break;
            }

            int size = SELECT(data_file, current_byte, &no_register); //printa o registro encontrado
            current_byte = current_byte + size;
        }

        if(no_register)    //Caso não encontre nenhum registro não removido
        {
            printf("Registro inexistente.\n\n");
        }

        // Desaloca memória
        destroy_strip_matrix(type_and_value);
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

void DELETE_FROM_WHERE(char *data_filename, char *index_filename, int delete_number)
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
        char** type_and_value = read_for_search();

        end_string_on_mark(type_and_value[1], "\n"); // retira o '\n' e o '\r' das strings
        end_string_on_mark(type_and_value[2], "\r"); // lidas

        long final_byte;                     // Coloca o cursor de arquivos de dados no byte offset
        fseek(data_file, 9, SEEK_SET);       // do cabeçalho contendo próximo byte offset livre
        fread(&final_byte, 8, 1, data_file); // Pega o próximo byte offset livre (fim do arquivo)

        long current_byte = DF_HEAD_REG_LEN;         // coloca byte atual logo após o cabeçalho
        fseek(data_file, DF_HEAD_REG_LEN, SEEK_SET); // coloca o cursor logo após o cabeçalho

        while(1)
        {
            // Procura o primeiro byte offset desejado depois do byte_atual
            current_byte = WHERE_PESSOA(data_file, index_file, index_filename,
                           type_and_value[1], type_and_value[2], current_byte, final_byte);
            
            if(current_byte >= final_byte) //Caso o byte atual chegue no final para a busca
            {
                break;
            }

            fseek(data_file, current_byte, SEEK_SET);
            fwrite(&removed, 1, 1, data_file); // Marca como logicamente removido o registro encontrado
            
            int size;
            fread(&size, 4, 1, data_file);  // Pega o campo "Tamanho_registro"
            
            
            int id;
            fread(&id, 4, 1, data_file);    // Pega o campo "id"
            remove_id_array(idx_array, id); // Retira o ID achado do indice
            
            current_byte = current_byte + size + 5; // Próxima pesquisa começa a partir do 
                                                    // próximo registro

            num_people--;   //Diminui o número de pesssoas e aumenta o número de removidos
            num_removed++;
        }

        // Desaloca memória
        destroy_strip_matrix(type_and_value);
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

        char** values = strip_by_delim(str_in, ',',0);
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
            new_data_reg->nomePessoa = (char*)calloc(strlen(values[2]) + 1, 1);
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
        new_data_reg->nomeUsuario = (char*)calloc(strlen(values[4]) + 1, 1);
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

/*
    Recebe o número de updates a serem feitas e o nome do arquivo de dados,
    abre-o para leitura e escrita, caso exista. O usuário informa o valore
    e o campo do registro a ser buscado e a ser alterado.

    params:
        const char* data_filename => nome do arquivo de dados a ser aberto
        const char* index_filename => nome do arquivo de index caso seja nescessário
        int update_number => número de atualizações a serem feitas

    return:
        void

*/

void UPDATE_SET_WHERE(char* data_filename, char *index_filename, int update_number)
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

    FILE* index_file = NULL;

    fseek(data_file, 5, SEEK_SET);
    int num_removed;                        // Le do cabeçalho o número de pessoas
    fread(&num_removed, 4, 1, data_file);
  
    fseek(data_file, 9, SEEK_SET);  // Posição do final byte no cabeçalho
    long final_byte = 0;
    fread(&final_byte, 8, 1, data_file);

    for(int i = 0; i < update_number; i++)
    {
        char str_in[100];
        fgets(str_in, 100, stdin);

        char** first_strip = strip_by_delim(str_in, '=',0);   // Separa a string por '='
        
        char** second_strip = strip_by_delim(first_strip[2], ' ',0); //Separa a string por ' '

        char* search[2];
        char* update[2];

        if(atoi(second_strip[0]) > 2)   //Caso tenha espaços no valor do primeiro campo
        {
            destroy_strip_matrix(second_strip);
            second_strip = strip_by_delim(first_strip[2], 34,0);    //Separa por aspas, pois é string
            
            search[0] = first_strip[1];
            remove_everychar_until_space(search[0]);    // Campo a ser buscado sem o número do busca
            search[1] = second_strip[2];    // second_strip[1] é vazia, não hà caracteres antes das aspas

            update[0] = second_strip[3];    
            remove_everychar_until_space(update[0]);    //retira o espaço entre o valor de busca e o campo a ser atualizado 
            update[1] = first_strip[3];
            remove_quotes(update[1]);
            end_string_on_mark(update[1], "\n");
            end_string_on_mark(update[1], "\r");
        }
        else
        {   
            search[0] = first_strip[1];
            remove_everychar_until_space(search[0]);    //Campo a ser buscado sem o número da busca
            search[1] = second_strip[1];
            remove_quotes(search[1]);   //Valor do campo sem aspas

            update[0] = second_strip[2];    //Campo a ser alterado
            update[1] = first_strip[3];
            remove_quotes(update[1]);
            end_string_on_mark(update[1], "\n");
            end_string_on_mark(update[1], "\r");    // Valor do campo a ser alterado sem aspas
        }

        long current_byte = DF_HEAD_REG_LEN;

        INDEX_ARR* idx_array = NULL;
        
        while (current_byte < final_byte)
        {
            current_byte = WHERE_PESSOA(data_file, index_file, index_filename,
                                 search[0], search[1], current_byte, final_byte);

            if(current_byte >= final_byte)
            {
                break;
            }

            DATA_DREG* reg = pull_reg_from_memory(current_byte, data_file);   //Pega um registro que satisfez a busca

            int next_byte = reg->tamReg + 5 + current_byte;

            if(strcmp(update[0], "idPessoa") == 0)
            {   
                //Abre o arquivo de indice e puxa o indice para memória primária
                open_and_pull_index(&index_file, &idx_array, index_filename);

                //tira o ID do array
                remove_id_array(idx_array, reg->idPessoa);

                int id = atoi(update[1]);
                fseek(data_file, current_byte + 5, SEEK_SET);
                fwrite(&id, 4, 1, data_file);

                // COloca o novo ID no array e ordena
                add_id_array(&idx_array, id, current_byte);
                order_index(idx_array);
            }
            else if(strcmp(update[0], "idadePessoa") == 0) 
            {
                if(strcmp(update[1], "NULO") == 0)
                {
                    int idade = -1;
                    fseek(data_file, current_byte + 9, SEEK_SET);
                    fwrite(&idade, 4, 1, data_file);
                }
                else
                {
                    int idade = atoi(update[1]);
                    fseek(data_file, current_byte + 9, SEEK_SET);
                    fwrite(&idade, 4, 1, data_file);
                }
            }
            else if(strcmp(update[0], "nomePessoa") == 0)
            {
                int len = strlen(update[1]);
                
                if(strcmp(update[1], "NULO") == 0)
                {
                    int old_size_name = reg->tamReg - 16 - reg->tamNomeUsuario;
                    reg->tamNomePessoa = 0;

                    push_reg_to_memory(current_byte, data_file, reg);
                    
                    char trash = '$';   //Completa com lixo
                    for(int j = 0; j < old_size_name; j++)
                    {
                        fwrite(&trash, 1, 1, data_file);
                    }
                }
                else if(reg->tamReg < len + 16 + reg->tamNomeUsuario)    // Caso caiba não novo registro
                {
                    fseek(data_file, current_byte, SEEK_SET);   //Remove
                    char removed = '1';
                    fwrite(&removed, 1, 1, data_file);
                    num_removed++;
                    
                    reg->tamReg = reg->tamReg + len - reg->tamNomePessoa; //Atualiza o registro
                    reg->tamNomePessoa = len;
                    
                    free(reg->nomePessoa);
                    reg->nomePessoa = (char *)calloc(len + 1, 1);
                    strcpy(reg->nomePessoa, update[1]);

                    push_reg_to_memory(final_byte, data_file, reg); //Escreve no arquivo
                    
                    // A pessoa com esse ID tem seu byte_offset atualizado no indice
                    open_and_pull_index(&index_file, &idx_array, index_filename);
                    int pos = index_binary_search(idx_array, reg->idPessoa);
                    idx_array->idx_arr[pos].byteOffset = final_byte;

                    final_byte = final_byte + reg->tamReg + 5;
                }
                else
                {
                    int old_size_name = reg->tamReg - 16 - reg->tamNomeUsuario;
                    reg->tamNomePessoa = len;
                    if(reg->nomePessoa != NULL)
                    {
                        free(reg->nomePessoa);
                        reg->nomePessoa = NULL;
                    }
                    reg->nomePessoa = (char*)calloc(len + 1, 1);
                    strcpy(reg->nomePessoa, update[1]);

                    push_reg_to_memory(current_byte, data_file, reg);
                    
                    char trash = '$';   //Completa com lixo
                    for(int j = 0; j < old_size_name - len; j++)
                    {
                        fwrite(&trash, 1, 1, data_file);
                    }
                }
            }
            else if(strcmp(update[0], "nomeUsuario") == 0)
            {
                int len = strlen(update[1]);
                
                if(reg->tamReg < 16 + len + reg->tamNomePessoa)
                {
                    fseek(data_file, current_byte, SEEK_SET);
                    char removed = '1';
                    fwrite(&removed, 1, 1, data_file);
                    num_removed++;
                    
                    reg->tamReg = reg->tamReg + len - reg->tamNomeUsuario;
                    reg->tamNomeUsuario = len;
                    
                    free(reg->nomeUsuario);
                    reg->nomeUsuario = (char *)calloc(len, 1);
                    strcpy(reg->nomeUsuario, update[1]);

                    push_reg_to_memory(final_byte, data_file, reg);

                    open_and_pull_index(&index_file, &idx_array, index_filename);
                    int pos = index_binary_search(idx_array, reg->idPessoa);
                    idx_array->idx_arr[pos].byteOffset = final_byte;
                    final_byte = final_byte + reg->tamReg + 5;
                }
                else
                {
                    int old_size_name = reg->tamReg - 16 - reg->tamNomePessoa;
                    reg->tamNomeUsuario = len;
                    if(reg->nomeUsuario != NULL)
                    {
                        free(reg->nomeUsuario);
                        reg->nomeUsuario = NULL;
                    }
                    reg->nomeUsuario = (char*)calloc(len, 1);
                    strcpy(reg->nomeUsuario, update[1]);

                    push_reg_to_memory(current_byte, data_file, reg);
                    
                    char trash = '$';
                    for(int j = 0; j < old_size_name - len; j++)
                    {
                        fwrite(&trash, 1, 1, data_file);
                    }
                }
            }
            else
            {
                exit(EXIT_FAILURE);
            }
            
            destroy_data_dreg(reg);
            current_byte = next_byte;
        }
        
        destroy_strip_matrix(first_strip);
        destroy_strip_matrix(second_strip);

        if(idx_array != NULL)
        {
            write_on_index_file(index_file, idx_array);
            destroy_index_arr(idx_array);
        }
    }

    if(index_file != NULL)
    {
        update_file_status(index_file, '1'); //atualiza o status do arquivo para consistente
        fclose(index_file);
    }

    update_file_status(data_file, '1'); //atualiza o status do arquivo para consistente
    fseek(data_file, 5, SEEK_SET);  // Pula para a parte do cabeçalho do num_pessoas removidas
    fwrite(&num_removed, 4, 1, data_file);   //atualiza o cabeçalho
    fwrite(&final_byte, 8, 1, data_file);

    fclose(data_file);

    binarioNaTela(data_filename);
    binarioNaTela(index_filename);
}

/*
    Lê os dados de um arquivo fonte, grava num arquivo de dados "follow file"

    params:
        const char* csv_filename => Nome do arquivo fonte dos dados (já existente)
        const char* follow_filename => Nome do arquivo binário gerado

    return:
        void
*/

void CREATE_FOLLOW_TABLE(char* csv_filename, char* follow_filename)
{
    char* csv_filepath = get_file_path(csv_filename);
    FILE* csv_file = fopen(csv_filepath, "r");
    free(csv_filepath);
    
    if(csv_file == NULL)
    {
        print_error();
        exit(EXIT_SUCCESS);
    }

    FOLLOW_ARR* follow_arr = load_follow_csv_into_array(csv_file);

    fclose(csv_file);

    char* follow_filepath = get_file_path(follow_filename);
    FILE* follow_file = fopen(follow_filepath, "wb");
    free(follow_filepath);

    if(follow_file == NULL)
    {
        print_error();
        destroy_follow_array(follow_arr);
        exit(EXIT_FAILURE);
    }
    
    write_on_follow_file(follow_file, follow_arr);

    update_file_status(follow_file, '1');

    destroy_follow_array(follow_arr);
    fclose(follow_file);

    binarioNaTela(follow_filename);
}


/*
    Lê do arquivo 'segue' e ordena seus registros de forma crescente, seguindo a hierarquia:
        idPessoaQueSegue -> idPessoaQueESeguida -> dataInicioQueSegue -> dataFimQueSegue
    
    params:
        const char* src_filename => Nome do arquivo que deseja-se ordenar os registros;
        const char* ord_dest_filename => Nome do arquivo que receberá os registros ordenados;
    
    return:
        void.
*/
void ORDER_BY(const char* src_filename, const char* ord_dest_filename)
{
    char* src_filepath = get_file_path(src_filename);                       // Pega o caminho dos arquivos
    char* ord_dest_filepath = get_file_path(ord_dest_filename);

    FILE* src_file = fopen(src_filepath,"rb");                              // Abre para leitura o arquivo fonte
    if(src_file == NULL)
    {
        if(DEBUG)
        {
            printf("Arquivo %s inexistente\n",src_filepath);
        }
        print_error();                                                      // Exibe mensagem de erro e sai do programa caso não exista
        exit(EXIT_FAILURE);
    }

    FILE* ord_dest_file = fopen(ord_dest_filepath,"wb");                    // Abre o arquiv destino para escrita
    if(ord_dest_file == NULL)
    {
        if(DEBUG)
        {
            printf("Impossível de criar arquivo %s\n",ord_dest_filepath);
        }
        print_error();                                                      // Caso não seja criado, exibe mensagem de erro e encerra o programa
        exit(EXIT_FAILURE);
    }

    update_file_status(ord_dest_file,'0');                                  // Atualiza o status do arquivo destino para '0' (inconsistente)

    free(src_filepath);                                                     // Libera o espaço da string do arquivo fonte
    
    FOLLOW_ARR* f_arr = read_follow_file(src_file);                         // Lê do arquivo fonte e o coloca em memória primária

    fclose(src_file);                                                       // Fecha o arquivo fonte
    
    ordenate_follow_dreg(f_arr);                                            // Ordena os registros do arquivo

    write_on_follow_file(ord_dest_file,f_arr);                              // Escreve os registros no arquivo de destino, já ordenados

    destroy_follow_array(f_arr);                                            // Libera o espaço alocado para os registros em memória primária

    update_file_status(ord_dest_file,'1');                                  // Atualiza o status do arquivo de destino para '1' (consistente)                     
    fclose(ord_dest_file);                                                  // Fecha o arquivo de destino

    binarioNaTela(ord_dest_filepath);                                      
    free(ord_dest_filepath);                                                // Usa binarioNaTela e libera a memória da string com o caminho do arquivo de destino

}

/*
    Implementa a funcionalidade SELECT_FROM_JOIN_ON -> Imprime todos os registros do arquivo follow relacionados
    às pessoas com 'idPessoa' no arquivo do tipo 'pessoa' igual à 'idPessoaQueSegue' no arquivo do tio 'segue'

    params:
        const char* data_filename => Nome do arquivo do tipo 'pessoa'
        const char* index_filename => Nome do arquivo de índice
        const char* follow_filename => Nome do arquivo do tipo 'segue'
        const int search_number => Número de buscas a serem feitas no arquivo do tipo 'pessoa'

    return:
        void;
*/
void SELECT_FROM_JOIN_ON(const char* data_filename, const char* index_filename, const char* follow_filename, const int search_number)
{
    char* data_filepath = get_file_path(data_filename);                     // Tenta abrir todos os arquivos para leitura e encerra o programa com uma mensagem de erro caso estes não existam
    FILE* data_file = fopen(data_filepath,"rb");
    if(data_file == NULL)
    {
        if(DEBUG){printf("Arquivo %s não encontrado !",data_filepath);}
        print_error();
        return;
    }
    free(data_filepath);
    
    char* index_filepath = get_file_path(index_filename);
    FILE* index_file = fopen(index_filepath,"rb");
    if(index_file == NULL)
    {
        if(DEBUG){printf("Arquivo %s não encontrado !",index_filepath);}
        print_error();
        return;
    }
    free(index_filepath);

    char* follow_filepath = get_file_path(follow_filename);
    FILE* follow_file = fopen(follow_filepath,"rb");
    if(follow_file == NULL)
    {
        if(DEBUG){printf("Arquivo %s não encontrado !",follow_filepath);}
        print_error();
        return;
    }
    free(follow_filepath);

    fseek(data_file,9,SEEK_SET);                                            // Move o ponteiro do arquivo para o campo 'proxByteOffset'

    long int final_byte;
    fread(&final_byte,8,1,data_file);                                       // Lê os 8 bytes que correspondem ao fim do arquivo

    FOLLOW_ARR* f_arr = read_follow_file(follow_file);                      // Carrega o arquivo do tipo 'segue' para memória primária

    for(int i = 0; i < search_number; i++)                                  // Atua enquanto houverem busacas à ser feitas
    {
        char** type_and_value = read_for_search();                          // Lê a strign na forma 'n type=value'

        remove_quotes(type_and_value[2]);                                   // Remove as aspas do valor, se houver

        end_string_on_mark(type_and_value[2], "\n");                        // Retira o '\n' e o '\r' das strings lidas
        end_string_on_mark(type_and_value[2], "\r");
        
        long int curr_byte = DF_HEAD_REG_LEN;                               // Variável auxiliar para percorrer todo o arquivo, pula o registro de cabeçalho

        int no_reg = 1;                                                     // Flag para saber sealgum registro fora encontrado

        while (curr_byte < final_byte)                                      // Percorre todo o arquivo
        {
            curr_byte = WHERE_PESSOA(data_file,index_file,index_filename,type_and_value[1],type_and_value[2],curr_byte,final_byte); // Encontra o byte offset cujo registro do arquivo do tipo 'pessoa' satisfaz as condiçẽos da busca

            DATA_DREG* d_dreg =  pull_reg_from_memory(curr_byte,data_file); // Carrega o referido registro para a memória primária
            
            if(d_dreg->idPessoa == -1)                                      // Encerra o loop caso não encontre registro equivalente
            {   
                destroy_data_dreg(d_dreg);
                break;
            }

            int size = SELECT(data_file,curr_byte,&no_reg);                 // Exibe o registro e calcula seu tamanho

            if(!no_reg)                                                     // Executa somente se um registro fora encontrado
            {
                SELECT_WHERE_FOLLOW(f_arr,d_dreg->idPessoa);                // Exibe todos os registros do arquivo do tipo 'segue' cujo 'idPessoaQueSegue' é igual à 'idPessoa' do registro encontrado anteriormente
            }

            curr_byte += size;                                              // Incrementa o byte offset atual

            destroy_data_dreg(d_dreg);                                      // Libear a memória do registro do arquivo 'pessoa'

            puts("");                                                       // Imprime nova linha

        }

        destroy_strip_matrix(type_and_value);                               // Libera a memória da matriz de tipo e valor
        if(no_reg){printf("Registro inexsitente.\n");}                      // Exibe a mensagem caso nenhum registro seja encontrado

    }

    destroy_follow_array(f_arr);                                            // Libera a memória do arquivo do tipo 'segue'

    fclose(data_file);                                                      // Fecha todos os arquivos
    fclose(index_file);
    fclose(follow_file);    
}