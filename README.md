# CESAR SCHOOL & EMBARQUE DIGITAL


## Jogo criado para as disciplinas de Programação Imperativa Funcional e Lógica para computação - 2025.1


    #      ##    ###  ###   ###   ##        ###   ###  ###  ###  #      ##    ##
    #     #  #  #      #   #     #  #      #     #      #  #     #     #  #  #  #
    #     #  #  # ##   #   #     #  #      ###   ####   #  ####  #     #  #  ### 
    #     #  #  #  #   #   #     ####      #        #   #  #     #     ####  #  #
     ###   ##    ###  ###   ###  #  #       ###  ###    #   ###   ###  #  #  #  #


## Equipe:

    Alessandra Barbosa @alebarbosas,

    Raphael Miranda @rapheto,

    Rayane Cavalcanti @Rayane32,
    
    Samuel Araujo @iwannaendme.


## Game Overview
    Lógica Estelar é um jogo no estilo Space Invaders onde o objetivo principal é ensinar 
    conceitos de lógica matemática de forma interativa e divertida para os alunos.

    Foi desenvolvido usando como base a biblioteca cli-lib, que serve para o
    desenvolvimento de aplicações e jogos em C.




## Objetivos
    O desafio é sobreviver o maior tempo possível, tomando decisões rápidas com base no
    raciocínio lógico, enquanto os obstáculos descem em direção à nave. Assim, o jogo
    combina habilidade, reflexo e aprendizado, reforçando o entendimento de expressões
    lógicas na prática.

## Mecânica


### Controles
    Teclas A - D: Movimenta o jogador
    Barra de espaço: Atira


### Pontuação
    O jogador pontua quando acerta a resposta da proposição lógica.
    Acertar: +100 pontos
    Errar: -50 pontos


## Como Jogar?


### Executando
    Abra seu terminal e vá até a pasta pifGame
    Em pifGame use o comando:
        $ gdb logica-estelar
        (gdb)run


#### Compilando (Caso seja necessário)
    Caso o aquivo logica-estela não esteja na pasta pifGame
    Abra seu terminal e vá até a pasta cli-lib
    Em cli-lib use o comando:
        $ gcc ./cli-lib/src/*.c -I ./cli-lib/include -o logica-estelar


#### Especificações
    Essa biblioteca funciona com os seguintes sistemas:
        - Linux based (Ubuntu, etc)
        - MacOS
    -É necessário ter o GCC instalado.