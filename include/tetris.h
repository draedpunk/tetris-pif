#ifndef _TETRIS_H_
#define _TETRIS_H_

#define TAMANHO_MAX_LINHAS 24
#define TAMANHO_MAX_COLUNAS 40

typedef struct {
    int linhas;
    int colunas;
    char** matriz; 
} MAPA;


void alocar_mapa(MAPA* t);
void ler_mapa(MAPA *t);
void imprimir_mapa(MAPA* t);
void carregar_tetraminos();
int rotacionar(int x, int y, int rot);
int pode_encaixar(MAPA* mapa, int tetramino, int rot, int posX, int posY);
void ler_input(int tecla[4]);
void explodir(MAPA* t, int cx, int cy);
void desenhar_mapa_com_peca(MAPA* t, int tetramino_atual, int rotacao_atual, int x_atual, int y_atual);
int remover_linhas_completas(MAPA *t);
void inicializar_mapa(MAPA *t);
void posicionar_tetramino_no_mapa(MAPA *t, int tipo, int rot, int x, int y);
void atualizar_pontuacao(int *pontuacao, int linhas, int eh_explosiva);
void processar_input(int *teclas, int *x, int *y, int *rot, int *bRotateHold, MAPA *t, int tipo);
int verificar_game_over(MAPA *t, int tipo, int rot, int x, int y);

#endif