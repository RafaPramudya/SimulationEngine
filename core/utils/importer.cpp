#include "importer.h"

#include <cstdio>
#include <cstdlib>
#include <cassert>

#define MIN_SIZE_BUFFER 0x100
#define MAX_INC_BUFFER 0x400000

char* getContentFromFile(const char* filename) {
    FILE* buffer = fopen(filename, "r");
    #ifdef DEBUG
    printf("Membaca file %s\n", filename);
    #endif
    assert(buffer);

    size_t increase = MIN_SIZE_BUFFER;
    size_t allocated = increase;
    char* string = (char*)malloc(allocated);
    size_t total = 0;

    while (!feof(buffer) && !ferror(buffer)) {
        string[total++] = fgetc(buffer);

        if (total >= allocated) {
            if (increase >= MAX_INC_BUFFER) increase = MAX_INC_BUFFER;
            allocated += increase;
            string = (char*)realloc(string, allocated);
            increase <<= 2;
        }
    }
    assert(!ferror(buffer));

    string = (char*)realloc(string, total);
    string[total - 1] = '\0';

    fclose(buffer);
    return string;
}

void freeContent(char* buffer) {
    free(buffer);
}