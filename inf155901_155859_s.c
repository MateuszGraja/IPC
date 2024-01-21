#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MAX_USERS 69
#define SERVER_KEY 6969

struct message {
    long mtype;
    struct client_info {
        int pid;
        char name[50];
    } client;
};

struct user {
    int pid;
    char name[50];
};

struct user users[MAX_USERS];
int user_count = 0;

void addUser(int pid, const char *name) {
    users[user_count].pid = pid;
    strcpy(users[user_count].name, name);
    user_count++;
}

int findUserByName(const char *name) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].name, name) == 0) {
            return users[i].pid;
        }
    }
    return -1;
}

// void printUsers() {
//     printf("Registered Users:\n");
//     for (int i = 0; i < user_count; i++) {
//         printf("PID: %d, Name: %s\n", users[i].pid, users[i].name);
//     }
//     printf("\n");
// }

int main() {
    int server_queue, client_queue;
    key_t key;
    struct message msg;

    // Utwórz kolejkę serwera
    key = ftok(".", SERVER_KEY);
    server_queue = msgget(key, IPC_CREAT | 0666);

    printf("%d\n", getpid());
    printf("Server started. Waiting for clients...\n");

    while (1) {
        // Odbierz komunikat od klienta
        msgrcv(server_queue, &msg, sizeof(msg.client), 1, 0);

        // Sprawdź czy to komunikat logowania
        if (msg.mtype == 1) {
            // Sprawdź czy użytkownik istnieje
            int userId = findUserByName(msg.client.name);

            if (userId == -1) {
                // Dodaj nowego użytkownika
                addUser(msg.client.pid, msg.client.name);
                printf("User '%s' registered with PID %d\n", msg.client.name, msg.client.pid);
                // printUsers();
            } else {
                // Zaloguj istniejącego użytkownika
                printf("User '%s' logged in with PID %d\n", msg.client.name, msg.client.pid);
                // printUsers();
            }

            // Utwórz kolejkę klienta i wyślij PID użytkownika
            key = ftok(".", msg.client.pid);
            client_queue = msgget(key, IPC_CREAT | 0666);

            msg.mtype = 1;
            sprintf(msg.client.name, "%d", msg.client.pid);
            msgsnd(client_queue, &msg, sizeof(msg.client), 0);
        }
    }
    return 0;
}
