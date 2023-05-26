#define _GNU_SOURCE

#include <dirent.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "my_argparse.h"

// clang-format off

#define NO_ARGS_EXIT        0x18    // 0b00011000
#define MISSING_ARGS_EXIT   0x20    // 0b00100000
#define USER_NOT_FOUND_EXIT 0x28    // 0b00101000

#define IGNORED_DIR_COUNT 2
#define IGNORED_DIRS "/sys", "/proc"

#define RELATIVE_PATH_COUNT 2
#define RELATIVE_PATHS ".", ".."

// clang-format on

char *ignored_dirs_arr[IGNORED_DIR_COUNT] = {IGNORED_DIRS};
char *relative_paths_arr[RELATIVE_PATH_COUNT] = {RELATIVE_PATHS};

int is_ignored(const char *dirname, const char *filename)
{
    for (size_t i = 0; i < IGNORED_DIR_COUNT; ++i)
        if (!strcmp(dirname, ignored_dirs_arr[i]))
            return 1;
    for (size_t i = 0; i < RELATIVE_PATH_COUNT; ++i)
        if (!strcmp(filename, relative_paths_arr[i]))
            return 1;
    return 0;
}

int permcheck(const char *filename, struct passwd *userstat)
{

    struct stat filestat;
    lstat(filename, &filestat);

    if (filestat.st_mode & S_IWOTH)
        return 1;

    if ((filestat.st_uid == userstat->pw_uid) && filestat.st_mode & S_IWUSR)
        return 1;

    return 0;
}

int print_writeable_rec(char *dirname, struct passwd *userstat)
{
    if (dirname[strlen(dirname) - 1] == '/')
        dirname[strlen(dirname) - 1] = '\0';

    int filecount = 0;

    DIR *dir = opendir(dirname);
    if (dir == NULL)
        return -1;

    struct dirent *ent;

    for (ent = readdir(dir); ent != NULL; ent = readdir(dir))
    {
        file_path_t path = {0};
        strncat(path, dirname, PATH_MAX);
        strncat(path, "/", PATH_MAX);
        strncat(path, ent->d_name, PATH_MAX);

        if (is_ignored(path, ent->d_name))
            continue;

        if (permcheck(path, userstat))
        {
            ++filecount;
            printf("%c %s\n", (ent->d_type == DT_DIR) ? 'd' : 'f', path);

            if (ent->d_type == DT_DIR)
                filecount += print_writeable_rec(path, userstat);
        }
    }

    closedir(dir);

    return filecount;
}

int main(int argc, char **argv)
{
    args_struct args;

    if (!(argc - 1))
    {
        puts(HELP_TEXT);
        return NO_ARGS_EXIT;
    }

    int rc;

    rc = parse_args(&args, argc, argv);

    if (rc == HELP_EXIT)
    {
        puts(HELP_TEXT);
        return EXIT_SUCCESS;
    }

    if (rc & BAD_ARG_EXIT)
    {
        printf(UNKNOWN_ARG_TEXT, (char)(rc & UCHAR_MAX));
        return rc;
    }

    if (args.filled != ALL_FILLED)
    {
        print_missing_args(args.filled);
        puts("\n" HELP_TEXT);
        return MISSING_ARGS_EXIT | args.filled;
    }

    struct passwd *userstats;

    userstats = getpwnam(args.username);

    if (userstats == NULL)
    {
        printf("usename %s not found", args.username);
        return USER_NOT_FOUND_EXIT;
    }

    print_writeable_rec(args.path, userstats);

    return EXIT_SUCCESS;
}
