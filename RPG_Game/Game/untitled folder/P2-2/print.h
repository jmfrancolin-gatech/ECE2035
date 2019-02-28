#include "globals.h"
#include "hardware.h"
#include "mbed.h"

#define MODULE_NAME "PRINT"

#define PRINT(x, ...) \
    { pc.printf("\x1b[33m"x"\x1b[39;49m\r\n", \
    ##__VA_ARGS__); fflush(stdout); }