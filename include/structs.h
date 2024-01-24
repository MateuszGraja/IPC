#ifndef STRUCTS_H
#define STRUCTS_H

#include "constants.h"

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
        char name[50];
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
    struct ClientSettings
    {
        char name[MAX_NAME_LENGTH];

    } client_settings;
};

struct User
{
    int id;
    char name[MAX_NAME_LENGTH];
};

#endif
