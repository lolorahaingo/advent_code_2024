#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define MAX_NUMBERS 1000

// Fonction pour afficher une combinaison d'opérations
void print_combination(long double *numbers, int count, int combination) {
    printf("Combination: ");
    // printf("%.2f", numbers[0]); // Premier nombre sans opérateur

    int current_combination = combination;

    for (int i = 0; i < count - 1; i++) {
        int operator_choice = current_combination % 2;
        current_combination /= 2;

        if (operator_choice == 1) {
            // printf(" + %.2f", numbers[i + 1]);
        } else {
            // printf(" * %.2f", numbers[i + 1]);
        }
    }

    // printf("\n");
}

// Fonction pour essayer toutes les combinaisons d'opérateurs et vérifier si la cible est atteinte
bool can_solve(long double target, long double *numbers, int count) {
    // Nombre total de combinaisons possibles : 2^(count - 1)
    int total_combinations = 1;
    for (int i = 0; i < count - 1; i++) {
        total_combinations *= 2; // Équivalent à 2^(count - 1)
    }

    // Allouer un tableau pour stocker les résultats
    long double *results = (long double *)malloc(total_combinations * sizeof(long double));
    if (!results) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    // Calculer tous les résultats possibles
    for (int combination = 0; combination < total_combinations; combination++) {
        long double result = numbers[0];
        int current_combination = combination;

        // Évaluer cette combinaison
        for (int i = 0; i < count - 1; i++) {
            int operator_choice = current_combination & 1;
            // current_combination /= 2;

            if (operator_choice == 1) {
                result += numbers[i + 1];
            } else {
                result *= numbers[i + 1];
            }
            current_combination >>= 1;
        }

        // Stocker le résultat dans le tableau
        results[combination] = result;

        // Si la cible est atteinte
        if (fabs(result - target) < 1e-6) { // Comparaison pour les long doubles
            free(results);
            return true;
        }
    }

    // Libérer la mémoire et retourner false si aucune correspondance n'est trouvée
    free(results);
    return false;
}

// Fonction pour lire une ligne et extraire les valeurs
bool parse_line(const char *line, long double *target, long double *numbers, int *count) {
    char *token;
    char buffer[1024];
    strcpy(buffer, line);

    // Extraire la valeur cible avant le ":"
    token = strtok(buffer, ":");
    if (!token) return false;
    *target = atof(token);

    // Extraire les nombres après le ":"
    *count = 0;
    token = strtok(NULL, " ");
    while (token) {
        numbers[(*count)++] = atof(token);
        token = strtok(NULL, " ");
    }

    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char line[1024];
    long double total_sum = 0;

    // Lire le fichier ligne par ligne
    while (fgets(line, sizeof(line), file)) {
        long double target;
        long double numbers[MAX_NUMBERS];
        int count;

        // Analyser la ligne
        if (!parse_line(line, &target, numbers, &count)) {
            fprintf(stderr, "Error parsing line: %s\n", line);
            continue;
        }

        // Vérifier si l'équation peut être résolue
        if (can_solve(target, numbers, count)) {
            // printf("Valid equation found for target %.2f\n", target);
            total_sum += target;
        }
    }

    fclose(file);

    // Afficher le résultat final
    printf("Total calibration result: %.2Lf\n", total_sum);

    return EXIT_SUCCESS;
}
