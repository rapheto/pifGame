#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "screen.h"
#include "player.h" 

typedef struct {
    char nome[20];
    int pontos;
} Rank; //Struct criada para printar a pontuação na tela

void salvarPontuacao(){
    Player *playerPontos = getPlayer(); //Pega os dados do player 
    int pontuacao = playerPontos->pontos; //Cria a variável pontuação e atribui os pontos do player
    FILE *arquivo = fopen("cli-lib/pont/pontos.txt", "a"); //Abre o arquivo
    fprintf(arquivo, "%s %d\n", playerPontos->nickname, pontuacao); //Escreve o nick e a pontuação do jogador no arquivo
    fclose(arquivo); //Fecha o arquivo
}

//Compara quem é maior entre A e B e vai deixando as maiores pontuações
//Será usada no QSORT Linha 45
int compararRank(const void *a, const void *b) {
    const Rank *ra = (const Rank *)a;
    const Rank *rb = (const Rank *)b;
    return rb->pontos - ra->pontos;  // decrescente
}

void mostrarRanking() { //Função chamada no menu
    screenClear(); //Limpa a tela
    screenDrawBorders(); //Desenha a tela

    FILE *arquivo = fopen("cli-lib/pont/pontos.txt", "r"); //Abre o arquivo

    Rank ranks[100]; //Vai pegar apenas 100 pontuações
    int count = 0; //Contador

    while (fscanf(arquivo, "%19s %d", ranks[count].nome, &ranks[count].pontos) == 2) { //Vai escanear apenas as 100 primeiras pontuações
        count++;
        if (count >= 100) break; 
    }

    fclose(arquivo); //Fecha o arquivo

    // Ordena do maior para menor
    qsort(ranks, count, sizeof(Rank), compararRank);

    // Exibe top 10 ou menos
    screenGotoxy((MAXX/2) - 12, 5);
    printf("MELHORES");

    int topDez = count < 10 ? count : 10;
    for (int i = 0; i < topDez; i++) {  //Printa nome e pontuação
        screenGotoxy((MAXX/2) - 10, 7 + i);
        printf("%2d. %s - %d", i + 1, ranks[i].nome, ranks[i].pontos); 
    }

    //Botão de saída
    screenGotoxy((MAXX/2) - 15, MAXY - 2);
    printf("Pressione ENTER para voltar");
    getchar();
}