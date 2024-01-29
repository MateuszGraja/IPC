#include <stdio.h>
#include <string.h>

#include "../include/printer.h"

void printHeader(const char *text, int width)
{
    printCharacter('=', width);
    printCenteredText(text, width);
    printCharacter('=', width);
}

void printCenteredText(const char *text, int width)
{
    int padding = (width - strlen(text)) / 2;

    for (int i = 0; i < padding; i++)
    {
        printf(" ");
    }

    printf("%s\n", text);
}

void printCharacter(char character, int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("%c", character);
    }
    printf("\n");
}

void printClear()
{
    printf("\033[2J\033[H");
}