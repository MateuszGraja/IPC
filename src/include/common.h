#ifndef COMMON_H
#define COMMON_H
#include <stdbool.h>

void clearBuffer();

int getInputInteger();

int getInputIntegerMinMax(int min, int max);

unsigned long hash(const char *str);

extern bool is_async_mode_on;

#endif
