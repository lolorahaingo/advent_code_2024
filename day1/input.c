#include "input.h"
#include <stdlib.h>
#include <string.h>

void read_lists(const char* filename, int* left_list, int* right_list, int* size) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (fscanf(file, "%d %d", &left_list[i], &right_list[i]) == 2) {
        i++;
    }

    *size = i;
    fclose(file);
}

