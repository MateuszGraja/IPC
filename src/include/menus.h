#ifndef MENUS_H
#define MENUS_H

#include "enums/OperationTypeEnum.h"
#include "structs.h"

enum OperationType displayInitialMenu();

enum OperationType displayMainMenu();

void displayLoginMenu(char *name);

void displayRegisterMenu(char *name);

void displayTopicCreationMenu(char *code);

void displayTopicSubscriptionMenu(struct SubscribeTopicMessage *subscribeTopicMessage);

#endif
