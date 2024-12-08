#include "similarity.h"

int calculate_similarity_score(int* left_list, int* right_list, int size) {
    int similarity_score = 0;

    for (int i = 0; i < size; i++) {
        int count = 0;
        // Compter combien de fois left_list[i] apparaît dans right_list
        for (int j = 0; j < size; j++) {
            if (left_list[i] == right_list[j]) {
                count++;
            }
        }
        similarity_score += left_list[i] * count;
    }

    return similarity_score;
}
