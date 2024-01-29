#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/common.h"

// Clear the input buffer to avoid an infinite loop
void clearBuffer()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

int getInputInteger()
{
    int userInput;
    while (scanf("%d", &userInput) != 1)
    {
        printf("Enter a valid integer.\n");
        clearBuffer();
    }
    return userInput;
}

int getInputIntegerMinMax(int min, int max)
{
    int userInput;
    while (scanf("%d", &userInput) != 1 && userInput >= min && userInput <= max)
    {
        printf("Enter a valid integer.\n");
        clearBuffer();
    }
    return userInput;
}

unsigned long hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash;
}