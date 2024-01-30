#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "include/enums/OperationTypeEnum.h"
#include "include/constants.h"
#include "include/common.h"
#include "include/menus.h"
#include "include/structs.h"
#include "include/printer.h"

static int server_queue, client_queue;
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
            printf("%s\n", serverResponseMessage.response.text);
            if (serverResponseMessage.response.status == 0)
            {
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
        struct SubscribeTopicMessage subscribeTopicMessage;
        displayTopicSubscriptionMenu(&subscribeTopicMessage);

        subscribeTopicMessage.mtype = SUBSCRIBE_TOPIC;
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
    case SEND_MESSAGE:
        struct BroadcastTextMessage broadcastTextMessage;
        displaySendMessageMenu(&broadcastTextMessage);

        broadcastTextMessage.mtype = SEND_MESSAGE;
        strcpy(broadcastTextMessage.name, name);

        msgsnd(server_queue, &broadcastTextMessage, sizeof(broadcastTextMessage) - sizeof(broadcastTextMessage.mtype), 0);
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
    case ASYNC_RECEIVE:
        is_async_mode_on = !is_async_mode_on;
        if (is_async_mode_on)
        {
            if (fork() == 0)
            {
                while (is_async_mode_on)
                {
                    if (msgrcv(client_queue, &broadcastTextMessage, sizeof(broadcastTextMessage) - sizeof(broadcastTextMessage.mtype), SEND_MESSAGE, IPC_NOWAIT | MSG_NOERROR) != -1)
                    {
                        usleep(1000000);
                        printMessage(broadcastTextMessage, 60);
                    };
                    if (!is_async_mode_on)
                    {
                        break;
                    }
                    usleep(100000);
                }
                printf("EXIT\n");

                exit(0);
            }
        }
        printHolder();
        handleOperation(displayMainMenu());
        break;
    case BAN_USER:
        struct BanMessage banMessage;
        char name_to_ban[MAX_NAME_LENGTH];

        displayBanUserMenu(name_to_ban);

        banMessage.mtype = BAN_USER;
        strcpy(banMessage.name_to_ban, name_to_ban);
        strcpy(banMessage.name, name);

        client_queue = msgget((key_t)hash(name), IPC_CREAT | 0666);
        msgsnd(server_queue, &banMessage, sizeof(banMessage) - sizeof(banMessage.mtype), 0);
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
    default:
        break;
    }
}

int main(int argc, char const *argv[])
{
    server_queue = msgget(ftok(".", IPC_SERVER_KEY), IPC_CREAT | 0666);

    handleOperation(displayInitialMenu());

    return 0;
}
