#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WORD "XMAS"
#define WORD_LEN 4

// Dimensions de la grille
#define MAX_ROWS 1000
#define MAX_COLS 1000

// Fonction pour vérifier une occurrence du mot "XMAS" dans une direction donnée
int check_word(char grid[MAX_ROWS][MAX_COLS], int start_row, int start_col, int dir_row, int dir_col) {
    for (int i = 0; i < WORD_LEN; i++) {
        int row = start_row + i * dir_row;
        int col = start_col + i * dir_col;
        if (row < 0 || row >= MAX_ROWS || col < 0 || col >= MAX_COLS || grid[row][col] != WORD[i]) {
            return 0; // Si l'une des lettres ne correspond pas, retourner 0
        }
    }
    return 1; // Si toutes les lettres correspondent, retourner 1
}

void find_word_occurrences(char grid[MAX_ROWS][MAX_COLS], int rows, int cols) {
    int count = 0;

    // Directions possibles pour vérifier les occurrences (horizontal, vertical, diagonale)
    int directions[8][2] = {
        {0, 1},  // Horizontal droit
        {0, -1}, // Horizontal gauche
        {1, 0},  // Vertical bas
        {-1, 0}, // Vertical haut
        {1, 1},  // Diagonale bas droite
        {1, -1}, // Diagonale bas gauche
        {-1, 1}, // Diagonale haut droite
        {-1, -1} // Diagonale haut gauche
    };

    // Parcours de chaque case de la grille
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Si la première lettre du mot "XMAS" correspond, vérifier dans chaque direction
            if (grid[i][j] == 'X') {
                for (int k = 0; k < 8; k++) {
                    if (check_word(grid, i, j, directions[k][0], directions[k][1])) {
                        count++;
                    }
                }
            }
        }
    }

    printf("Le mot 'XMAS' a été trouvé %d fois.\n", count);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s\n", argv[1]);
        return 1;
    }

    char grid[MAX_ROWS][MAX_COLS];
    int row = 0;

    // Lecture du fichier et stockage de la grille
    while (fgets(grid[row], sizeof(grid[0]), file) != NULL) {
        grid[row][strlen(grid[row]) - 1] = '\0';  // Enlever le saut de ligne
        row++;
    }

    fclose(file);

    // Trouver et compter les occurrences de "XMAS"
    find_word_occurrences(grid, row, strlen(grid[0]));

    return 0;
}
