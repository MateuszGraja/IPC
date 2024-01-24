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
#include "include/structs.h"

int main(int argc, char const *argv[])
{
    int server_queue, client_queue;

    server_queue = msgget(ftok(".", IPC_SERVER_KEY), IPC_CREAT | 0666);
    client_queue = msgget(ftok(".", getpid()), IPC_CREAT | IPC_EXCL | 0666);

    enum OperationTypes operationType = displayInitialMenu();

    printf("%d", operationType);
    switch (operationType)
    {
    case LOGIN:
        char name[MAX_NAME_LENGTH];
        displayLoginMenu(name);

        struct LoginMessage msg;
        msg.mtype = LOGIN;
        msg.client.pid = getpid();
        strcpy(msg.client.name, name);
        msgsnd(server_queue, &msg, sizeof(msg.client), 0);

        break;
    case REGISTER:
        displayRegisterMenu();
        break;
    case EXIT:
        printf("E choosen");
        return 0;
    default:
        return 0;
    }

    // key_t serverKey = ftok();
    return 0;
}
