#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "screen.h"
#include "player.h" 

typedef struct {
    char nome[20];
    int pontos;
} Rank;

void salvarPontuacao(){
    Player *playerPontos = getPlayer();
    int pontuacao = playerPontos->pontos;
    FILE *arquivo = fopen("cli-lib/pont/pontos.txt", "a");
    fprintf(arquivo, "%s %d\n", playerPontos->nickname, pontuacao);
    fclose(arquivo);
}

int compararRank(const void *a, const void *b) {
    const Rank *ra = (const Rank *)a;
    const Rank *rb = (const Rank *)b;
    return rb->pontos - ra->pontos;  // decrescente
}

void mostrarRanking() {
    screenClear();
    screenDrawBorders();
    FILE *arquivo = fopen("cli-lib/pont/pontos.txt", "r");

    Rank ranks[100]; 
    int count = 0;

    while (fscanf(arquivo, "%19s %d", ranks[count].nome, &ranks[count].pontos) == 2) {
        count++;
        if (count >= 100) break; 
    }

    fclose(arquivo);

    // Ordena do maior para menor
    qsort(ranks, count, sizeof(Rank), compararRank);

    // Exibe top 10 ou menos
    screenGotoxy((MAXX/2) - 10, 5);
    printf("10 MELHORES");

    int to_show = count < 10 ? count : 10;

    for (int i = 0; i < to_show; i++) {
        screenGotoxy((MAXX/2) - 10, 7 + i);
        printf("%2d. %s - %d", i + 1, ranks[i].nome, ranks[i].pontos);
    }

    screenGotoxy((MAXX/2) - 15, MAXY - 2);
    printf("Pressione ENTER para voltar");
    getchar();
}