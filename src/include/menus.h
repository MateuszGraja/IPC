#ifndef MENUS_H
#define MENUS_H

#include "enums/OperationTypeEnum.h"
#include "structs.h"

void printHolder();

enum OperationType displayInitialMenu();

enum OperationType displayMainMenu();

void displayLoginMenu(char *name);

void displayRegisterMenu(char *name);

void displayBanUserMenu(char *name);

void displayTopicCreationMenu(char *code);

void displayTopicSubscriptionMenu(struct SubscribeTopicMessage *subscribeTopicMessage);

void displaySendMessageMenu(struct BroadcastTextMessage *broadcastTextMessage);

extern bool is_async_mode_on;

#endif
