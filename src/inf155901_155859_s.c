#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "include/enums/OperationTypeEnum.h"
#include "include/constants.h"
#include "include/common.h"
#include "include/structs.h"

static struct User logged_users[MAX_USERS];
static unsigned int logged_users_count = 0;

static struct UserSettings registered_users[MAX_USERS];
static unsigned int registered_users_count = 0;

static struct Topic topics[MAX_TOPICS];
static unsigned int topics_count = 0;

static int getTopicIdxByCode(char code[MSG_CODE_LENGTH])
{
    for (int i = 0; i < topics_count; i++)
    {
        if (strcmp(topics[i].code, code) == 0)
        {
            return i;
        }
    }
    return -1;
}

static int getUserIdxByName(char name[MAX_NAME_LENGTH])
{
    for (int i = 0; i < registered_users_count; i++)
    {
        if (strcmp(registered_users[i].name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

static bool isNameUnique(char name[MAX_NAME_LENGTH])
{
    for (size_t i = 0; i < registered_users_count; i++)
    {
        if (strcmp(name, registered_users[i].name) == 0)
        {
            return false;
        }
    }
    return true;
}

static bool isTopicUnique(char code[MSG_CODE_LENGTH])
{
    for (size_t i = 0; i < topics_count; i++)
    {
        if (strcmp(code, topics[i].code) == 0)
        {
            return false;
        }
    }
    return true;
}


int main(int argc, char const *argv[])
{
    int client_queue;
    const int server_queue = msgget(ftok(".", IPC_SERVER_KEY), IPC_CREAT | 0666);

    struct LoginMessage loginMessage;
    struct RegisterMessage registerMessage;
    struct CreateTopicMessage createTopicMessage;
    struct SubscribeTopicMessage subscribeTopicMessage;
    struct BanMessage banMessage;
    struct BroadcastTextMessage broadcastTextMessage;
    struct ServerResponseMessage serverResponseMessage;

    printf("Server started. Waiting for clients...\n");

    while (1)
    {
        // Logowanie użytkownika
        if (msgrcv(server_queue, &loginMessage, sizeof(loginMessage) - sizeof(loginMessage.mtype), LOGIN, IPC_NOWAIT | MSG_NOERROR) != -1)
        {
            printf("\n[LOGIN] Started request\n");
            if (logged_users_count < MAX_USERS)
            {
                if (!isNameUnique(loginMessage.client.name))
                {
                    int idx = logged_users_count++;
                    logged_users[idx].id = loginMessage.client.pid;
                    strcpy(logged_users[idx].name, loginMessage.client.name);

                    snprintf(serverResponseMessage.response.text, sizeof(serverResponseMessage.response.text), "%s %s", "Successfully logged in with name:", loginMessage.client.name);
                    serverResponseMessage.response.status = 0;
                }
                else
                {
                    strcpy(serverResponseMessage.response.text, "Error - no such user exists");
                    serverResponseMessage.response.status = 1;
                }
            }
            else
            {
                strcpy(serverResponseMessage.response.text, "Error - maximum logged users exceeded");
                serverResponseMessage.response.status = 1;
            }
            serverResponseMessage.mtype = SERVER_RESPONSE;
            client_queue = msgget((key_t)hash(loginMessage.client.name), IPC_CREAT | 0666);
            msgsnd(client_queue, &serverResponseMessage, sizeof(serverResponseMessage) - sizeof(serverResponseMessage.mtype), 0);
            printf("[LOGIN] Ended request\n\n");
        };

        // Rejestracja użytkownika
        if (msgrcv(server_queue, &registerMessage, sizeof(registerMessage) - sizeof(registerMessage.mtype), REGISTER, IPC_NOWAIT | MSG_NOERROR) != -1)
        {
            printf("\n[REGISTER] Started request\n");
            if (registered_users_count < MAX_USERS)
            {
                if (isNameUnique(registerMessage.client.name))
                {
                    registered_users[registered_users_count].banned_by_users_count = 0;
                    strcpy(registered_users[registered_users_count++].name, registerMessage.client.name);
                    snprintf(serverResponseMessage.response.text, sizeof(serverResponseMessage.response.text), "%s %s", "Successfully registered with name:", registerMessage.client.name);
                    serverResponseMessage.response.status = 0;
                }
                else
                {
                    strcpy(serverResponseMessage.response.text, "Error - name is already occupied");
                    serverResponseMessage.response.status = 1;
                }
            }
            else
            {
                strcpy(serverResponseMessage.response.text, "Error - maximum registered users exceeded");
                serverResponseMessage.response.status = 1;
            }
            serverResponseMessage.mtype = SERVER_RESPONSE;
            client_queue = msgget((key_t)hash(registerMessage.client.name), IPC_CREAT | 0666);
            msgsnd(client_queue, &serverResponseMessage, sizeof(serverResponseMessage) - sizeof(serverResponseMessage.mtype), 0);
            printf("[REGISTER] Ended request\n\n");
        };

        // Tworzenie nowego tematu
        if (msgrcv(server_queue, &createTopicMessage, sizeof(createTopicMessage) - sizeof(createTopicMessage.mtype), CREATE_TOPIC, IPC_NOWAIT | MSG_NOERROR) != -1)
        {
            printf("\n[NEW TOPIC] Started request\n");
            if (topics_count < MAX_TOPICS)
            {
                if (isTopicUnique(createTopicMessage.topic.code))
                {
                    strcpy(topics[topics_count].code, createTopicMessage.topic.code);
                    topics[topics_count++].settings_count = 0;

                    snprintf(serverResponseMessage.response.text, sizeof(serverResponseMessage.response.text), "%s %s", "Successfully added new topic with code:", createTopicMessage.topic.code);
                    serverResponseMessage.response.status = 0;
                }
                else
                {
                    strcpy(serverResponseMessage.response.text, "Error - topic already exists");
                    serverResponseMessage.response.status = 1;
                }
            }
            else
            {
                strcpy(serverResponseMessage.response.text, "Error - maximum topics exceeded");
                serverResponseMessage.response.status = 1;
            }
            serverResponseMessage.mtype = SERVER_RESPONSE;
            client_queue = msgget((key_t)hash(createTopicMessage.name), IPC_CREAT | 0666);
            msgsnd(client_queue, &serverResponseMessage, sizeof(serverResponseMessage) - sizeof(serverResponseMessage.mtype), 0);
            printf("[NEW TOPIC] Ended request\n\n");
        };

        // Subskrybowanie tematu
        if (msgrcv(server_queue, &subscribeTopicMessage, sizeof(subscribeTopicMessage) - sizeof(subscribeTopicMessage.mtype), SUBSCRIBE_TOPIC, IPC_NOWAIT | MSG_NOERROR) != -1)
        {
            printf("\n[SUBSCRIBE TOPIC] Started request\n");
            int idx = getTopicIdxByCode(subscribeTopicMessage.code);
            if (idx != -1)
            {
                if (topics[idx].settings_count < MAX_USERS)
                {
                    topics[idx].settings[topics[idx].settings_count++] = subscribeTopicMessage.settings;

                    snprintf(serverResponseMessage.response.text, sizeof(serverResponseMessage.response.text), "%s %s", "Successfully subscribe to topic with code:", subscribeTopicMessage.code);
                    serverResponseMessage.response.status = 0;
                }
                else
                {
                    strcpy(serverResponseMessage.response.text, "Error - maximum threshold of settings for this topic has been exceeded");
                    serverResponseMessage.response.status = 1;
                }
            }
            else
            {
                strcpy(serverResponseMessage.response.text, "Error - no topic with provided code found");
                serverResponseMessage.response.status = 1;
            }

            serverResponseMessage.mtype = SERVER_RESPONSE;
            client_queue = msgget((key_t)hash(subscribeTopicMessage.settings.name), IPC_CREAT | 0666);
            msgsnd(client_queue, &serverResponseMessage, sizeof(serverResponseMessage) - sizeof(serverResponseMessage.mtype), 0);
            printf("[SUBSCRIBE TOPIC] Ended request\n\n");
        };
        

        // Wysyłanie/rozgłaszanie wiadomości
        if (msgrcv(server_queue, &broadcastTextMessage, sizeof(broadcastTextMessage) - sizeof(broadcastTextMessage.mtype), SEND_MESSAGE, IPC_NOWAIT | MSG_NOERROR) != -1)
        {
            printf("\n[BROADCAST] Started request\n");

            int idx = getTopicIdxByCode(broadcastTextMessage.message.code);
            if (idx != -1)
            {
                int userIdx = getUserIdxByName(broadcastTextMessage.name);
                if (userIdx != -1)
                {
                    for (size_t i = 0; i < topics[idx].settings_count; i++)
                    {
                        struct TopicSettings settings = topics[idx].settings[i];
                        printf("%s\n", settings.name);

                        // Jeśli ilość otrzymanych wiadomości przekroczyła limit, nie wysyłamy nowej wiadomości
                        if (settings.max_messages != 0 && settings.messages_count >= settings.max_messages)
                        {
                            continue;
                        }

                        // Jeśli użytkownik wysyłający komunikat zawiera w liście osób przez które został zbanowany osobę, której zamierzał wysłać komunikat,
                        // komunikat ten nie zostanie przesłany
                        printf("%d\n", registered_users[userIdx].banned_by_users_count);
                        for (size_t j = 0; j < registered_users[userIdx].banned_by_users_count; j++)
                        {
                            printf("%s\n", registered_users[userIdx].banned_by_users[j]);
                            printf("%s\n", settings.name);
                            if (strcmp(registered_users[userIdx].banned_by_users[j], settings.name) == 0)
                            {
                                goto skip_iteration;
                            };
                        }

                        topics[idx].settings[i].messages_count++;
                        int recipient_queue = msgget((key_t)hash(settings.name), IPC_CREAT | 0666);
                        msgsnd(recipient_queue, &broadcastTextMessage, sizeof(broadcastTextMessage) - sizeof(broadcastTextMessage.mtype), 0);
                        strcpy(serverResponseMessage.response.text, "Successfully sent message");
                        serverResponseMessage.response.status = 0;
                    skip_iteration:
                    }
                }
                else
                {
                    strcpy(serverResponseMessage.response.text, "Error - no user found");
                    serverResponseMessage.response.status = 1;
                }
            }
            else
            {
                strcpy(serverResponseMessage.response.text, "Error - no topic with provided code found");
                serverResponseMessage.response.status = 1;
            }

            serverResponseMessage.mtype = SERVER_RESPONSE;
            client_queue = msgget((key_t)hash(subscribeTopicMessage.settings.name), IPC_CREAT | 0666);
            msgsnd(client_queue, &serverResponseMessage, sizeof(serverResponseMessage) - sizeof(serverResponseMessage.mtype), 0);
            printf("[BROADCAST] Ended request\n\n");
        }
        else
        {
            // printf("Server connection problem.\n");
        };

        // Banowanie usera
        if (msgrcv(server_queue, &banMessage, sizeof(banMessage) - sizeof(banMessage.mtype), BAN_USER, IPC_NOWAIT | MSG_NOERROR) != -1)
        {
            printf("\n[BAN] Started request\n");
            int idx = getUserIdxByName(banMessage.name_to_ban);
            if (idx != -1 && !isNameUnique(banMessage.name))
            {
                if (registered_users[idx].banned_by_users_count < MAX_USERS)
                {
                    strcpy(registered_users[idx].banned_by_users[registered_users[idx].banned_by_users_count++], banMessage.name);

                    snprintf(serverResponseMessage.response.text, sizeof(serverResponseMessage.response.text), "%s %s", "Successfully banned user with name:", banMessage.name_to_ban);
                    serverResponseMessage.response.status = 0;
                }
                else
                {
                    strcpy(serverResponseMessage.response.text, "Error - maximum banned users exceeded");
                    serverResponseMessage.response.status = 1;
                }
            }
            else
            {
                strcpy(serverResponseMessage.response.text, "Error - no such user exists");
                serverResponseMessage.response.status = 1;
            }

            serverResponseMessage.mtype = SERVER_RESPONSE;
            client_queue = msgget((key_t)hash(banMessage.name), IPC_CREAT | 0666);
            msgsnd(client_queue, &serverResponseMessage, sizeof(serverResponseMessage) - sizeof(serverResponseMessage.mtype), 0);
            printf("[BAN] Ended request\n\n");
        }
        else
        {
            // printf("Server connection problem.\n");
        };

        // Opóźniamy kolejną iterację o 100 milisekund, żeby nie przeładowywać procesora
        usleep(100000);
    }

    return 0;
}
