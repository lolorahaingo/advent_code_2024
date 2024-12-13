#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 200
#define MAX_COLS 200

int map[MAX_ROWS][MAX_COLS];
int rows, cols;
bool visited[MAX_ROWS][MAX_COLS];

// Directions pour se déplacer (haut, bas, gauche, droite)
int dX[] = {-1, 1, 0, 0};
int dY[] = {0, 0, -1, 1};

// Fonction pour vérifier si un mouvement est valide
bool is_valid(int x, int y, int prev_height) {
    return x >= 0 && x < rows && y >= 0 && y < cols &&
           !visited[x][y] && map[x][y] == prev_height + 1;
}

// Fonction DFS pour explorer un chemin depuis un trailhead
int explore(int x, int y) {
    visited[x][y] = true;
    int score = 0;

    // Si on atteint un sommet de hauteur 9, incrémente le score
    if (map[x][y] == 9) {
        score++;
    }

    // Explore les directions possibles
    for (int i = 0; i < 4; i++) {
        int newX = x + dX[i];
        int newY = y + dY[i];
        if (is_valid(newX, newY, map[x][y])) {
            score += explore(newX, newY);
        }
    }

    return score;
}

// Fonction pour calculer le score total des trailheads
int calculate_trailhead_scores() {
    int total_score = 0;

    // Parcourir la matrice pour trouver les trailheads
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (map[i][j] == 0) {
                // Réinitialiser la matrice des visites
                for (int x = 0; x < rows; x++) {
                    for (int y = 0; y < cols; y++) {
                        visited[x][y] = false;
                    }
                }

                // Explorer le sentier depuis ce trailhead
                total_score += explore(i, j);
            }
        }
    }

    return total_score;
}

// Fonction pour lire la carte à partir d'un fichier
void read_map(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    rows = 0;
    char line[MAX_COLS + 2]; // Ligne avec \n et \0
    while (fgets(line, sizeof(line), file)) {
        cols = strlen(line);
        if (line[cols - 1] == '\n') {
            line[--cols] = '\0';
        }

        for (int j = 0; j < cols; j++) {
            map[rows][j] = line[j] - '0';
        }
        rows++;
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Lire la carte depuis le fichier
    const char *filename = argv[1];
    read_map(filename);

    // Calculer le score total des trailheads
    int total_score = calculate_trailhead_scores();

    // Afficher le résultat
    printf("Score total des trailheads : %d\n", total_score);

    return 0;
}
