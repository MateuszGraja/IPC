#ifndef PRINTER_H
#define PRINTER_H

#include "structs.h"

void printMessage(struct BroadcastTextMessage broadcastTextMessage, int width);

void printHeader(const char *text, int width);

void printCenteredText(const char *text, int width);

void printCharacter(char character, int count);

void printClear();
#endif