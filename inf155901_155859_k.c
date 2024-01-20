#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "OperationTypeEnum.h"

int main(int argc, char const *argv[])
{
    while (1)
    {
        printf("");
    }
    switch (enum OperationsType operationType)
    {
    case LOGIN:

        break;
    case REGISTER:

        break;
    case EXIT:
        return 0;
    default:
        return 0;
    }

    // key_t serverKey = ftok();
    printf("Hello!");
    return 0;
}
