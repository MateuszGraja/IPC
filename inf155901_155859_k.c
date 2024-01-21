#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define SERVER_KEY 6969

struct message {
    long mtype;
    struct client_info {
        int pid;
        char name[50];
    } client;
};

int main() {
    int server_queue, client_queue;
    key_t key;
    struct message msg;

    // Utwórz kolejkę serwera
    key = ftok(".", SERVER_KEY);
    server_queue = msgget(key, IPC_CREAT | 0666);

    // Utwórz unikalny klucz dla klienta
    key = ftok(".", getpid());
    client_queue = msgget(key, IPC_CREAT | 0666);

    // Zaloguj się do serwera
    char username[40];
    printf("%d\n", getpid());
    printf("Login: \n");
    scanf("%s", username);

    msg.mtype = 1;
    msg.client.pid = getpid();
    strcpy(msg.client.name, username);

    msgsnd(server_queue, &msg, sizeof(msg.client), 0);

    // Odbierz potwierdzenie i PID użytkownika
    msgrcv(client_queue, &msg, sizeof(msg.client), 1, 0);
    int userPid = msg.client.pid;
    printf("Server response: PID %d\n", userPid);

    return 0;
}
