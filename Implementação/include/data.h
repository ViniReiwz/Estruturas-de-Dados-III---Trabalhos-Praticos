/*
                                TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
                                Vinicius Reis Gonçalves             15491921
*/

//Arquivo 'data.h', contém definiçãode funções, valores e tipos de dados relacionados ao arquivo de dados

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
    int tamNomeUsuario;     // Tamanho do nome de usuário em bytes
    struct _data_dreg* next;
    

}DATA_DREG;

typedef struct _data_hreg       // Registro de cabeçalho do arquivo de dados
{

    int qtdPessoas;             // Quantidade de pessoas (registros) no arquivo de dados
    int qtdRemovidos;           // Quantidade de pessoas (registros) removidos logicamente no arquivo de dados
    long int proxByteOffset;    // Próxima posição disponível para armazenamento de novo registro

}DATA_HREG;

typedef struct _data_dlist  // Lista de dados do arquivo de dados
{
    DATA_DREG* head;        // Cabeça da listsa (primeiro nó)
    DATA_DREG* tail;        // Cauda da lista (último nó)
    DATA_HREG* header_reg;  // Registro de cabeçalho ligado à lista
        
}DATA_LIST;


/*
    Cria um arquivo de dados e seu registro de cabeçalho

    params:
        const char* filename => Nome_do_arquivo.extensão (arquivo.bin, por exemplo)

    return:
        FILE* dlist => Ponteiro para a lista de dados emmemória primária
*/
DATA_LIST* create_data_list();

/*
    Cria um nó que representa um registro de cabeçalho do arquivo de dados em memória primária, de tamanho total 17 bytes

    return:
        DATA_HREG* dhreg => Retorna um endereço para uma variável do tipo 'registro de cabeçalho' do arquivo de dados
*/
DATA_HREG* create_data_hreg();

/*
    Cria um nó que representa um registro de dados no arquivo de dados

    return:
        DATA_DREG* ddreg => Retorna um endereço para uma variável do tipo 'registro de dados' do arquivo de dados
*/
DATA_DREG* create_data_dreg();


/*
    Libera a memória de um registro de dados do arquivo de dados em memória primária

    params:
        DATA_DREG* ddreg => Registro a ser liberado
    
    return:
        void
*/
void destroy_data_dreg(DATA_DREG* ddreg);

/*
    Libera a memória de um registro de cabeçalho do arquivo de dados em memória primária

    params:
        DATA_HREG* dhreg => Registro de cabeçalho cuja memória deve ser liberada
*/
void destroy_data_hreg(DATA_HREG* dhreg);

/*
    Destrói uma lista de registros de dados do arquivo de dados em memória primária

    params:
        DATA_LIST* dlist => Lista cuja memória deve ser liberada

    return:
        void
*/
void destroy_data_list(DATA_LIST* dlist);

/*
Atualiza o registro de cabeçalho do arquivo de dados

params:
DATA_HREG* d_hreg => Registro de cabeçalho em memória primária, contendo os dados corretos para atualizar
FILE* file => Ponteiro para o arquivo ao qual deseja atualizar o registro de cabeçalho

return:
FILE* old => Posição original do ponteiro 'file'
*/
void  update_dheader_reg(DATA_HREG* d_hreg, FILE* file);

/*
    Cria um novo arquivo de dados

    params:
        const char* filename => Nome do arquivo a ser criado

    return:
        FILE* data_file_p => Retorna um ponteiro para o início do arquivo de dados
*/
FILE* create_data_file(const char* filename);

/*
    Carrega os dados do arquivo fonte para a memória primária

    params:
        FILE* file => Ponteiro para arquivo fonte
        DATA_LIST* data_list => Lista de dados para qual deseja carregar o conteúdo

    return:
        void
*/
void load_csvfile_to_mem(FILE* file, DATA_LIST* data_list);

/*
    Escreve os itens da lista no arquivo de daados

    params:
        DATA_LIST* dlist => Lista onde estão presentes os dados a serem escritos
        FILE* file => Arquivo onde os dados devem ser escritos
    
    return:
        INDEX_ARR* idx => Arquivo de índice em memória primária;
*/
INDEX_ARR* write_on_data_file(DATA_LIST* dlist, FILE* file);

/*
    Preenche arquivo de dados e cria um arquivo de índice em memória primária

    params:
        const char* dest_filename => Arquivo de destino, o qual deseja-se preencher
        const char* src_filename => Arquivo donte, do qual os dados seram lidos
    
    return:
        INDEX_ARR* idx => Vetor contendo os registros de dados do arquivo de indíce relacionado ao arquivo de dados, já ordenado
*/
INDEX_ARR* fill_data_file(FILE* src_file, FILE* dest_file);

/*
    Printa um registro de dados para o usuário assim como foi pedido

    params:
        DATA_DREG data_register => registro de dados a ser printado
    
    return:
        void
*/

void print_data_register(DATA_DREG data_register);

/*
    Recebe um arquivo já aberto e puxa para a memória primária o conteúdo de um registro,

    params:
        const FILE* data_file => Arquivo fonte dos dados (já existente)
        const long byte_offset => Byte offset do registro

    return:
        DATA_DREG* data_register => registro puxado da memória

*/

DATA_DREG* pull_reg_to_memory(long byte_offset, FILE* data_file);

/*
    Recebe um arquivo já aberto e escreve da memória primária um registro no arquivo.

    params:
        const FILE* data_file => Arquivo fonte dos dados (já existente)
        const long byte_offset => byte offset onde deve ser escrito o registro
        DATA_DREG* data_register => registro a ser escrito

    return:
        void

*/

void push_reg_to_memory(long byte_offset, FILE* data_file, DATA_DREG *data_register);

#endif