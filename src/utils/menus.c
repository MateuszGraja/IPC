#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <stdbool.h>
#include <string.h>
#include "../include/constants.h"
#include "../include/common.h"
#include "../include/menus.h"
#include "../include/printer.h"
#include "../include/enums/OperationTypeEnum.h"

bool is_async_mode_on = false;

void printHolder()
{
    printf("Press any key to continue...\n");
    getchar();
}

enum OperationType displayInitialMenu()
{
    printHeader("LOGGING", 40);

    printf("1) Log In\n");
    printf("2) Register\n");
    printf("3) Exit\n");

    int choice = getInputInteger();
    printClear();

    switch (choice)
    {
    case 1:
        return LOGIN;
    case 2:
        return REGISTER;
    case 3:
        return EXIT;
    default:
        return NONE;
    }
};

enum OperationType displayMainMenu()
{
    printHeader("MAIN MENU", 40);

    printf("1) Add new topic\n");
    printf("2) Subscribe to topic\n");
    printf("3) Send message\n");
    printf("4) Ban user\n");
    printf("5) Automatical messaging");
    if (is_async_mode_on)
    {
        printf(" [ON]");
    }
    else
    {
        printf(" [OFF]");
    }
    printf("\n");
    printf("6) Exit\n");

    int choice = getInputInteger();
    printClear();

    switch (choice)
    {
    case 1:
        return CREATE_TOPIC;
    case 2:
        return SUBSCRIBE_TOPIC;
    case 3:
        return SEND_MESSAGE;
    case 4:
        return BAN_USER;
    case 5:
        return ASYNC_RECEIVE;
    default:
        return NONE;
    }
};

void displayLoginMenu(char *name)
{
    printf("\n");
    printf("Provide name to log in: ");
    scanf("%49s", name);
    printf("\n");
};

void displayRegisterMenu(char *name)
{
    printf("\n");
    printf("Provide name to sign up: ");
    scanf("%49s", name);
    printf("\n");
};

void displayBanUserMenu(char *name)
{
    printf("\n");
    printf("Provide user name to ban: ");
    scanf("%49s", name);
    printf("\n");
};

void displayTopicCreationMenu(char *code)
{
    printf("\n");
    printf("Provide code for new topic: ");
    scanf("%14s", code);
    printf("\n");
};

void displayTopicSubscriptionMenu(struct SubscribeTopicMessage *subscribeTopicMessage)
{
    printf("\n");
    printf("Provide code for topic to subscribe: ");
    scanf("%14s", subscribeTopicMessage->code);
    printf("\n");
    printf("Provide maximum messages you want to receive (max 100): ");
    subscribeTopicMessage->settings.max_messages = getInputIntegerMinMax(0, 100);
    printf("\n");
};

void displaySendMessageMenu(struct BroadcastTextMessage *broadcastTextMessage)
{
    printf("\n");
    printf("Provide topic code: ");
    scanf("%14s", broadcastTextMessage->message.code);
    printf("\n");
    printf("Enter your message: ");
    getchar();
    scanf("%511[^\n]", broadcastTextMessage->message.text);
    printf("\n");
    printf("Provide priority for your message: ");
    broadcastTextMessage->message.priority = getInputIntegerMinMax(0, 100);
    printf("\n");
};
