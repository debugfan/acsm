#include "acsmx.h"
#include <stdarg.h>

/*
*    A Match is found
*/
int
MatchFound(unsigned id, int index, void *data)
{
    fprintf(stdout, "%s\n", (char *)id);
    return 0;
}

int test_ac(unsigned char *text, ...)
{
    va_list va_li;
    int idx, nocase = 0;
    ACSM_STRUCT * acsm;
    unsigned char *pat;
    idx = 1;
    pat = NULL;
    int state = 0;

    acsm = acsmNew(NULL, NULL, NULL);
    va_start(va_li, text);     /* Initialize variable arguments. */
    for (;;)
    {
        pat = va_arg(va_li, unsigned char *);
        if (pat == NULL)
        {
            break;
        }
        acsmAddPattern(acsm, pat, strlen(pat), nocase, 0, 0, 0,
            pat, idx++);
    }
    va_end(va_li);              /* Reset variable arguments.      */

    acsmCompile(acsm, NULL, NULL);
    acsmSearch(acsm, text, strlen(text), (void *)MatchFound, (void *)0, &state);
    acsmFree(acsm);
    return 0;
}

int unit_test()
{
    test_ac("this a simple test", NULL);
    test_ac("this a simple test", "this", NULL);
    test_ac("this a simple test", "sim", NULL);
    test_ac("this a simple test", "thiz", "sim", NULL);
    test_ac("this a simple test", "this", "sip", NULL);
    test_ac("this a simple test", "this", "sim", NULL);
    test_ac("this a simple test", "", NULL);
    test_ac("this a simple test", "test", NULL);
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
        acsmAddPattern(acsm, argv[i], strlen(argv[i]), nocase, 0, 0, 0,
            argv[i], i - 2);
    }
    acsmCompile(acsm, NULL, NULL);
    acsmSearch(acsm, text, strlen(text), (void *)MatchFound, (void *)0, NULL);
    acsmFree(acsm);
    printf("normal pgm end\n");
    return (0);
#endif
}
