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

static struct Topic *getTopicByCode(char code[MSG_CODE_LENGTH])
{
    for (size_t i = 0; i < topics->settings_count; i++)
    {
        if (strcmp(topics[i].code, code))
        {
            return &topics[i];
        }
    }
}

static bool isTopicSettingsNameUnique(char code[MSG_CODE_LENGTH], char name[MAX_NAME_LENGTH])
{
    for (size_t i = 0; i < topics->settings_count; i++)
    {
        if (strcmp(topics[i].code, code))
        {
            for (size_t j = 0; j < topics[i].settings_count; j++)
            {
                if (strcmp(topics[i].settings[j].name, name) == 0)
                {
                    return false;
                }
            }
            break;
        }
    }
    return true;
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

// static subscribeToTopic() {

// }

int main(int argc, char const *argv[])
{
    int client_queue;
    const int server_queue = msgget(ftok(".", IPC_SERVER_KEY), IPC_CREAT | 0666);

    struct LoginMessage loginMessage;
    struct RegisterMessage registerMessage;
    struct CreateTopicMessage createTopicMessage;
    struct SubscribeTopicMessage subscribeTopicMessage;
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
        }
        else
        {
            // printf("Server connection problem.\n");
        };

        // Rejestracja użytkownika
        if (msgrcv(server_queue, &registerMessage, sizeof(registerMessage) - sizeof(registerMessage.mtype), REGISTER, IPC_NOWAIT | MSG_NOERROR) != -1)
        {
            printf("\n[REGISTER] Started request\n");
            if (registered_users_count < MAX_USERS)
            {
                if (isNameUnique(registerMessage.client.name))
                {
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
        }
        else
        {
            // printf("Server connection problem.\n");
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
        }
        else
        {
            // printf("Server connection problem.\n");
        };

        // Subskrybowanie tematu
        if (msgrcv(server_queue, &subscribeTopicMessage, sizeof(subscribeTopicMessage) - sizeof(subscribeTopicMessage.mtype), SUBSCRIBE_TOPIC, IPC_NOWAIT | MSG_NOERROR) != -1)
        {
            printf("\n[SUBSCRIBE TOPIC] Started request\n");
            if (topics->settings_count < MAX_USERS)
            {
                if (isTopicSettingsNameUnique(subscribeTopicMessage.code, subscribeTopicMessage.settings.name))
                {
                    struct SubscribeTopicMessage *topic = getTopicByCode(subscribeTopicMessage.code);
                    strcpy(topics[topics_count].code, createTopicMessage.topic.code);
                    topics[topics_count++].settings_count = 0;

                    snprintf(serverResponseMessage.response.text, sizeof(serverResponseMessage.response.text), "%s %s", "Successfully added new topic with code:", createTopicMessage.topic.code);
                    serverResponseMessage.response.status = 0;
                }
                else
                {
                    strcpy(serverResponseMessage.response.text, "Error - settings for this user and topic already exist");
                    serverResponseMessage.response.status = 1;
                }
            }
            else
            {
                strcpy(serverResponseMessage.response.text, "Error - too many settings");
                serverResponseMessage.response.status = 1;
            }
            serverResponseMessage.mtype = SERVER_RESPONSE;
            client_queue = msgget((key_t)hash(createTopicMessage.name), IPC_CREAT | 0666);
            msgsnd(client_queue, &serverResponseMessage, sizeof(serverResponseMessage) - sizeof(serverResponseMessage.mtype), 0);
            printf("[SUBSCRIBE TOPIC] Ended request\n\n");
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
