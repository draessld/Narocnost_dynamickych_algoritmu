
/*
 *   implementation 4.0 -- classic iterative global implementation with reverse calculation, extended gap penalization
 *
 *   AUTHOR__Dominika Draesslerová
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define GAP -10
#define EXTENDEDGAP -1

#define SWAP(x, y) (x ^= y ^= x ^= y)

#define Cindex 0
#define Tindex 1
#define Aindex 2
#define Gindex 3

int scoreMatrix[4][4] = {{5, -4, -4, -4}, {-4, 5, -4, -4}, {-4, -4, 5, -4}, {-4, -4, -4, 5}};

/*
*   find best score from all posibilities calculated from scoreArray
*/
int maxScore(int *opt)
{
    if (opt[0] > opt[1] && opt[0] > opt[2])
        return 0;
    if (opt[1] > opt[2])
        return 1;
    return 2;
}

/*
*   look for max in columns
*/
int getMaxInColumn(int **scoreArray, int i, int j)
{
    int retv = -100000;
    for (int a = i; a > 0; a--)
        if (retv < (scoreArray[i - a][j] + GAP + (EXTENDEDGAP * a)))
            retv = scoreArray[i - a][j] + GAP + (EXTENDEDGAP * a);
    return retv;
}

/*
*   look for max in rows
*/
int getMaxInRow(int **scoreArray, int i, int j)
{
    int retv = -100000;
    for (int a = j; a > 0; a--)
        if (retv < (scoreArray[i][j - a] + GAP + (EXTENDEDGAP * a)))
            retv = scoreArray[i][j - a] + GAP + (EXTENDEDGAP * a);
    return retv;
}

/*
*   score from scoreMatrix
*/
int scoref(char a, char b)
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
*   h-function: reverse string
*/
void reverseStr(char *str, int n)
{
    // Swap character starting from two corners
    for (int i = 0; i < n / 2; i++)
        SWAP(str[i], str[n - i - 1]);
}

/*
*   h-function: print scoreArr
*/
void printScore(int **scoreArr, int lens1, int lens2)
{
    for (int i = 0; i < lens1; i++)
    {
        for (int j = 0; j < lens2; j++)
        {
            printf("%d ", scoreArr[i][j]);
        }
        printf("\n");
    }
}

/*
*   major calculation proccess
*/
int alignSequence(char *s1, char *s2, int lens1, int lens2, int **scoreArr)
{
    //  start time calculation
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    int opt[3];

    //  for each cell in scoreArr calculate 3 options
    for (int i = 1; i < lens1; i++)
        for (int j = 1; j < lens2; j++)
        {
            opt[0] = scoreArr[i - 1][j - 1] + scoref(s1[i], s2[j]);
            opt[1] = getMaxInColumn(scoreArr, i, j);
            opt[2] = getMaxInRow(scoreArr, i, j);

            //  get maximum from 3 options
            int max = maxScore(opt);
            scoreArr[i][j] = opt[max];
        }

    //  backtrace
    int i = lens1 - 1;
    int j = lens2 - 1;
    int k = 0;

    int score = scoreArr[i][j];
    char out1[lens1 + lens2];
    char bind[lens1 + lens2];
    char out2[lens1 + lens2];

    //  create best alignment
    while (i > 0 && j > 0)
    {
        if (scoreArr[i][j] == (scoreArr[i - 1][j - 1] + scoref(s1[i], s2[j])))
        {
            /* diagonal */
            out2[k] = s2[j];
            out1[k] = s1[i];
            if (s1[i] == s2[j])
                bind[k] = '|';
            else
                bind[k] = '.';
            i--;
            j--;
        }
        else if (scoreArr[i][j] == getMaxInColumn(scoreArr, i, j))
        {
            /* up */
            out1[k] = s1[i];
            bind[k] = ' ';
            out2[k] = '-';
            i--;
        }
        else if (scoreArr[i][j] == getMaxInRow(scoreArr, i, j))
        {
            /* left */
            out1[k] = '-';
            bind[k] = ' ';
            out2[k] = s2[j];
            j--;
        }
        k++;
    }

    while (i > 0)
    {
        out1[k] = s1[i];
        bind[k] = ' ';
        out2[k] = '-';
        i--;
        k++;
    }
    
    while (j > 0)
    {
        out1[k] = '-';
        bind[k] = ' ';
        out2[k] = s2[j];
        j--;
        k++;
    }

    //  stop time calculation
    gettimeofday(&stop, NULL);

    //   reverse string
    reverseStr(out1, k);
    reverseStr(bind, k);
    reverseStr(out2, k);
    out1[k] = bind[k] = out2[k] = '\0';

    //  print result
    printf("%s\n%s\n%s\n", out1, bind, out2);
    printf("score: %d\n", score);
    printf("time %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
}

/*
*   MAIN
*/
int main(int argc, char const *argv[])
{
    int lens1 = 10;
    int lens2 = 10;

    char *s1 = (char *)malloc(lens1 * sizeof(char));
    char *s2 = (char *)malloc(lens2 * sizeof(char));
    s1[0] = s2[0] = ' ';

    //  read sequences from file
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Can't open input file in.list!\n");
        return 1;
    }
    char c;
    int p = 0;
    int i = 1;
    int j = 1;
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
            if (i >= lens1)
            {
                lens1 *= 2;
                s1 = (char *)realloc(s1, lens1);
            }
            s1[i] = c;
            i++;
        }
        else
        {
            if (j >= lens2)
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

    printf("s1: %s\ns2: %s\n", s1, s2);

    lens1 = i;
    lens2 = j;
    printf("lenght of s1: %d\n", lens1 - 1);
    printf("lenght of s2: %d\n", lens2 - 1);

    //  declare scoreArray
    int **scoreArr = (int **)malloc((lens1 + 1) * sizeof(int *));
    for (int i = 0; i < lens1 + 1; i++)
    {
        scoreArr[i] = (int *)malloc((lens2 + 1) * sizeof(int));
        for (int j = 0; j < lens2 + 1; j++)
        {
            if (i == 0)
            {
                scoreArr[i][j] = 2;
                scoreArr[i][j] = j * EXTENDEDGAP;
            }
            else if (j == 0)
            {
                scoreArr[i][j] = 1;
                scoreArr[i][j] = i * EXTENDEDGAP;
            }
            else
                scoreArr[i][j] = 0;
        }
    }

    //  start calculation
    alignSequence(s1, s2, lens1, lens2, scoreArr);

    //  tidy up
    for (int i = 0; i < lens1 + 1; i++)
        free(scoreArr[i]);
    free(scoreArr);
    free(s1);
    free(s2);

    return 0;
}