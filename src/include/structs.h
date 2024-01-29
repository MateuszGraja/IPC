#ifndef STRUCTS_H
#define STRUCTS_H

#include "constants.h"

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
    struct UserSettings *banned_users[MAX_USERS];
};

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

#endif
