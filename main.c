#include <stdio.h>
#include <stdlib.h>
#include "cl.h"

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 3) {
        printf("Usage: tcpscan   %%ip%%   %%beginport%%   %%endport%%\n");
        printf("    or\nUsage: tcpscan   %%ip%%   %%port%%\n");
        return 0;
    }
    if (argc == 4)
        scanPorts(argv[1], atoi(argv[2]), atoi(argv[3]));
    else
        scanPorts(argv[1], atoi(argv[2]), atoi(argv[2]));
    return 0;
}
