#include "acsmx.h"
#include <stdarg.h>

/*
*    A Match is found
*/
int
MatchFound(const unsigned char *pat, int n, void *id, int index, void *data)
{
    fprintf(stdout, "%s\n", (char *)id);
    return 0;
}

int test_ac(unsigned char *text, int nocase, ...)
{
    va_list va_li;
    ACSM_STRUCT * acsm = NULL;
    unsigned char *pat = NULL;
    int idx = 1;
    int *match_table;
    int state = 0;

    acsm = acsmNew(NULL, NULL, NULL);
    va_start(va_li, nocase);     /* Initialize variable arguments. */
    for (;;)
    {
        pat = va_arg(va_li, unsigned char *);
        if (pat == NULL)
        {
            break;
        }
        acsmAddPatternExtended(acsm, pat, strlen(pat), nocase, 0, 0, 0,
            pat, idx++);
    }
    va_end(va_li);              /* Reset variable arguments.      */

    acsmCompile(acsm, NULL, NULL);
    match_table = malloc(sizeof(int) * acsmGetMatchTableNumbers(acsm));
    acsmResetMatchTable(acsm, match_table);
    acsmSearch(acsm, text, 2, (void *)MatchFound, (void *)0, &state, match_table);
    acsmSearch(acsm, text + 2, strlen(text) - 2, (void *)MatchFound, (void *)0, &state, match_table);
    acsmFree(acsm);
    return 0;
}

int unit_test()
{
	test_ac("this a simple test", 0, "s?m??", NULL);
	test_ac("this a simple test", 0, "tes??", NULL);
    test_ac("this a simple test", 0, NULL);
    test_ac("this a simple test", 0, "this", "is", NULL);
    test_ac("this a simple test", 0, "sim", NULL);
    test_ac("this a simple test", 0, "thiz", "sim", NULL);
    test_ac("this a simple test", 0, "this", "sip", NULL);
    test_ac("this a simple test", 0, "this", "sim", NULL);
    test_ac("this a simple test", 0, "", NULL);
    test_ac("this a simple test", 0, "TEST", NULL);
    test_ac("this a simple test", 1, "TEST", NULL);
    test_ac("ABABababaBaBa", 0, 0, "ababa", NULL);
    test_ac("Match me with wildcard", 0, "Match * with", NULL);
    test_ac("Match me with wildcard", 0, "Match \\* with", NULL);
    test_ac("ababa", 0, "ababa", NULL);
    return 0;
}

/*
*
*/
int
main(int argc, char **argv)
{
#define UNIT_TEST
#ifdef UNIT_TEST
    return unit_test();
#else
    /*
    *  Text Data Buffer
    */
    unsigned char text[512];

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
        acsmAddPatternExtended(acsm, argv[i], strlen(argv[i]), nocase, 0, 0, 0,
            argv[i], i - 2);
    }
    acsmCompile(acsm, NULL, NULL);
    acsmSearch(acsm, text, strlen(text), (void *)MatchFound, (void *)0, NULL);
    acsmFree(acsm);
    printf("normal pgm end\n");
    return (0);
#endif
}
