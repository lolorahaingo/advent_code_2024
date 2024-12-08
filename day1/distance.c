#include "distance.h"
#include <stdlib.h>

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int calculate_total_distance(int* left_list, int* right_list, int size) {
    // Trier les listes
    qsort(left_list, size, sizeof(int), compare);
    qsort(right_list, size, sizeof(int), compare);

    int total_distance = 0;
    for (int i = 0; i < size; i++) {
        total_distance += abs(left_list[i] - right_list[i]);
    }

    return total_distance;
}

