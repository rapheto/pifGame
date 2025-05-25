#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "player.h"
#include "ranking.h"
#include "main.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>

char nickname[20];
void nomeJogador(){
    screenClear();
    screenDrawBorders();
    screenGotoxy(5, 3);
    printf("Olá! Digite o seu nickname: ");
    if (fgets(nickname, sizeof(nickname), stdin)) {
        nickname[strcspn(nickname, "\n")] = '\0';
        initPlayer(nickname);
    }
}

void printLogo() {
    // Primeira linha
    screenGotoxy(11, 6);
    printf("#      ##    ###  ###   ###   ##        ###   ###  ###  ###  #      ##    ##");
    
    // Segunda linha
    screenGotoxy(11, 7);
    printf("#     #  #  #      #   #     #  #      #     #      #  #     #     #  #  #  #");
    
    // Terceira linha
    screenGotoxy(11, 8);
    printf("#     #  #  # ##   #   #     #  #      ###   ####   #  ####  #     #  #  ### ");
    
    // Quarta linha
    screenGotoxy(11, 9);
    printf("#     #  #  #  #   #   #     ####      #        #   #  #     #     ####  #  #");
    
    // Quinta linha
    screenGotoxy(11, 10);
    printf(" ###   ##    ###  ###   ###  #  #       ###  ###    #   ###   ###  #  #  #  #");
}

void menuOpcoes(){
    screenClear();
    screenDrawBorders();

    printLogo();

    screenGotoxy(40, 16);
    printf( "1 - Jogar" );
    screenGotoxy(40, 18);
    printf( "2 - Como Jogar?" );
    screenGotoxy(40, 20);
    printf ("3 - Ranking" );
    screenGotoxy(40, 22);
    printf ("4 - Créditos" );
    screenGotoxy(40, 24);
    printf ("5 - Sair" );

}

void animacaoNave() {
    const char* frames[] = {
        "<I-I>                             ",
        "        <I-I>                     ",
        "               <I-I>              ",
        "                      <I-I>       ",
        "                             <I-I>",
    };

    const char* framesBala[] = {
        "|",
        " ",
        "|",
        " ",
        "|",
    };

    int frameCount = sizeof(frames) / sizeof(frames[0]);
    int i = 0;
    int dir = 1;

    // Rodar um número fixo de ciclos, exemplo: 3 ciclos completos
    int ciclos = 0;
    while (ciclos < 20) {
        screenGotoxy(7, 11);
        printf("%s", frames[i]);

        screenGotoxy(23, 17);
        printf("%s", framesBala[i]);

        fflush(stdout);
        usleep(200000);

        i += dir;

        if (i == frameCount - 1 || i == 0) {
            dir = -dir;
            ciclos++;
        }

        if (keyhit()) {
            int tecla = readch();
            if (tecla == '\n') {
                menuOpcoes(); 
                break;
            }
        }
    }
    
}

void comoJogar() {
    screenClear();
    screenDrawBorders();

    screenGotoxy(7, 8);
    printf("  Use A  D para se mover a nave  ");

    screenGotoxy(7, 14);
    printf("Use a Barra de Espaço para atirar");
    screenGotoxy(21, 18);
    printf("<I-I>");

    screenGotoxy(55, 8);
    printf("Você receberá uma proposição aleatória");
    screenGotoxy(55, 11);
    printf("          Proposição: ¬F → F          ");

    screenGotoxy(55, 14);
    printf("   Atire na resposta correta V ou F   ");
    screenGotoxy(55, 16);
    printf("        V         F                F     ");
    screenGotoxy(55, 17);
    printf("      F                      V ");
    screenGotoxy(55, 18);
    printf("              F                     ");
    screenGotoxy(55, 19);
    printf("   F                  V          ");
    screenGotoxy(55, 20);
    printf("              | ");
    screenGotoxy(55, 22);
    printf("            <I-I>");
    

    screenGotoxy((MAXX/2) - 15, MAXY - 2);
    printf("Pressione ENTER para voltar");
    animacaoNave();
    getchar();

}

void telaCreditos(){
    screenClear();
    screenDrawBorders();

    screenGotoxy(45, 5);
    printf("Criado por");

    screenGotoxy(40, 9);
    printf( "Alessandra Barbosa" );
    screenGotoxy(40, 12);
    printf( "Raphael Miranda" );
    screenGotoxy(40, 15);
    printf ("Rayane Cavalcanti");
    screenGotoxy(40, 18);
    printf ("Samuel Pereira");
    
    screenGotoxy((MAXX/2) - 15, MAXY - 2);
    printf("Pressione ENTER para voltar");
    getchar();
}

void quit(){
    screenClear();
    screenDrawBorders();
    screenGotoxy((MAXX/2)-5, (MAXY/2));
    printf("Saindo...");
    screenUpdate();
    sleep(1);
    screenClear();
    exit(0);
}

void telaDerrota(){
    Player* p = getPlayer();
    screenClear();
    screenDrawBorders();
    while (1)
    {
        screenGotoxy(31, 12);
        printf("###    ###   ##    ##    ##   ###   ##");
        
        // Segunda linha
        screenGotoxy(31, 13);
        printf("#  #  #     #  #  #  #  #  #   #   #  #");
        
        // Terceira linha
        screenGotoxy(31, 14);
        printf("#  #  ####  ###   ###   #  #   #   ####");
        
        // Quarta linha
        screenGotoxy(31, 15);
        printf("#  #  #     #  #  #  #  #  #   #   #  #");
        
        // Quinta linha
        screenGotoxy(31, 16);
        printf("###    ###  #  #  #  #   ##    #   #  #");

        screenGotoxy(34, 20);
        printf( "%s sua pontuação foi: %4d", p->nickname, p->pontos);
        screenGotoxy(MAXX/2 - 13, 32);
        printf("ESPAÇO para jogar novamente");
        screenGotoxy(MAXX/2 - 11, 33);
        printf("ENTER para ir ao menu");

        if (keyhit()) {
            int ch = readch();
            if (ch == 32) {  // Tecla espaço
                inicializarJogo();
                break;
            }
            else if(ch == 10){
                menuOpcoes();
                break;
            }
        }
    } 
}

int iniciarMenu() {
    nomeJogador();

    screenInit(1);
    keyboardInit();

    while (1) {

        menuOpcoes();
        int c = getchar();

        switch(c) {
            case '1': 
                inicializarJogo();
                break;
            case '2': 
                comoJogar(); 
                break;
            case '3':
                mostrarRanking();
                getchar();
                break;
            case '4': 
                telaCreditos(); 
                break;
            case '5':
                quit();
                break;
                
        }
    }
    return 0;
}
