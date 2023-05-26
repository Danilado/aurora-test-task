#include "my_argparse.h"

int init_args_s(args_struct *args)
{
    args->filled = 0;
    return 0;
}

int parse_args(args_struct *args, int argc, char **argv)
{
    opterr = 0;
    int option;
    while ((option = getopt(argc, argv, "u:g:p:h")) != -1)
    {
        switch (option)
        {
        case 'h':
            return HELP_EXIT;
            break;
        case 'u':
            args->username = optarg;
            args->filled |= UNAME_FILLED;
            break;
        case 'g':
            args->group_name = optarg;
            args->filled |= GROUP_FILLED;
            break;
        case 'p':
            args->path = optarg;
            args->filled |= PATH_FILLED;
            break;
        case '?':
            return BAD_ARG_EXIT | optopt;
            break;
        }
    }

    return 0;
}

int print_missing_args(int filled)
{
    int counter = 0;

    for (int i = 1; i < (1 << 3); i <<= 1)
    {
        if (filled & i)
            continue;

        ++counter;
        switch (i)
        {
        case UNAME_FILLED:
            printf(MISSING_ARG_TEXT, 'u');
            break;
        case GROUP_FILLED:
            printf(MISSING_ARG_TEXT, 'g');
            break;
        case PATH_FILLED:
            printf(MISSING_ARG_TEXT, 'p');
            break;
        default:
            return -1;
        }
    }

    return counter;
}
