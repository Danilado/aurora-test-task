#include <unistd.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "my_argparse.h"

#define NO_ARGS_EXIT 0x00000001
#define MISSING_ARGS_EXIT 0x04000000

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
        return 0;
    }

    if (rc & BAD_ARG_EXIT)
    {
        printf(UNKNOWN_ARG_TEXT, (char)(rc & 0xFF));
        return rc;
    }

    if (args.filled != EVERYTHING_FILLED)
    {
        print_missing_args(args.filled);
        puts("\n" HELP_TEXT);
        return MISSING_ARGS_EXIT | args.filled;
    }

    printf("uname: %s\n", args.username);
    printf("group: %s\n", args.group_name);
    printf("path:  %s\n", args.path);

    return EXIT_SUCCESS;
}