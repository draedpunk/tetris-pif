#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "screen.h"
#include "timer.h"
#include "keyboard.h"
#include "tetris.h"
#include "ui.h"

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

void banner_gameover() {
    int bannerX = (80 - 59) / 2;
    int bannerY = (24 - 6) / 2;

    screenGotoxy(bannerX, bannerY);
    printf(" _____   ___  ___  ___ _____   _____  _   _  _____ ______ ");
    screenGotoxy(bannerX, bannerY + 1);
    printf("|  __ \\ / _ \\ |  \\/  ||  ___| |  _  || | | ||  ___|| ___ \\");
    screenGotoxy(bannerX, bannerY + 2);
    printf("| |  \\// /_\\ \\| .  . || |__   | | | || | | || |__  | |_/ /");
    screenGotoxy(bannerX, bannerY + 3);
    printf("| | __ |  _  || |\\/| ||  __|  | | | || | | ||  __| |    / ");
    screenGotoxy(bannerX, bannerY + 4);
    printf("| |_\\ \\| | | || |  | || |___  \\ \\_/ /\\ \\_/ /| |___ | |\\ \\ ");
    screenGotoxy(bannerX, bannerY + 5);
    printf(" \\____/\\_| |_/\\_|  |_/\\____/   \\___/  \\___/ \\____/ \\_| \\_|");
}

void exibir_banner_titulo() {
    screenClear();
    banner_titulo();
    screenUpdate();
    sleep(2);
    screenClear();
    screenUpdate();
}

void exibir_banner_gameover() {
    screenClear();
    banner_gameover();
    screenUpdate();
    sleep(2);
    screenClear();
    screenUpdate();
}


