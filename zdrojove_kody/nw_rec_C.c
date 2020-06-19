/*
 *   implementation 1.4 -- recursive implementation, memoization 2D array memCon, extended gap, endgap penalization
 *
 *   AUTHOR__Dominika Draesslerová
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define GAP -10
#define EXTENDEDGAP -1
#define ENDGAP 0

#define Cindex 0
#define Tindex 1
#define Aindex 2
#define Gindex 3

//  global variables
int scoreMatrix[4][4] = {{5, -4, -4, -4}, {-4, 5, -4, -4}, {-4, -4, 5, -4}, {-4, -4, -4, 5}};

typedef struct result
{
    int score;
    char *out1;
    char *bind;
    char *out2;
} Result;

Result **memCon;

/*
* hfunction - print Result
*/
void printResult(Result result)
{
    printf("%s\n%s\n%s\n", result.out1, result.bind, result.out2);
}

/*
* score from scoreMatrix
*/
int score(char a, char b)
{
    switch (a)
    {
    case 'C':
        switch (b)
        {
        case 'C':
            return scoreMatrix[Cindex][Cindex];
            break;
        case 'T':
            return scoreMatrix[Cindex][Tindex];
            break;
        case 'A':
            return scoreMatrix[Cindex][Aindex];
            break;
        case 'G':
            return scoreMatrix[Cindex][Gindex];
            break;
        default:
            break;
        }

        break;
    case 'T':
        switch (b)
        {
        case 'C':
            return scoreMatrix[Tindex][Cindex];
            break;
        case 'T':
            return scoreMatrix[Tindex][Tindex];
            break;
        case 'A':
            return scoreMatrix[Tindex][Aindex];
            break;
        case 'G':
            return scoreMatrix[Tindex][Gindex];
            break;
        default:
            break;
        }

        break;
    case 'A':
        switch (b)
        {
        case 'C':
            return scoreMatrix[Aindex][Cindex];
            break;
        case 'T':
            return scoreMatrix[Aindex][Tindex];
            break;
        case 'A':
            return scoreMatrix[Aindex][Aindex];
            break;
        case 'G':
            return scoreMatrix[Aindex][Gindex];
            break;
        default:
            break;
        }
        break;
    case 'G':
        switch (b)
        {
        case 'C':
            return scoreMatrix[Gindex][Cindex];
            break;
        case 'T':
            return scoreMatrix[Gindex][Tindex];
            break;
        case 'A':
            return scoreMatrix[Gindex][Aindex];
            break;
        case 'G':
            return scoreMatrix[Gindex][Gindex];
            break;
        default:
            break;
        }
        break;
    default:
        return 0;
        break;
    }
    return 0;
}

/*
*   find best score from all posibilities calculated from scoreArray
*/
int maxInOpt(Result *opt)
{
    int max = 0;
    for (int i = 0; i < 3; i++)
        if (opt[i].score > opt[max].score)
            max = i;
    return max;
}

/*
*   free memory of unsaved results
*/
void tidyUp(int max, Result *opt)
{
    switch (max)
    {
    case 0:
        free(opt[1].out1);
        free(opt[1].bind);
        free(opt[1].out2);
        free(opt[2].out1);
        free(opt[2].bind);
        free(opt[2].out2);
        break;
    case 1:
        free(opt[0].out1);
        free(opt[0].bind);
        free(opt[0].out2);
        free(opt[2].out1);
        free(opt[2].bind);
        free(opt[2].out2);
        break;
    case 2:
        free(opt[1].out1);
        free(opt[1].bind);
        free(opt[1].out2);
        free(opt[0].out1);
        free(opt[0].bind);
        free(opt[0].out2);
        break;
    default:
        break;
    }
}

