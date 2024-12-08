#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "safety.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    int** reports = NULL;
    int* report_sizes = NULL;

    int report_count = read_reports(filename, &reports, &report_sizes);
    if (report_count < 0) {
        fprintf(stderr, "Failed to read reports from file: %s\n", filename);
        return 1;
    }

    int safe_count = count_safe_reports(reports, report_sizes, report_count);
    printf("Number of safe reports: %d\n", safe_count);

    free_reports(reports, report_sizes, report_count);
    return 0;
}
