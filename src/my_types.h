#ifndef MY_TYPES_H
#define MY_TYPES_H

#include <linux/limits.h>

// Максимальный размер (в байтах) имени пользователя или группы в Linux - 255
// https://www.ibm.com/docs/ru/aix/7.1?topic=accounts-user-group-name-length-limit
#define MAX_USERNAME_LEN 255
#define MAX_GROUP_NAME_LEN 255

typedef char file_path_t[PATH_MAX + 1];
typedef char filename_t[NAME_MAX + 1];

typedef char username_t[MAX_USERNAME_LEN + 1];
typedef char group_name_t[MAX_GROUP_NAME_LEN + 1];

#endif