/*
*   create new result 
*/
Result newResult(int lenght, const char *s1, const char *s2, int lens1, int lens2, int i, int j)
{
    Result result;
    result.score = 0;
    result.out1 = (char *)malloc((lenght + lens1 - i + 1) * sizeof(char));
    result.bind = (char *)malloc((lenght + 1) * sizeof(char));
    result.out2 = (char *)malloc((lenght + lens2 - j + 1) * sizeof(char));

    for (int k = 0; k < lenght; k++)
    {
        result.out1[k] = '.';
        result.bind[k] = '.';
        result.out2[k] = '.';
    }

    result.bind[lenght] = '\0';

    int x = lenght;
    int y = lenght;

    while (i != lens1)
    {
        result.out1[x] = s1[i];
        i++;
        x++;
    }
    result.out1[x] = '\0';

    while (j != lens2)
    {
        result.out2[y] = s2[j];
        j++;
        y++;
    }
    result.out2[y] = '\0';

    return result;
}

/*
*   return gap penalty, endgap if string not started yet, extendedgap if gap already started else gap penalty
*/
int getGapPenalty(char *s1, char *s2, int depth, int opt, int endgap, int lens1, int lens2)
{
    if (depth == 0)
        return ENDGAP;

    switch (opt)
    {
    case 1:
        if (depth >= strlen(s1))
        {
            return GAP;
        }
        else if (endgap == 0)
        {
            return ENDGAP;
        }
        else if (s1[depth - 1] == '-')
        {
            return EXTENDEDGAP;
        }
        else
        {
            return GAP;
        }
        break;

    case 2:
        if (depth >= strlen(s2))
        {
            return GAP;
        }
        else if (endgap == 0)
        {
            return ENDGAP;
        }
        else if (s2[depth - 1] == '-')
        {
            return EXTENDEDGAP;
        }
        else
        {
            return GAP;
        }
        break;

    default:
        return 0;
        break;
    }
    return 0;
}

/*
*   save result to memory 2dtable
*/
void saveResult(Result opt, int i, int j)
{
    free(memCon[i][j].out1);
    free(memCon[i][j].bind);
    free(memCon[i][j].out2);

    memCon[i][j].out1 = (char *)malloc((strlen(opt.out1) + 1) * sizeof(char));
    memCon[i][j].bind = (char *)malloc((strlen(opt.bind) + 1) * sizeof(char));
    memCon[i][j].out2 = (char *)malloc((strlen(opt.out2) + 1) * sizeof(char));
    strcpy(memCon[i][j].out1, opt.out1);
    strcpy(memCon[i][j].bind, opt.bind);
    strcpy(memCon[i][j].out2, opt.out2);
    memCon[i][j].score = opt.score;
}

/*
*   return existing resut from memory 2dtable
*/
Result getResult(int i, int j)
{
    Result result;
    result.out1 = (char *)malloc((strlen(memCon[i][j].out1) + 1) * sizeof(char));
    result.bind = (char *)malloc((strlen(memCon[i][j].bind) + 1) * sizeof(char));
    result.out2 = (char *)malloc((strlen(memCon[i][j].out2) + 1) * sizeof(char));
    strcpy(result.out1, memCon[i][j].out1);
    strcpy(result.bind, memCon[i][j].bind);
    strcpy(result.out2, memCon[i][j].out2);
    result.score = memCon[i][j].score;

    return result;
}

void printMemcon(int lens1, int lens2)
{
    for (int i = 0; i < lens1; i++)
    {
        for (int j = 0; j < lens2; j++)
        {
            printf("%d ", memCon[i][j].score);
            // printResult(memCon[i][j]);
        }
        printf("\n");
    }
}

