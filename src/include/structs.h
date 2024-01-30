#ifndef STRUCTS_H
#define STRUCTS_H

#include "constants.h"

// Struktury dot. tematów

struct TopicSettings
{
    char name[MAX_NAME_LENGTH];
    unsigned short int max_messages;
    unsigned short int messages_count;
};

struct Topic
{
    char code[MSG_CODE_LENGTH];
    struct TopicSettings settings[MAX_USERS];
    unsigned short int settings_count;
};

// Struktury dot. użytkowników

struct User
{
    // Id - identyfikuje konkretną sesję/proces
    int id;
    char name[MAX_NAME_LENGTH];
};

struct UserSettings
{
    char name[MAX_NAME_LENGTH];
    char banned_by_users[MAX_USERS][MAX_NAME_LENGTH];
    unsigned short int banned_by_users_count;
};

// Struktury komunikatów

struct MessageBase
{
    long mtype;
};

struct LoginMessage
{
    long mtype;
    struct ClientInfo
    {
        int pid;
        char name[MAX_NAME_LENGTH];
    } client;
};

struct TextMessage
{
    long mtype;
    char text[512];
};

struct RegisterMessage
{
    long mtype;
    struct Client
    {
        char name[MAX_NAME_LENGTH];
    } client;
};

struct BanMessage
{
    long mtype; 
    char name[MAX_NAME_LENGTH];
    char name_to_ban[MAX_NAME_LENGTH]; 
};

struct SubscribeTopicMessage
{
    long mtype;
    char code[MSG_CODE_LENGTH];
    struct TopicSettings settings;
};

struct CreateTopicMessage
{
    long mtype;
    char name[MAX_NAME_LENGTH];
    struct TopicCreation
    {
        char code[MSG_CODE_LENGTH];
    } topic;
};

struct ServerResponseMessage
{
    long mtype;
    struct Response
    {
        // 0 = success
        short int status;
        char text[512];
    } response;
};

// Komunikat dla rozgłaszania/wysyłania wiadomości - unlucky nazwa
struct BroadcastTextMessage
{
    long mtype;
    char name[MAX_NAME_LENGTH];
    struct Message
    {
        char code[MSG_CODE_LENGTH];
        char text[512];
        unsigned int priority;
    } message;
};

#endif
