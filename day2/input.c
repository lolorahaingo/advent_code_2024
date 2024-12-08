#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

int read_reports(const char* filename, int*** reports, int** report_sizes) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        return -1;
    }

    int report_capacity = 10;  // Capacité initiale des rapports
    *reports = malloc(report_capacity * sizeof(int*));
    *report_sizes = malloc(report_capacity * sizeof(int));
    int report_count = 0;

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        int level_capacity = 10;
        int *levels = malloc(level_capacity * sizeof(int));
        int level_count = 0;

        char *token = strtok(line, " \t\n");
        while (token) {
            if (level_count >= level_capacity) {
                level_capacity *= 2;
                levels = realloc(levels, level_capacity * sizeof(int));
            }
            levels[level_count++] = atoi(token);
            token = strtok(NULL, " \t\n");
        }

        if (level_count > 0) {
            if (report_count >= report_capacity) {
                report_capacity *= 2;
                *reports = realloc(*reports, report_capacity * sizeof(int*));
                *report_sizes = realloc(*report_sizes, report_capacity * sizeof(int));
            }
            (*reports)[report_count] = levels;
            (*report_sizes)[report_count] = level_count;
            report_count++;
        } else {
            free(levels);
        }
    }

    fclose(file);
    return report_count;
}

void free_reports(int** reports, int* report_sizes, int report_count) {
    for (int i = 0; i < report_count; i++) {
        free(reports[i]);
    }
    free(reports);
    free(report_sizes);
}

