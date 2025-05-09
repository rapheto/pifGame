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

float x = 37, y = 22;
int incX = 1, incY = 1;

//Bool
int isShooting = 0;

void movimentacao(int ch){
    screenGotoxy(x, y);
    printf("      ");
    /*if (ch == 119 && y > MINY + 1)
    {
        y = y - 0.5;
        screenSetColor(RED, DARKGRAY);
    }
    else if (ch == 115 && y < MAXY - 1)
    {
        y++;
        screenSetColor(GREEN, DARKGRAY);
    }*/
    if (ch == 100 && x < MAXX - strlen("I--I") - 1)
    {
        x = x + 1;
        screenSetColor(BLUE, DARKGRAY);
    }
    else if (ch == 97 && x > MINX + 1)
    {
        x = x - 1;
        screenSetColor(MAGENTA, DARKGRAY);
    }
    else{
        screenSetColor(YELLOW, DARKGRAY);   
    }
    screenGotoxy(x, y);
    printf("I--I");
    screenUpdate();
}

void bullet(int speedY){
    int bulletX = (int)x + 1;
    int bulletY = (int)y - 1;
    
    while (bulletY > MINY + 1) {
        screenGotoxy(bulletX, bulletY + speedY);
        printf("  ");

        screenGotoxy(bulletX, bulletY);
        printf("||");
    
        screenUpdate();
        bulletY = bulletY - speedY;
        timerDelay(50);

    }

    screenGotoxy(bulletX, bulletY + speedY);
    printf("  ");
    isShooting = 0;
}

void atirar(int ch){
    if (ch == 32)
    {
        isShooting = 1;
        printf("Atirou:%d",isShooting);
        bullet(1);
        printf("Atirou:%d",isShooting);
    }
}

int main() 
{
    static int ch = 0;
    static long timer = 0;
    
    screenInit(2);
    keyboardInit();
    timerInit(50);

    movimentacao(0);

    screenUpdate();

    while (ch != 10 && timer <= 1000) //enter or 5s
    {
        // Handle user input
        if (keyhit()) 
        {
            ch = readch();
            movimentacao(ch);
            if(isShooting == 0){
                atirar(ch);
                
            }
        }

        // Update game state (move elements, verify collision, etc)
        if (timerTimeOver() == 1)
        {
            int newX = x + incX;
            if (newX >= (MAXX -strlen("Hello World") -1) || newX <= MINX+1) incX = -incX;
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
