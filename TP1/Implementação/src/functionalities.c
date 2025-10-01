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
    return;
}