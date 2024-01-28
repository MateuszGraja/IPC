#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "include/enums/OperationTypeEnum.h"
#include "include/constants.h"
#include "include/common.h"
#include "include/structs.h"

int main(int argc, char const *argv[])
{
    int client_queue, user_count;
    const int server_queue = msgget(ftok(".", IPC_SERVER_KEY), IPC_CREAT | 0666);

    struct User users[MAX_USERS];
    user_count = 0;

    struct LoginMessage loginMessage;
    struct TextMessage textMessage;

    printf("Server started. Waiting for clients...\n");

    while (1)
    {
        // Logowanie użytkownika
        if (msgrcv(server_queue, &loginMessage, sizeof(loginMessage) - sizeof(loginMessage.mtype), LOGIN, 0) != -1)
        // if (msgrcv(server_queue, &loginMessage, getContentSize(&loginMessage), LOGIN, 0) != -1)
        {
            printf("LOGGIN SERVER");
            if (user_count < MAX_USERS)
            {
                int client_pid = loginMessage.client.pid;
                users[user_count].id = client_pid;
                strcpy(users[user_count].name, loginMessage.client.name);

                client_queue = msgget(ftok(".", client_pid), IPC_CREAT | 0666);
                strcpy(textMessage.text, "Successfully logged in with name:\n");
                msgsnd(client_queue, &textMessage, sizeof(textMessage) - sizeof(textMessage.mtype), 0);
            }
            else
            {
            }
            break;
        }
        else
        {
            printf("ZLE\n");
        };

        

        // msgrcv(server_queue, &messageBase, sizeof(messageBase) - sizeof(messageBase.mtype), 0, 0);

        // switch (messageBase.mtype)
        // {
        // case LOGIN:
        //     // Kastowanie wskaźnika typu MessageBase do wskaźnika innego typu
        //     struct LoginMessage *loginMessage = (struct LoginMessage *)&messageBase;
        //     printf("Your data: \n");
        //     printf("PID = %d\n", loginMessage->client.pid);
        //     printf("Name = %s\n", loginMessage->client.name);
        //     break;
        // case REGISTER:

        //     break;

        // default:
        //     break;
        // }
        // if (messageBase.mtype == LOGIN)
        // {
        //     // Sprawdź czy użytkownik istnieje
        //     // int userId = findUserByName(msg.client.name);

        //     // if (userId == -1)
        //     // {
        //     //     // Dodaj nowego użytkownika
        //     //     addUser(msg.client.pid, msg.client.name);
        //     //     printf("User '%s' registered with PID %d\n", msg.client.name, msg.client.pid);
        //     //     // printUsers();
        //     // }
        //     // else
        //     // {
        //     //     // Zaloguj istniejącego użytkownika
        //     //     printf("User '%s' logged in with PID %d\n", msg.client.name, msg.client.pid);
        //     //     // printUsers();
        //     // }

        //     // // Utwórz kolejkę klienta i wyślij PID użytkownika
        //     // key = ftok(".", msg.client.pid);
        //     // client_queue = msgget(key, IPC_CREAT | 0666);

        //     // msg.mtype = 1;
        //     // sprintf(msg.client.name, "%d", msg.client.pid);
        //     // msgsnd(client_queue, &msg, sizeof(msg.client), 0);
        // }
    }

    return 0;
}
