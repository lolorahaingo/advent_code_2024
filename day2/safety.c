#include <stdlib.h>
#include "safety.h"

int is_safe_report(int* levels, int size) {
    if (size < 2) return 0;

    int increasing = levels[1] > levels[0];
    for (int i = 1; i < size; i++) {
        int diff = levels[i] - levels[i - 1];
        if (diff < -3 || diff > 3) return 0; // Écart non valide
        if (((levels[i] > levels[i - 1]) != increasing) || ((levels[i] < levels[i - 1]) == increasing)) return 0; // Changement de direction
    }
    return 1;
}

int count_safe_reports(int** reports, int* report_sizes, int report_count) {
    int safe_count = 0;
    for (int i = 0; i < report_count; i++) {
        if (is_safe_report(reports[i], report_sizes[i])) {
            safe_count++;
        }
    }
    return safe_count;
}

