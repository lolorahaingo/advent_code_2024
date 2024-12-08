#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Fonction pour analyser et extraire les instructions "mul(X,Y)" depuis un fichier
int process_memory(FILE *file) {
    int total_sum = 0;
    char ch;
    int x, y;

    while ((ch = fgetc(file)) != EOF) { // Lire caractère par caractère
        if (ch == 'm') { // Si on trouve "mul("
            // Vérifier "mul("
            if (fgetc(file) == 'u' && fgetc(file) == 'l' && fgetc(file) == '(') {
                
                // Lire X
                if (fscanf(file, "%d", &x) == 1 && x >= 1 && x <= 999) { // Vérifier que X est entre 1 et 3 chiffres
                    // Vérifier la virgule
                    if (fgetc(file) == ',') {
                        // Lire Y
                        if (fscanf(file, "%d", &y) == 1 && y >= 1 && y <= 999) { // Vérifier que Y est entre 1 et 3 chiffres
                            // Vérifier la parenthèse fermante
                            if (fgetc(file) == ')') {
                                // Calculer le produit et l'ajouter au total
                                total_sum += x * y;
                                printf("Found mul(%d,%d), product = %d\n", x, y, x * y);
                            }
                        }
                    }
                }
            }
        }
    }

    return total_sum;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    // Ouvrir le fichier
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return 1;
    }

    // Traiter la mémoire pour trouver les instructions "mul"
    int result = process_memory(file);
    printf("Sum of all valid multiplications: %d\n", result);

    // Fermer le fichier
    fclose(file);
    return 0;
}
