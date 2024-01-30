#include <stdio.h>
#include <string.h>

#include "../include/printer.h"
#include "../include/structs.h"

void printMessage(struct BroadcastTextMessage broadcastTextMessage, int width)
{
    printCharacter('=', width);
    printf("\tType:\t%s\n", broadcastTextMessage.message.code);
    printCharacter('-', width);
    printf("\tAuthor:\t%s\n", broadcastTextMessage.name);
    printCharacter('-', width);
    printf("\t%s\n", broadcastTextMessage.message.text);
    printCharacter('=', width);
}

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
    for (size_t i = 0; i < 10; i++)
    {
        printf("\n");
    }
    // printf("\033[2J\033[H");
}