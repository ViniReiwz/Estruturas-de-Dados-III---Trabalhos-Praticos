/*
                                TRABALHO PRÁTICO 1 - ESTRUTURA DE DADOS III
                                
                                Pedro Avelar Machado                15497396
                                Vinicius Reis Gonçalves             15491921
*/

//Arquivo 'utilidades.h', contém as definições das funções auxiliares fornecidas pelos avaliadores

#ifndef UTILS_H
#define UTILS_H

#include "all.h"

// Exibe os dados de um arquivo binário na stdout -> Ver arquivo 'utilidades.c' em src/ para mais informações
void binarioNaTela(char *nomeArquivoBinario);

// Lê uma string delimitada por ' " ' -> Ver arquivo 'utilidades.c' em src/ para mais informações
void scan_quote_string(char *str);

#endif