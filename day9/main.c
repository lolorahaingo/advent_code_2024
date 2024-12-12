#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction pour remplir le tableau dynamique avec les IDs des fichiers
void fillDiskMap(const char *line, double long **diskMap, int *size) {
    double long fileId = 0;  // ID des fichiers
    int i;

    // Parcours de chaque caractère dans la chaîne lue
    for (i = 0; i < strlen(line) - 1; i++) { // Ignorer le '\n' à la fin
        if (i % 2 == 0) {  // Prendre uniquement les indices pairs
            char digitChar = line[i];
            int digit = digitChar - '0'; // Convertir le caractère en entier

            // Agrandir le tableau dynamique selon la longueur du fichier
            *diskMap = (double long *)realloc(*diskMap, sizeof(double long) * (*size + digit));

            // Remplir le tableau avec l'ID du fichier, selon la longueur donnée par digit
            for (int j = 0; j < digit; j++) {
                (*diskMap)[*size] = fileId; // Ajouter l'ID du fichier
                (*size)++;  // Incrémenter la taille
            }

            // Passer à l'ID du fichier suivant
            fileId++;
        }
        else {  // Prendre uniquement les indices impairs
            char digitChar = line[i];
            int digit = digitChar - '0'; // Convertir le caractère en entier

            // Agrandir le tableau dynamique selon la longueur du fichier
            *diskMap = (double long *)realloc(*diskMap, sizeof(double long) * (*size + digit));

            // Remplir le tableau avec l'ID du fichier, selon la longueur donnée par digit
            for (int j = 0; j < digit; j++) {
                (*diskMap)[*size] = -1; // Ajouter l'ID du fichier
                (*size)++;  // Incrémenter la taille
            }
        }
    }
}


// Affichage du tableau
void printTab(double long *diskMap, int size) {
    printf("Disk Map: ");
    for (int i = 0; i < size; i++) {
        if (diskMap[i] == -1) printf(".");
        else printf("%.0Lf", diskMap[i]);
    }
    printf("\n");
}

void compressDiskMap(double long *diskMap, int size) {
    char released = 0;
    char stop = 0;
    // printf("InitTab: \n");
    // printTab(diskMap, size);
    // printf("\n");
    for(int i=size-1; i>0; i--) { 
        // printTab(diskMap, size);    
        for(int j=0; j<i;j++) {
            // printf("j:%d ", j);
            if(diskMap[j]==-1) {
                diskMap[j] = diskMap[i];
                diskMap[i] = -1;
                break;
            }
            // if(j==i) stop=1;
        }

        // printTab(diskMap, size);
        if(stop) break;
    }
}

// Fonction pour additionner la multiplication de chaque élément du tableau par son index
long double sumMultiplicationsWithIncrement(double long *diskMap, int size) {
    long double sum = 0;

    printf("\nCalcul des opérations de multiplication...\n");
    for (int i = 0; i < size; i++) {
        if(diskMap[i]==-1) break;
        long double multiplication = diskMap[i] * i;  // Multiplier la valeur à l'indice 'i' par l'indice 'i' lui-même
        // printf("diskMap[%d] = %.0Lf * %d = %.0Lf\n", i, diskMap[i], i, multiplication);
        sum += multiplication;
        
    }

    return sum;
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file1>\n", argv[0]);
        return 1;
    }

    // Ouverture du fichier en mode lecture
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return 1;
    }

    // Trouver la taille du fichier
    fseek(file, 0, SEEK_END); // Déplace le pointeur de fichier à la fin
    long fileSize = ftell(file); // Obtient la position (taille en octets)
    rewind(file); // Remet le pointeur au début

    // Allouer dynamiquement de la mémoire pour le tableau 'line'
    char *line = (char *)malloc((fileSize + 1) * sizeof(char)); // +1 pour le caractère nul '\0'
    if (line == NULL) {
        perror("Erreur d'allocation de mémoire");
        fclose(file);
        return 1;
    }

    // Lecture de la chaîne de caractères depuis le fichier
    fread(line, sizeof(char), fileSize, file);
    line[fileSize] = '\0'; // Assurer que la chaîne est terminée par '\0'
    fclose(file);

    // Affichage de l'input de base
    printf("Input: %s\n", line);

    // Initialisation de la taille du tableau dynamique
    double long *diskMap = NULL; // Tableau dynamique des blocs
    int size = 0;          // Taille actuelle du tableau

    // Appeler la fonction pour remplir le tableau avec les IDs des fichiers
    fillDiskMap(line, &diskMap, &size);
    compressDiskMap(diskMap, size);
    // printTab(diskMap, size);

    // Calculer la somme des multiplications de chaque élément par son index
    long double result = sumMultiplicationsWithIncrement(diskMap, size);

    // Affichage du résultat
    printf("\nSum of multiplications with increment: %.0Lf\n", result);

    // Libération de la mémoire allouée pour le tableau dynamique
    free(line);
    free(diskMap);

    return 0;
}