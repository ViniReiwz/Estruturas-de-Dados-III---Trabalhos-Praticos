
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

    binarioNaTela(output_filepath);
    binarioNaTela(index_path);

    free(index_path);                                       // Libera a memória da string do caminho do arquivo de índice
    free(output_filepath);                                  // Libera a memória da string do caminho de dados
}