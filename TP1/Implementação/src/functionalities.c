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
    FILE* index_file = create_index_file(index_filename);

    fseek(index_file,0,SEEK_SET);

    char status = '1';

    fwrite(&status,1,1,index_file);

    fclose(index_file);
}

void CREATE_TABLE(const char* src_filename, const char* output_filename, const char* index_filename)
{
    char* src_path = get_file_path(src_filename);
    FILE* src_file = fopen(src_path,"r");
    if(src_file == NULL)
    {
        print_error();
        return;
    }

    FILE* output_file = create_data_file(output_filename);

    char* index_path = get_file_path(index_filename);
    FILE* index_file = fopen(index_path,"rb+");
    if(index_file == NULL)
    {
        print_error();
    }

    DATA_LIST* dlist = fill_data_file(src_file,output_file);

    
}