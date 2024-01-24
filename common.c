#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "include/constants.h"
#include "include/structs.h"
#include "include/common.h"
#include "include/enums/OperationTypeEnum.h"

// Clear the input buffer to avoid an infinite loop
void clearBuffer()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

void printHeader()
{
    printf("\n");
    for (int i = 0; i < 30; ++i)
    {
        printf("=");
    }
    printf("\n");

    for (int i = 0; i < 11; ++i)
    {
        printf("-");
    }
    printf("OPTIONS");
    for (int i = 0; i < 11; ++i)
    {
        printf("-");
    }
    printf("\n");

    for (int i = 0; i < 30; ++i)
    {
        printf("=");
    }
    printf("\n");
}

static int getInputInteger()
{
    int userInput;
    while (scanf("%d", &userInput) != 1)
    {
        printf("Enter a valid integer.\n");
        clearBuffer();
    }
    return userInput;
}

enum OperationTypes displayInitialMenu()
{
    printHeader();

    printf("1) Log In\n");
    printf("2) Register\n");
    printf("9) Exit\n");

    int choice = getInputInteger();

    switch (choice)
    {
    case 1:
        return LOGIN;
    case 2:
        return REGISTER;
    case 3:
        return NONE;
    default:
        return NONE;
    }
};

void displayLoginMenu(char *name)
{
    printf("\n");

    printf("Provide name to log in...\n");

    scanf("%49s", name);
};

void displayRegisterMenu()
{
    printf("\n");

    printf("Provide name to log in...\n");

    char name[MAX_NAME_LENGTH];

    scanf("%49s", name);
};

size_t getContentSize(void *ptr)
{
    return sizeof(*ptr) - sizeof(unsigned long);
};

// size_t getContentSize(struct MessageBase *msg)
// {
//     return sizeof(*msg) - sizeof(msg->mtype);
// };
