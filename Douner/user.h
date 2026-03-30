#ifndef USER_H
#define USER_H

#include <stdbool.h>

bool user_exists(const char* name);
void add_user(const char* name);

#endif
