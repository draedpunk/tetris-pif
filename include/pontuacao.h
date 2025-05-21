#ifndef _PONTUACAO_H
#define _PONTUACAO_H

// #include "tetraminos.h";
// #include "tetris.h";
// #include "mapa.h"

typedef struct{
    char nome[30];
    int pontuacao;
} Jogador;

#define TOP_MAIORES_PONT 5
#define LIMITE_JOGADORES 50

void atualizar_pontuacao(int *pontuacao, int linhas, int eh_explosiva);
void receber_dados_pontuacao(Jogador pontos[], int *qtd_dados);
void colocar_ordem_cresc(Jogador pontos[], int qtd_dados);
void exibir_ranking();

#endif