/*
*   major calculation function
*/
Result alignSequence(char *s1, char *s2, int lens1, int lens2, int i, int j)
{
    //  check actual recursion depth
    static int depth = -1;

    depth++;

    // stop condition 
    if (i == lens1 || j == lens2)
        return newResult(depth--, s1, s2, lens1, lens2, i, j);

    // already calculated?
    if (memCon[i][j].score != -1)
    {
        depth--;
        return getResult(i, j);
    }

    //  calculate new value
    Result opt[3];

    //  get options
    opt[0] = alignSequence(s1, s2, lens1, lens2, i + 1, j + 1);
    opt[1] = alignSequence(s1, s2, lens1, lens2, i + 1, j);
    opt[2] = alignSequence(s1, s2, lens1, lens2, i, j + 1);

    //  edit score
    opt[0].score += score(s1[i], s2[j]);
    if (j == 0)
        opt[1].score += getGapPenalty(s1, s2, depth, 1, 0, lens1, lens2);
    else
        opt[1].score += getGapPenalty(s1, s2, depth, 1, 1, lens1, lens2);
    if (i == 0)
        opt[2].score += getGapPenalty(s1, s2, depth, 2, 0, lens1, lens2);
    else
        opt[2].score += getGapPenalty(s1, s2, depth, 2, 1, lens1, lens2);

    int max = maxInOpt(opt);

    //  edit sequences
    switch (max)
    {
    case 0:
        /* diagonal */
        opt[0].out1[depth] = s1[i];
        opt[0].bind[depth] = (s1[i] == s2[j]) ? '|' : '.';
        opt[0].out2[depth] = s2[j];
        break;
    case 1:
        /* up */
        opt[1].out1[depth] = s1[i];
        opt[1].bind[depth] = ' ';
        opt[1].out2[depth] = '-';
        break;
    case 2:
        /* left */
        opt[2].out1[depth] = '-';
        opt[2].bind[depth] = ' ';
        opt[2].out2[depth] = s2[j];
        break;
    default:
        break;
    }

    //  get best options, save and return best result
    if (memCon[i][j].score == -1)
        saveResult(opt[max], i, j);

    tidyUp(max, opt);

    depth--;
    return opt[max];
}

/*
*   MAIN
*/
int main(int argc, char const *argv[])
{
    //  load sequences from file
    int lens1 = 10;
    int lens2 = 10;

    char *s1 = (char *)malloc(lens1 * sizeof(char));
    char *s2 = (char *)malloc(lens2 * sizeof(char));
    s1[0] = s2[0] = ' ';

    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Can't open input file in.list!\n");
        return 1;
    }
    char c;
    int p = 0;
    int i = 0;
    int j = 0;
    while (fscanf(input, "%c", &c) != EOF)
    {
        if (c == '\n')
        {
            s1[i] = '\0';
            p = 1;
            continue;
        }
        if (p == 0)
        {
            if (i >= lens1 - 1)
            {
                lens1 *= 2;
                s1 = (char *)realloc(s1, lens1);
            }
            s1[i] = c;
            i++;
        }
        else
        {
            if (j >= lens2 - 1)
            {
                lens2 *= 2;
                s2 = (char *)realloc(s2, lens2);
            }
            s2[j] = c;
            j++;
        }
    }
    s2[j] = '\0';

    fclose(input);

    clock_t start = clock();

    printf("s1: %s\ns2: %s\n", s1, s2);

    lens1 = i;
    lens2 = j;
    printf("lenght of s1: %d\n", lens1 - 1);
    printf("lenght of s2: %d\n", lens2 - 1);

    //  create memoization 2d table
    memCon = (Result **)malloc(lens1 * sizeof(Result *));
    for (int i = 0; i < lens1; i++)
    {
        memCon[i] = (Result *)malloc(lens2 * sizeof(Result));
        for (int j = 0; j < lens2; j++)
        {
            memCon[i][j].score = -1;
            memCon[i][j].out1 = NULL;
            memCon[i][j].bind = NULL;
            memCon[i][j].out2 = NULL;
        }
    }

    //  start calculation
    Result result = alignSequence(s1, s2, lens1, lens2, 0, 0);

    clock_t end = clock();
    double time_spent = (double)(end - start);

    printResult(result);
    printf("score: %d\n", result.score);
    printf("time: %lf\n", time_spent);

    //  tidy up
    for (int i = 0; i < lens1; i++)
    {
        for (int j = 0; j < lens2; j++)
        {
            if (memCon[i][j].out1)
                free(memCon[i][j].out1);
            if (memCon[i][j].bind)
                free(memCon[i][j].bind);
            if (memCon[i][j].out2)
                free(memCon[i][j].out2);
        }
        free(memCon[i]);
    }
    free(memCon);

    free(s1);
    free(s2);
    free(result.out1);
    free(result.bind);
    free(result.out2);

    return 0;
}
