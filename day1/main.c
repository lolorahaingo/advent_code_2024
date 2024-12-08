#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "distance.h"
#include "similarity.h"

#define MAX_SIZE 1000

int main() {
    int left_list[MAX_SIZE];
    int right_list[MAX_SIZE];
    int size;

    // Lire les données d'entrée
    read_lists("input_file", left_list, right_list, &size);

    // Calculer la distance totale
    int total_distance = calculate_total_distance(left_list, right_list, size);
    printf("Total distance: %d\n", total_distance);

    // Calculer le score de similarité
    int similarity_score = calculate_similarity_score(left_list, right_list, size);
    printf("Similarity score: %d\n", similarity_score);

    return 0;
}
