/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
*/

#include <string.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

int x = 34, y = 12;
int incX = 1, incY = 1;

void printHello(int nextX, int nextY)
{
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(x, y);
    printf("           ");
    x = nextX;
    y = nextY;
    screenGotoxy(x, y);
    printf("Hello World");
}

void printKey(int ch)
{
    //screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(35, 22);
    printf("Key code :");

    screenGotoxy(34, 23);
    printf("            ");
    
    if (ch == 27) screenGotoxy(36, 23);
    else screenGotoxy(39, 23);

    printf("%d ", ch);
    while (keyhit())
    {
        printf("%d ", readch());
    }
}

void movimentacao(int ch){
    if (ch == 119)
    {
        screenSetColor(RED, DARKGRAY);
        printKey(ch);
        screenUpdate();
    }
    else if (ch == 115)
    {
        screenSetColor(GREEN, DARKGRAY);
        printKey(ch);
        screenUpdate();
    }
    else if (ch == 100)
    {
        screenSetColor(BLUE, DARKGRAY);
        printKey(ch);
        screenUpdate();
    }
    else if (ch == 97)
    {
        screenSetColor(MAGENTA, DARKGRAY);
        printKey(ch);
        screenUpdate();
    }
    else{
        screenSetColor(YELLOW, DARKGRAY);
        printKey(ch);
        screenUpdate();
    }
}

int main() 
{
    static int ch = 0;
    static long timer = 0;

    screenInit(1);
    keyboardInit();
    timerInit(50);

    printHello(x, y);
    screenUpdate();

    while (ch != 10 && timer <= 1000) //enter or 5s
    {
        // Handle user input
        if (keyhit()) 
        {
            ch = readch();
            movimentacao(ch);
  
        }

        // Update game state (move elements, verify collision, etc)
        if (timerTimeOver() == 1)
        {
            int newX = x + incX;
            int newY = y + incY;
            if (newX >= (MAXX -strlen("Hello World") -1) || newX <= MINX+1) incX = -incX;
            if (newY >= MAXY-1 || newY <= MINY+1) incY = -incY;
            
            

            screenUpdate();
            timer++;
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
