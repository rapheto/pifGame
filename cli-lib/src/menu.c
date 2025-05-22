#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include "player.h"

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

void print_nome_jogo() {
    // Primeira linha
    screenGotoxy(11, 5);
    printf("#      ##    ###  ###   ###   ##        ###   ###  ###  ###  #      ##    ##");
    
    // Segunda linha
    screenGotoxy(11, 6);
    printf("#     #  #  #      #   #     #  #      #     #      #  #     #     #  #  #  #");
    
    // Terceira linha
    screenGotoxy(11, 7);
    printf("#     #  #  # ##   #   #     #  #      ###   ####   #  ####  #     #  #  ### ");
    
    // Quarta linha
    screenGotoxy(11, 8);
    printf("#     #  #  #  #   #   #     ####      #        #   #  #     #     ####  #  #");
    
    // Quinta linha
    screenGotoxy(11, 9);
    printf(" ###   ##    ###  ###   ###  #  #       ###  ###    #   ###   ###  #  #  #  #");
}


void menuOpcoes(){
    screenClear();
    screenDrawBorders();

    print_nome_jogo();
    screenGotoxy( (MAXX/2)-10, 14);
    printf( "Olá, %s!", nickname);

    screenGotoxy(35, 18);
    printf( "1 - Jogar" );
    screenGotoxy(35, 20);
    printf( "2 - Como Jogar?" );
    screenGotoxy(35, 22);
    printf ("3 - Sair" );

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
    

    screenGotoxy((MAXX/2) - 15, 27);
    printf("Pressione ENTER para voltar");
    animacaoNave();
    getchar();

}


int iniciar_menu() {
    nomeJogador();

    screenInit(1);
    keyboardInit();

    while (1) {

        menuOpcoes();
        int c = getchar();

        if (c == '1') {
            break;
        }
        else if (c == '2') {
            screenClear();
            screenDrawBorders();
            comoJogar(); 
        }
        else if (c == '3') {
            keyboardDestroy();
            screenDestroy();
            timerDestroy();

            return 0;
        }
    }
    return 0;
}
