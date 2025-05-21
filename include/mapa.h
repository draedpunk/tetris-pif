#ifndef _MAPA_H_
#define _MAPA_H_

typedef struct {
    int linhas;
    int colunas;
    char** matriz; 
} MAPA;

#include "tetris.h"
#include "tetraminos.h"

#define TAMANHO_MAX_LINHAS 24
#define TAMANHO_MAX_COLUNAS 40
#define INICIO_X 6
#define INICIO_Y 4
#define LARGURA_JOGO 9
#define ALTURA_JOGO 18

void alocar_mapa(MAPA* t);
void ler_mapa(MAPA *t);
void imprimir_mapa(MAPA* t);
void desenhar_mapa_com_peca(MAPA* t, int tetramino_atual, int rotacao_atual, int x_atual, int y_atual);
void inicializar_mapa(MAPA *t);

#endif