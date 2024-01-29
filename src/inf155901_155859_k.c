#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include "include/enums/OperationTypeEnum.h"
#include "include/constants.h"
#include "include/common.h"
#include "include/menus.h"
#include "include/structs.h"
#include "include/printer.h"

static int server_queue;
static int client_queue;
static char name[MAX_NAME_LENGTH];

void handleOperation(enum OperationType operationType)
{
    struct ServerResponseMessage serverResponseMessage;
    switch (operationType)
    {
    case LOGIN:
        struct LoginMessage loginMessage;
        displayLoginMenu(name);

        loginMessage.mtype = LOGIN;
        loginMessage.client.pid = getpid();
        strcpy(loginMessage.client.name, name);

        client_queue = msgget((key_t)hash(name), IPC_CREAT | 0666);
        msgsnd(server_queue, &loginMessage, sizeof(loginMessage) - sizeof(loginMessage.mtype), 0);
        if (msgrcv(client_queue, &serverResponseMessage, sizeof(serverResponseMessage) - sizeof(serverResponseMessage.mtype), SERVER_RESPONSE, 0) != -1)
        {
            if (serverResponseMessage.response.status == 0)
            {
                printf("%s\n", serverResponseMessage.response.text);
                handleOperation(displayMainMenu());
            }
            else
            {
                handleOperation(displayInitialMenu());
            }
        }
        else
        {
            printf("Server connection problem.\n");
        }
        break;
    case REGISTER:
        struct RegisterMessage registerMessage;
        displayRegisterMenu(name);

        registerMessage.mtype = REGISTER;
        strcpy(registerMessage.client.name, name);

        client_queue = msgget((key_t)hash(name), IPC_CREAT | 0666);
        msgsnd(server_queue, &registerMessage, sizeof(registerMessage) - sizeof(registerMessage.mtype), 0);
        if (msgrcv(client_queue, &serverResponseMessage, sizeof(serverResponseMessage) - sizeof(serverResponseMessage.mtype), SERVER_RESPONSE, 0) != -1)
        {
            printf("%s\n", serverResponseMessage.response.text);
            handleOperation(displayInitialMenu());
        }
        else
        {
            printf("Server connection problem.\n");
        }
        break;
    case CREATE_TOPIC:
        char code[MSG_CODE_LENGTH];
        struct CreateTopicMessage createTopicMessage;
        displayTopicCreationMenu(code);

        createTopicMessage.mtype = CREATE_TOPIC;
        strcpy(createTopicMessage.topic.code, code);
        strcpy(createTopicMessage.name, name);

        msgsnd(server_queue, &createTopicMessage, sizeof(createTopicMessage) - sizeof(createTopicMessage.mtype), 0);
        if (msgrcv(client_queue, &serverResponseMessage, sizeof(serverResponseMessage) - sizeof(serverResponseMessage.mtype), SERVER_RESPONSE, 0) != -1)
        {
            printf("%s\n", serverResponseMessage.response.text);
            handleOperation(displayMainMenu());
        }
        else
        {
            printf("Server connection problem.\n");
        }
        break;
    case SUBSCRIBE_TOPIC:
        char code[MSG_CODE_LENGTH];
        struct SubscribeTopicMessage subscribeTopicMessage;
        displayTopicSubscriptionMenu(&subscribeTopicMessage);

        subscribeTopicMessage.mtype = SUBSCRIBE_TOPIC;
        strcpy(subscribeTopicMessage.code, code);
        strcpy(subscribeTopicMessage.settings.name, name);
        subscribeTopicMessage.settings.messages_count = 0;

        msgsnd(server_queue, &subscribeTopicMessage, sizeof(subscribeTopicMessage) - sizeof(subscribeTopicMessage.mtype), 0);
        if (msgrcv(client_queue, &serverResponseMessage, sizeof(serverResponseMessage) - sizeof(serverResponseMessage.mtype), SERVER_RESPONSE, 0) != -1)
        {
            printf("%s\n", serverResponseMessage.response.text);
            handleOperation(displayMainMenu());
        }
        else
        {
            printf("Server connection problem.\n");
        }
        break;
    case RETURN:
        handleOperation(displayInitialMenu());
        break;
    case EXIT:
        break;
    default:
        break;
    }
}

int main(int argc, char const *argv[])
{
    server_queue = msgget(ftok(".", IPC_SERVER_KEY), IPC_CREAT | 0666);
    // client_queue = msgget(ftok(".", ), IPC_CREAT | IPC_EXCL | 0666);

    // pid_t pid = fork();

    if (1 > 0)
    {
        handleOperation(displayInitialMenu());
    }
    else
    {
        struct TextMessage textMessage;

        while (1)
        {
            if (msgrcv(server_queue, &textMessage, sizeof(textMessage) - sizeof(textMessage.mtype), TEXT, 0) != -1)
            {
                printf("%s\n", textMessage.text);
            }
            else
            {
                printf("ZLE\n");
            };
        }
    }

    // key_t serverKey = ftok();
    return 0;
}
