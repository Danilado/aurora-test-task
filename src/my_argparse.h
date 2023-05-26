#ifndef MY_ARGPARSE
#define MY_ARGPARSE

#include "my_types.h"
#include <getopt.h>
#include <stdio.h>

// clang-format off

#define HELP_TEXT                                                           \
  "mycheckperm\n"                                                           \
  "Использование: mycheckperm [options]\n"                                  \
  "Опции:\n"                                                                \
  "-u <username>         имя пользователя для проверки прав\n"              \
  "-g <groupname>        название группы пользователей для проверки прав\n" \
  "-p <path>             путь к директории для проверки прав\n"             \
  "-h                    выводит это сообщение\n"

#define UNKNOWN_ARG_TEXT                                                    \
  "Ошибка! Нераспознанный аргумент: %c\n"                                   \
  "\n"                                                                      \
  HELP_TEXT

#define MISSING_ARG_TEXT                                                    \
  "Ошибка! Не введено значение аргумента %c\n"

#define HELP_EXIT     0x08  // 0b00001000
#define BAD_ARG_EXIT  0x16  // 0b00010000

#define UNAME_FILLED  0x01  // 0b00000001
#define GROUP_FILLED  0x02  // 0b00000010
#define PATH_FILLED   0x04   //0b00000100
#define ALL_FILLED    (UNAME_FILLED | GROUP_FILLED | PATH_FILLED)

// clang-format on

typedef struct
{
    char *username;
    char *group_name;
    char *path;
    int filled;
} args_struct;

int parse_args(args_struct *args, int argc, char **argv);
int print_missing_args(int filled);
int init_args_s(args_struct *args);

#endif
