#include <stdio.h>
#include <stdlib.h>

#define DEBUGFS_PATH "/sys/kernel/debug/cpustat"

int main(int argc, char *argv[]) {
    FILE *file = fopen(DEBUGFS_PATH, "r");
    if (!file) {
        perror("Failed to open debugfs file");
        return EXIT_FAILURE;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }

    fclose(file);
    return EXIT_SUCCESS;
}

