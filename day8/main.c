#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the point structure
typedef struct {
    char x;
    char y;
    char type;
} point_s;

// Function to read and parse the file, returning the dynamic array of Points
point_s *parseFile(const char *filename, int *width, int *height, int *nb_points) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }
    *nb_points = 1;
    char c;
    char x = 0, y = 0;
    int i = 0;
    point_s *points = malloc(*nb_points * sizeof(point_s));
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            y++;
            *width = x;
            x = 0;
            continue;
        }

        if (c != '.') {
            if (i + 1 > *nb_points) {
                *nb_points = i + 1;
                points = realloc(points, *nb_points * sizeof(point_s));
            }
            points[i].type = c;
            points[i].x = x;
            points[i].y = y;
            // printf("Point added: type=%c, x=%d, y=%d\n", c, x, y);
            i++;
        }

        x++;
    }
    *height = y;

    fclose(file);

    return points;
}

// Comparison function for qsort
int comparePoints(const void *a, const void *b) {
    const point_s *pointA = (const point_s *)a;
    const point_s *pointB = (const point_s *)b;
    return (pointA->type - pointB->type);
}

// Function to sort the points array by type
void sort_type(point_s *points, size_t count) {
    qsort(points, count, sizeof(point_s), comparePoints);
}

// Function to print the antinodes
void print_antinodes(point_s *antinodes, size_t count) {
    // printf("Antinodes:\n");
    for (size_t i = 0; i < count; i++) {
        // printf("Antinode %zu: type=%c, x=%d, y=%d\n", i, antinodes[i].type, antinodes[i].x, antinodes[i].y);
    }
}

// Fonction pour vérifier si un antinœud existe déjà
char antinode_exists(point_s* antinodes, size_t count, float x, float y) {
    for (size_t i = 0; i < count; i++) {
        if (antinodes[i].x == x && antinodes[i].y == y) {
            return 1;
        }
    }
    return 0;
}

// Function to find and return antinodes
point_s *find_pairs_n_add_antinodes(point_s *points, size_t count, size_t *antinodes_count) {
    size_t k = 0;
    size_t capacity = 2;
    point_s *antinodes = malloc(capacity * sizeof(point_s));
    for (size_t i = 0; i < count; i++) {
        for (size_t j = i + 1; j < count; j++) {
            if (points[i].type == points[j].type) {
                // Calcul des antinœuds
                float x1 = (points[i].x - points[j].x) + points[i].x;
                float y1 = (points[i].y - points[j].y) + points[i].y;
                float x2 = (points[j].x - points[i].x) + points[j].x;
                float y2 = (points[j].y - points[i].y) + points[j].y;

                // Vérifie si l'antinœud existe déjà avant de l'ajouter
                if (!antinode_exists(antinodes, k, x1, y1)) {
                    if (k + 1 > capacity) {
                        capacity *= 2;
                        antinodes = realloc(antinodes, capacity * sizeof(point_s));
                    }
                    antinodes[k].x = x1;
                    antinodes[k].y = y1;
                    antinodes[k++].type = '#';
                }

                if (!antinode_exists(antinodes, k, x2, y2)) {
                    if (k + 1 > capacity) {
                        capacity *= 2;
                        antinodes = realloc(antinodes, capacity * sizeof(point_s));
                    }
                    antinodes[k].x = x2;
                    antinodes[k].y = y2;
                    antinodes[k++].type = '#';
                }
            }
        }
    }
    *antinodes_count = k;
    return antinodes;
}

int count_antinodes(point_s *antinodes, size_t count, int width, int height) {
    int counter = 0;
    for (size_t i = 0; i < count; i++) {
        if (antinodes[i].x < width && antinodes[i].y < height && antinodes[i].x >= 0 && antinodes[i].y >= 0) {
            counter++;
            printf("Antinode %zu: type=%c, x=%d, y=%d\n", i, antinodes[i].type, antinodes[i].x, antinodes[i].y);
        }
    }
    return counter;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int width = 0, height = 0, nb_antenna = 0;
    point_s *points = parseFile(argv[1], &width, &height, &nb_antenna);
    if (!points) {
        return EXIT_FAILURE;
    }

    printf("Total points to sort: %d\n", nb_antenna);
    sort_type(points, nb_antenna);

    size_t antinodes_count = 0;
    point_s *antinodes = find_pairs_n_add_antinodes(points, nb_antenna, &antinodes_count);

    if (antinodes_count > 0) {
        print_antinodes(antinodes, antinodes_count);
        printf("%d %d\n", width, height);
        int valid_count = count_antinodes(antinodes, antinodes_count, width, height);
        printf("Valid antinodes within bounds: %d\n", valid_count);
    } else {
        printf("No antinodes found.\n");
    }

    free(antinodes);
    free(points);

    return EXIT_SUCCESS;
}
