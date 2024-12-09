#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_ROWS 1000
#define MAX_COLS 1000

// Directions : haut, droite, bas, gauche
int d_row[] = {-1, 0, 1, 0};
int d_col[] = {0, 1, 0, -1};

typedef struct {
    int row, col, dir;
} State;

bool visited[MAX_ROWS][MAX_COLS];

void print_visited(bool visited[MAX_ROWS][MAX_COLS], int rows, int cols) {
    printf("\nCurrent state of visited positions:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (visited[i][j]) {
                printf("X");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void load_map(const char *filename, char map[MAX_ROWS][MAX_COLS], int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    *rows = 0;
    *cols = 0;

    char line[MAX_COLS];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        strcpy(map[*rows], line);
        (*rows)++;
        if (*cols == 0) {
            *cols = strlen(line);
        }
    }
    fclose(file);
}

void print_map(char map[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        printf("%s\n", map[i]);
    }
    printf("\n\n\n");
}

int simulate_guard(char map[MAX_ROWS][MAX_COLS], int rows, int cols) {
    memset(visited, 0, sizeof(visited));
    int visited_count = 0;

    // Trouver la position et la direction initiale
    int row = -1, col = -1, direction = -1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (map[i][j] == '^') direction = 0;
            if (map[i][j] == '>') direction = 1;
            if (map[i][j] == 'v') direction = 2;
            if (map[i][j] == '<') direction = 3;
            if (direction != -1) {
                row = i;
                col = j;
                break;
            }
        }
        if (direction != -1) break;
    }

    if (direction == -1) {
        fprintf(stderr, "Error: No guard position found.\n");
        return -1;
    }

    printf("\nInitial guard position: (%d, %d), direction: %d\n", row, col, direction);

    // Simuler le mouvement
    int steps = 0;
    while (row >= 0 && row < rows && col >= 0 && col < cols) {
        // Debugging : Position actuelle et direction
        printf("Step %d: Position (%d, %d), direction: %d\n", steps, row, col, direction);

        // Marquer la position comme visitée
        if (!visited[row][col]) {
            visited[row][col] = true;
            visited_count++;
            printf("  Marked position (%d, %d) as visited. Total visited: %d\n", row, col, visited_count);
        }

        // Calculer la prochaine position
        int next_row = row + d_row[direction];
        int next_col = col + d_col[direction];
        printf("  Next position to check: (%d, %d)\n", next_row, next_col);

        // Vérifier si la prochaine position est valide
        if (next_row < 0 || next_row >= rows || next_col < 0 || next_col >= cols) {
            printf("  Out of bounds! Breaking simulation.\n");
            break;
        }
        if (map[next_row][next_col] == '#') {
            printf("  Obstacle detected at (%d, %d). Turning right.\n", next_row, next_col);
            direction = (direction + 1) % 4;
        } else {
            row = next_row;
            col = next_col;
        }

        steps++;
        if (steps > 100000) {
            fprintf(stderr, "Infinite loop detected after %d steps. Terminating simulation.\n", steps);
            break;
        }
    }

    return visited_count;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char map[MAX_ROWS][MAX_COLS];
    int rows, cols;
    printf("Avant load map\n");
    load_map(argv[1], map, &rows, &cols);

    printf("Initial map:\n");
    print_map(map, rows, cols);

    int visited = simulate_guard(map, rows, cols);

    printf("\nDistinct positions visited: %d\n", visited);
    return 0;
}
