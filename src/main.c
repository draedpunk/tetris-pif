#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

int posicao_x = 8 + (24 - 2 - 4)/2;
int posicao_y = 4;
int rotacao = 0;
int id_peca = 0;

char *tetraminos[7];

void gerar_tetraminos(){
    tetraminos[0] = "...."
                    "XXXX"
                    "...."
                    "...."; // I

    tetraminos[1] = "..X."
                    "..X."
                    ".XX."
                    "...."; // J

    tetraminos[2] = ".X.."
                    ".X.."
                    ".XX."
                    "...."; // L

    tetraminos[3] = "..X."
                    ".XX."
                    ".X.."
                    "...."; // T

    tetraminos[4] = "...."
                    ".XX."
                    ".XX."
                    "...."; // O

    tetraminos[5] = "..X."
                    ".XX."
                    ".X.."
                    "...."; // S

    tetraminos[6] = ".X.."
                    ".XX."
                    "..X."
                    "...."; // Z
}

int rotacionar_peca(int pX, int pY, int r){
    switch(r % 4){
        case 0: return (pY * 4 + pX);             // 0°
        case 1: return (12 + pY - (pX * 4));       // 90°
        case 2: return (15 - (pY * 4) - pX);       // 180°
        case 3: return (3 - pY + (pX * 4));        // 270°
    }
    return 0;
}

void limpar_peca(){
    for (int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            char bloco = tetraminos[id_peca][rotacionar_peca(x, y, rotacao)];
            if (bloco == 'X'){
                // screenGotoxy(posicao_x + x, posicao_y + y);
                // printf(" ");
                int desenhar_x = posicao_x + x;
                int desenhar_y = posicao_y + y;

                if (desenhar_x > 8 && desenhar_x < 31 && desenhar_y >= 4 && desenhar_y < 22) {
                    screenGotoxy(desenhar_x, desenhar_y);
                    printf(" ");
                }

            }
        }
    }
}

void desenhar_peca(){
    for (int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            char bloco = tetraminos[id_peca][rotacionar_peca(x, y, rotacao)];
            if (bloco == 'X'){
                // screenGotoxy(posicao_x + x, posicao_y + y);
                // printf("Z");
                int desenhar_x = posicao_x + x;
                int desenhar_y = posicao_y + y;

                if (desenhar_x > 8 && desenhar_x < 31 && desenhar_y >= 4 && desenhar_y < 22) {
                    screenGotoxy(desenhar_x, desenhar_y);
                    printf("P");
                }

            }
        }
    }
}

int checar_colisao_parede(char *peca, int px, int py, int r){
    int colidiu = 0;
    for (int x =0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            int i = rotacionar_peca(x, y, r);
            if(peca[i] == 'X'){
                int novo_x = px + x;
                int novo_y = py + y;

                if ((novo_x <= 8 || novo_x >= 31 || novo_y >= 22)){
                    colidiu =1;
                    return colidiu;
                }
            }

        }
    }
    return 0;
}

void area_jogo(){
    int largura = 24;
    int altura = 18;
    int inicioX = 8, inicioY = 4;

    for (int y = 0; y < altura; y++){
        for (int x = 0; x < largura; x++){
            screenGotoxy(inicioX + x, inicioY + y);

            if (x == 0 || x == largura - 1){
                printf("|");
            } else {
                printf(" ");
            }
        }
    }

    //base
    for (int x = 0; x < largura; x++){
        screenGotoxy(inicioX + x, inicioY + altura);
        printf("#");
    }
}

void banner_titulo(){
    int bannerX = (80 - 53) / 2;
    int bannerY = (24 - 6) / 2;

    screenGotoxy(bannerX, bannerY);
    printf(" _____ ______  _____  _____ ______  _____  _____ ");
    screenGotoxy(bannerX, bannerY + 1);
    printf("|_   _|| ___ \\|  ___||_   _|| ___ \\|_   _|/  ___|");
    screenGotoxy(bannerX, bannerY + 2);
    printf("  | |  | |_/ /| |__    | |  | |_/ /  | |  \\ `--. ");
    screenGotoxy(bannerX, bannerY + 3);
    printf("  | |  |    / |  __|   | |  |    /   | |   `--. \\");
    screenGotoxy(bannerX, bannerY + 4);
    printf("  | |  | |\\ \\ | |___   | |  | |\\ \\  _| |_ /\\__/ /");
    screenGotoxy(bannerX, bannerY + 5);
    printf("  \\_/  \\_| \\_|\\____/   \\_/  \\_| \\_| \\___/ \\____/ ");
}

void exibir_banner(){
    screenInit(1);
    banner_titulo();
    screenUpdate();
    sleep(2);
    system("clear");
}

int main(){
    exibir_banner();

    gerar_tetraminos();
    screenInit(1);
    keyboardInit();
    timerInit(300000);

    area_jogo();

    screenUpdate();

    while(1){
        limpar_peca();

        if (keyhit()){
            int tecla = readch();

            switch (tecla){
                case 'a': 
                    if (!checar_colisao_parede(tetraminos[id_peca], posicao_x - 1, posicao_y, rotacao))
                        posicao_x--; 
                    break;
                case 'd': 
                    if (!checar_colisao_parede(tetraminos[id_peca], posicao_x + 1, posicao_y, rotacao))
                        posicao_x++; 
                    break;
                case 's': 
                    if (!checar_colisao_parede(tetraminos[id_peca], posicao_x, posicao_y + 1, rotacao))
                        posicao_y++; 
                    break;
                case 'w':
                    if (!checar_colisao_parede(tetraminos[id_peca], posicao_x, posicao_y, rotacao + 1))
                        rotacao++; 
                    break;
            }

        }

        // gravidade
        if (timerTimeOver()){
            posicao_y++;
            timerInit(300000);
        }

    
        desenhar_peca();

        screenUpdate();
        usleep(50 * 1000);
    }

    screenDestroy();
    keyboardDestroy();
    timerDestroy();

    return 0;
}
