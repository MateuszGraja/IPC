#ifndef COMMON_H
#define COMMON_H

#include "enums/OperationTypeEnum.h"
#include "structs.h"

extern void clearBuffer();

// extern void printHeader();

extern enum OperationTypes displayInitialMenu();

extern void displayLoginMenu(char *name);

extern void displayRegisterMenu();

extern size_t getContentSize(void *ptr);
// extern size_t getContentSize(struct MessageBase *msg);

#endif
