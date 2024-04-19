#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "utils.h"

void print_error(char* msg) {
    printf("%s: %s\n", msg, strerror(errno));
}
