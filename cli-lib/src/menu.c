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

//Função para inserir o nick do jogador
char nickname[20];
void nomeJogador(){
    screenClear(); //Limpa a tela
    screenDrawBorders(); //Desenha a tela

    screenGotoxy(5, 3); //Vai para a posição

    printf("Olá! Digite o seu nickname: ");
    if (fgets(nickname, sizeof(nickname), stdin)) { //Fgets == Scanf para entrar o nome do jogador
        nickname[strcspn(nickname, "\n")] = '\0';
        initPlayer(nickname); //Chama a função de player para salvar o nickname
    }
}

void printLogo() { //Printa a logo
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

//Menu inicial
void menuOpcoes(){
    screenClear(); //Limpa a tela
    screenDrawBorders(); //Desenha a tela

    printLogo(); //Logo

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

//Cria as animações
void animacaoNave() {
    const char* frames[] = { //Cria um vetor char para os "frames"
        "<I-I>                             ",
        "        <I-I>                     ",
        "               <I-I>              ",
        "                      <I-I>       ",
        "                             <I-I>",
    };

    const char* framesBala[] = { //Cria um vetor char para os "frames"
        "|",
        " ",
        "|",
        " ",
        "|",
    };

    int frameCount = sizeof(frames) / sizeof(frames[0]);
    int i = 0;
    //Define a direção dos frames (1 = Direita e -1 = Esquerda)
    int dir = 1;

    // Rodar um número fixo de ciclos
    int ciclos = 0;
    while (ciclos < 20) {
        screenGotoxy(7, 11);
        printf("%s", frames[i]); //Printa a nave na tela e altera os frames de acordo com index[i]

        screenGotoxy(23, 17);
        printf("%s", framesBala[i]); //Printa a bala na tela e altera os frames de acordo com index[i]

        fflush(stdout); //Força a saída do buffer a ser imediatamente enviada para o terminal
        usleep(200000); //Faz o programa esperar 0,2 segundos para dar fluidez a animação

        i += dir;

        //Muda direção da animação e soma um ciclo
        if (i == frameCount - 1 || i == 0) {
            dir = -dir;
            ciclos++;
        }

        //Botão de saída
        if (keyhit()) {
            int tecla = readch();
            if (tecla == '\n') {
                menuOpcoes(); 
                break;
            }
        }
    }
    
}

//Tela de como jogar
void comoJogar() {
    screenClear(); //Limpa a tela
    screenDrawBorders(); //Desenha a tela

    //Movimentar
    screenGotoxy(7, 8);
    printf("  Use A  D para se mover a nave  ");

    //Atirar
    screenGotoxy(7, 14);
    printf("Use a Barra de Espaço para atirar");
    screenGotoxy(21, 18);
    printf("<I-I>");

    //Proposições
    screenGotoxy(55, 8);
    printf("Você receberá uma proposição aleatória");
    screenGotoxy(55, 11);
    printf("           Expressão: ¬F → F          ");

    //Interface do jogo
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
    

    //Botão de saída
    screenGotoxy((MAXX/2) - 15, MAXY - 2);
    printf("Pressione ENTER para voltar");
    animacaoNave(); //VCria as animações na tela
    getchar(); //Botão de saída

}

//Tela créditos
void telaCreditos(){
    screenClear(); //Limpa a tela
    screenDrawBorders(); //Desenha a tela

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
    
    //Botão de saída
    screenGotoxy((MAXX/2) - 15, MAXY - 2);
    printf("Pressione ENTER para voltar");
    getchar();
}

//Função de sair do jogo
void quit(){
    screenClear(); //Limpa a tela
    screenDrawBorders(); //Desenha a tela

    screenGotoxy((MAXX/2)-5, (MAXY/2));
    printf("Saindo...");

    screenUpdate(); //Atualiza a tela do terminal

    sleep(1); //Pausa o terminal por 1 segundo

    screenClear(); //Limpa a tela

    exit(0); //Função de sair do GDB / encerrar o jogo
}

//Tela de derrota
void telaDerrota(){
    screenClear(); //Limpa a tela
    screenDrawBorders(); //Desenha a tela

    Player* p = getPlayer(); //Pega os dados do player
    while (1)
    {
        //Escreve DERROTA na tela
        //Primeira linha
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
        //Fim do DERROTA

        screenGotoxy(34, 20);
        printf( "%s sua pontuação foi: %4d", p->nickname, p->pontos); //Mostra a pontuação do jogador

        //Printa botão de jogar novamente
        screenGotoxy(MAXX/2 - 13, 32);
        printf("ESPAÇO para jogar novamente");

        //Printa botão de ir para o menu
        screenGotoxy(MAXX/2 - 11, 33);
        printf("ENTER para ir ao menu");

        //Vai pegar as teclas pressionadas
        if (keyhit()) {
            int ch = readch();
            if (ch == 32) {  // Tecla espaço
                inicializarJogo(); //Joga novamente
                break;
            }
            else if(ch == 10){ //Enter
                menuOpcoes(); //Sai para o menu
                break;
            }
        }
    } 
}

//Funcionalidades do menu
int iniciarMenu() {
    //Começa com o nome do jogador
    nomeJogador();

    //Inicia a tela e o teclado
    screenInit(1);
    keyboardInit();

    while (1) {
        menuOpcoes(); //Printa as opções
        int c = getchar(); //Pega um char para o switch
        switch(c) {
            //Mostra na tela a opção escolhida
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
