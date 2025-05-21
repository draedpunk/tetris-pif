#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#include "tetris.h"
#include "tetraminos.h"

void alocar_mapa(MAPA* t) {
    t->matriz = malloc(t->linhas * sizeof(char*));

    for (int i = 0; i < t->linhas; i++) {
        t->matriz[i] = malloc((t->colunas + 1) * sizeof(char));
        for (int j = 0; j < t->colunas + 1; j++) {
            t->matriz[i][j] = 0;
        }
    }
}

void ler_mapa(MAPA *t) {
    FILE *f = fopen("mapa.txt", "r");
    if (f == NULL) {
        printf("Erro ao abrir o mapa.\n");
        exit(1);
    }

    char linha[TAMANHO_MAX_COLUNAS + 2];
    int linhas = 0;
    int colunas = -1;

    char temp[TAMANHO_MAX_LINHAS][TAMANHO_MAX_COLUNAS];

    while (fgets(linha, sizeof(linha), f) != NULL) {
        linha[strcspn(linha, "\r\n")] = 0; 
        int len = strlen(linha);

        if (colunas == -1) colunas = len;
        if (len != colunas) {
            printf("Erro: mapa com colunas irregulares (linha %d).\n", linhas + 1);
            fclose(f);
            exit(1);
        }

        if (linhas >= TAMANHO_MAX_LINHAS) {
            printf("Erro: mapa excede o número máximo de linhas.\n");
            fclose(f);
            exit(1);
        }

        strcpy(temp[linhas], linha);
        linhas++;
    }

    fclose(f);

    t->linhas = linhas;
    t->colunas = colunas;
    alocar_mapa(t); 

    for (int i = 0; i < linhas; i++) {
        strcpy(t->matriz[i], temp[i]);
    }
    printf("Debug - First line of map: %s\n", t->matriz[0]);

}

void imprimir_mapa(MAPA* t) {
    for (int i = 0; i < t->linhas; i++) {
        screenGotoxy(INICIO_X, INICIO_Y + i);
        for (int j = 0; j < t->colunas; j++) {
            printf("%c", t->matriz[i][j]);
        }
    }
}

void desenhar_mapa_com_peca(MAPA* t, int tetramino_atual, int rotacao_atual, int x_atual, int y_atual) {
    for (int y = 0; y < t->linhas; y++) {
        for (int x = 0; x < t->colunas; x++) {
            screenGotoxy(INICIO_X + x, INICIO_Y + y);

            int caractere_peca = 0;

            int rel_x = x - x_atual;
            int rel_y = y - y_atual;
            if (rel_x >= 0 && rel_x < 4 && rel_y >= 0 && rel_y < 4) {
                int pi = rotacionar(rel_x, rel_y, rotacao_atual);
                if (tetraminos[tetramino_atual][pi] != '.') {
                    caractere_peca = 1;
                }
            }

            if (caractere_peca) {
                printf("%c", 'A' + tetramino_atual);
            } else {
                printf("%c", t->matriz[y][x]);
            }
        }
        printf("\n");
    }
}

void inicializar_mapa(MAPA *t) {
    for (int y = 0; y < t->linhas; y++) {
        for (int x = 0; x < t->colunas; x++) {
            t->matriz[y][x] = ' ';
        }
    }
}