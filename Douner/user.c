#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "user.h"

bool user_exists(const char* name) {
    FILE* f = fopen("users.txt", "r");
    if (!f) return false;

    char buffer[50];

    while (fgets(buffer, sizeof(buffer), f)) {
        buffer[strcspn(buffer, "\n")] = 0; // ???? ????

        if (strcmp(buffer, name) == 0) {
            fclose(f);
            return true;
        }
    }

    fclose(f);
    return false;
}

void add_user(const char* name) {
    FILE* f = fopen("users.txt", "a"); // append ???
    if (!f) return;

    fprintf(f, "%s\n", name);
    fclose(f);
}