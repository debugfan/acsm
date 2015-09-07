#include "acsmx.h"

/*
*  Text Data Buffer
*/
unsigned char text[512];

/*
*    A Match is found
*/
int
MatchFound(unsigned id, int index, void *data)
{
    fprintf(stdout, "%s\n", (char *)id);
    return 0;
}


/*
*
*/
int
main(int argc, char **argv)
{
    int i, nocase = 0;
    ACSM_STRUCT * acsm;
    if (argc < 3)

    {
        fprintf(stderr,
            "Usage: acsmx pattern word-1 word-2 ... word-n  -nocase\n");
        exit(0);
    }
    acsm = acsmNew(NULL, NULL, NULL);
    strcpy(text, argv[1]);
    for (i = 1; i < argc; i++)
        if (strcmp(argv[i], "-nocase") == 0)
            nocase = 1;
    for (i = 2; i < argc; i++)

    {
        if (argv[i][0] == '-')
            continue;
        acsmAddPattern(acsm, argv[i], strlen(argv[i]), nocase, 0, 0, 0,
            argv[i], i - 2);
    }
    acsmCompile(acsm, NULL, NULL);
    acsmSearch(acsm, text, strlen(text), (void *)MatchFound, (void *)0, NULL);
    acsmFree(acsm);
    printf("normal pgm end\n");
    return (0);
}
