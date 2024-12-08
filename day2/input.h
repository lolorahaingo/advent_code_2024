#ifndef INPUT_H
#define INPUT_H

#define MAX_LEVELS 10 // Peut être redéfini dynamiquement au besoin

int read_reports(const char* filename, int*** reports, int** report_sizes);
void free_reports(int** reports, int* report_sizes, int report_count);

#endif