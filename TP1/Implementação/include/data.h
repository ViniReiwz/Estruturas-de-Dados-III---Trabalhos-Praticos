#ifndef DATA_H
#define DATA_H

#include "all.h"

#define DF_HEAD_REG_LEN 17  // Tamanho do registro de cabeçalho do arquivo de dados em bytes


typedef struct _data_dreg    // Registro de dados do arquivo de dados
{
    char removido;          // Campo removido, pode receber '0' (não removido) ou '1' (removido) para indicar remoção lógica
    int idPessoa;           // Id da pessoa cadastrada nesse registro (4 bytes)
    int idadePessoa;        // Idade da pessoa cadastrada nesse  registro (4 bytes)
    char* nomePessoa;       // Nome da pessoa cadastrada no registro (tamanho variável)
    char* nomeUsuario;      // Nome de usuário da pessoa cadastrada no registro (tamanho variável)
    int tamReg;             // Tamanho final do registro
    int tamNomePessoa;      // Tamanho do nome da pessoa em bytes
    int tamNomeUsuário;     // Tamanho do nome de usuário em bytes
    struct _data_dreg* next;
    

}DATA_DREG;

typedef struct _data_hreg        // Registro de cabeçalho do arquivo de dados
{

    char status;                // Campo de status - Indica se o arquivo está sendo manipulado ('0') ou não ('1')
    int qtdPessoas;             // Quantidade de pessoas (registros) no arquivo de dados
    int qtdRemovidos;           // Quantidade de pessoas (registros) removidos logicamente no arquivo de dados
    long int proxByteOffset;    // Próxima posição disponível para armazenamento de novo registro

}DATA_HREG;

typedef struct _data_dlist
{
    DATA_DREG* head;
    DATA_DREG* tail;
        
}DATA_DLIST;




/*
    Cria um arquivo de dados e seu registro de cabeçalho

    params:
        const char* filename => Nome_do_arquivo.extensão (arquivo.bin, por exemplo)

    return:
        FILE* data_file_p => Ponteiro para o arquivo de dados
*/
FILE* create_data_file(const char* filename);


void fill_data_file(const char* filename_dest, const char* filename_src);

#endif