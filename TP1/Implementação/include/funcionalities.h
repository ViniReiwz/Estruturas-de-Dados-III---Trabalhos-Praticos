#ifndef FUNCTIONALITIES_H
#define FUNCTIONALITIES_H

#include "all.h"

/*
    Cria um arquivo de índice como especificado, atualizando o status quando necessário

    params:
        const char* index_filename => Nome do arquivo de índice a ser criado
    
    return:
        void
*/
void CREATE_INDEX(const char* index_filename);

void CREATE_TABLE(const char* src_filename, const char* output_filename, const char* index_filename);

#endif