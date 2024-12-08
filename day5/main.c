#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RULES 1000
#define MAX_UPDATES 1000

// Structure pour une règle de dépendance (X|Y)
typedef struct {
    int before;
    int after;
} Rule;

// Fonction pour lire les règles et les mises à jour depuis le fichier
int parse_input(FILE *file, Rule **rules, int *num_rules, int ***updates, int *num_updates) {
    char line[256];
    int is_rules_section = 1;

    // Allouer de la mémoire pour les règles et mises à jour
    *rules = (Rule *)malloc(MAX_RULES * sizeof(Rule));
    *updates = (int **)malloc(MAX_UPDATES * sizeof(int *));
    
    if (*rules == NULL || *updates == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire.\n");
        return -1;
    }

    int rules_count = 0, updates_count = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';  // Enlever le saut de ligne

        // Changer de section si une ligne vide est rencontrée
        if (strcmp(line, "") == 0) {
            is_rules_section = 0;
            continue;
        }

        if (is_rules_section) {
            // Lecture des règles
            int before, after;
            if (sscanf(line, "%d|%d", &before, &after) == 2) {
                (*rules)[rules_count].before = before;
                (*rules)[rules_count].after = after;
                rules_count++;
            }
        } else {
            // Lecture des mises à jour
            int *update = (int *)malloc(256 * sizeof(int));  // Allocation dynamique pour une mise à jour
            int num_pages = 0;
            char *token = strtok(line, ",");
            while (token != NULL) {
                update[num_pages++] = atoi(token);
                token = strtok(NULL, ",");
            }
            // Terminer la mise à jour avec 0 (pour faciliter le débogage)
            update[num_pages] = 0; 
            (*updates)[updates_count] = update;
            updates_count++;
        }
    }

    *num_rules = rules_count;
    *num_updates = updates_count;

    return 0;
}

// Fonction pour vérifier si une mise à jour est dans le bon ordre
int is_correct_order(int *update, int num_pages, Rule *rules, int num_rules) {
    for (int i = 0; i < num_rules; i++) {
        int before = rules[i].before;
        int after = rules[i].after;

        // Trouver les indices des pages dans la mise à jour
        int before_index = -1, after_index = -1;
        for (int j = 0; j < num_pages; j++) {
            if (update[j] == before) before_index = j;
            if (update[j] == after) after_index = j;
        }

        // Affichage pour débogage
        printf("Règle : %d|%d, indices : before = %d, after = %d\n", before, after, before_index, after_index);

        // Si la page avant est après la page après, la mise à jour est incorrecte
        if (before_index != -1 && after_index != -1 && before_index > after_index) {
            printf("Ordre incorrect : %d avant %d\n", before, after);
            return 0;  // Pas dans le bon ordre
        }
    }
    return 1;  // Mise à jour correcte
}

// Fonction pour calculer la somme des pages centrales des mises à jour correctes
int calculate_middle_pages_sum(int **updates, int num_updates, Rule *rules, int num_rules) {
    int sum = 0;
    for (int i = 0; i < num_updates; i++) {
        int *update = updates[i];
        int num_pages = 0;
        while (update[num_pages] != 0) num_pages++;  // Compter le nombre de pages

        // Affichage pour débogage
        printf("Mise à jour %d : ", i + 1);
        for (int j = 0; j < num_pages; j++) {
            printf("%d ", update[j]);
        }
        printf("\n");

        // Vérifier si la mise à jour est dans le bon ordre
        if (is_correct_order(update, num_pages, rules, num_rules)) {
            int middle_index = num_pages / 2;
            sum += update[middle_index];
            printf("Page centrale ajoutée à la somme : %d\n", update[middle_index]);
        }
        // Libérer la mémoire allouée pour la mise à jour
        // free(update);
    }

    return sum;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s.\n", argv[1]);
        return 1;
    }

    Rule *rules = NULL;
    int **updates = NULL;
    int num_rules = 0, num_updates = 0;

    // Parser les règles et mises à jour
    if (parse_input(file, &rules, &num_rules, &updates, &num_updates) == -1) {
        fclose(file);
        return 1;
    }

    // Calculer la somme des pages centrales des mises à jour correctes
    int result = calculate_middle_pages_sum(updates, num_updates, rules, num_rules);
    printf("La somme des pages centrales des mises à jour correctes est : %d\n", result);

    // Libérer la mémoire allouée pour les règles et les mises à jour
    free(rules);
    // printf("ptr rules liberee");
    for (int i = 0; i < num_updates; i++) {
        free(updates[i]);
        // printf("ptr updates[%d] liberee",i);
    }
    free(updates);
    // printf("ptr updates liberee");

    fclose(file);

    return 0;
}
