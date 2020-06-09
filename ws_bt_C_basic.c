
/*
 *   implementation 2.4 -- classic iterative implementation with reverse calculation, no extended gap penalty, no endgap penalty
 *
 *   AUTHOR__Dominika Draesslerová
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define GAP -10

#define SWAP(x, y) (x ^= y ^= x ^= y)

#define Gindex 0
#define Cindex 1
#define Aindex 2
#define Tindex 3

/*
*   fill origin score array with zero before calculation
*/
void fillWithZero(int **scoreArr, int n1, int n2)
{
    for (int i = 0; i < n1; i++)
        for (int j = 0; j < n2; j++)
            scoreArr[i][j] = 0;
}

int scoreMatrix[4][4] = {{5, -4, -4, -4}, {-4, 5, -4, -4}, {-4, -4, 5, -4}, {-4, -4, -4, 5}};

/*
* score from scoreMatrix
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
*   find best score from all posibilities calculated from scoreArray
*/
int maxScore(int *opt)
{
    if ((opt[0] > 0) || (opt[1] > 0) || (opt[2] > 0))
    {
        if (opt[0] > opt[1] && opt[0] > opt[2])
            return 0;
        if (opt[1] > opt[2])
            return 1;
    }
    return 3;
}

/*
*   find position of maximums in in score array
*/
void findMaximumInScoreArray(int **scoreArr, int *maximum, int lens1, int lens2)
{
    int max = 0;
    int k = 0;
    for (int i = 0; i < lens1; i++)
    {
        for (int j = 0; j < lens2; j++)
        {
            if (scoreArr[i][j] == max)
            {
                maximum[k] = i;
                maximum[k + 1] = j;
                k += 2;
            }
            if (scoreArr[i][j] > max)
            {
                k = 0;
                max = scoreArr[i][j];
                maximum[k] = i;
                maximum[k + 1] = j;
                k += 2;
            }
        }
    }
    maximum[k] = -1;
}

/*
*   reverse result string
*/
void reverseStr(char *str, int n)
{
    // Swap character starting from two corners
    for (int i = 0; i < n / 2; i++)
        SWAP(str[i], str[n - i - 1]);
}

/*
*   h-functions: print score array
*/
void printScoreArr(int **scoreArr, int n1, int n2)
{
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < n2; j++)
        {
            printf(" %d ", scoreArr[i][j]);
        }
        printf("\n");
    }
}

/*
*   major calculation process
*/
void alignSequence(char *s1, char *s2, int lens1, int lens2, int **scoreArr)
{
    //  start time calculation
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    int opt[4];

    //  for each cell in scoreArr 3 posibilities
    for (int i = 1; i < lens1; i++)
        for (int j = 1; j < lens2; j++)
        {
            opt[0] = scoreArr[i - 1][j - 1] + scoref(s1[i], s2[j]);
            opt[1] = scoreArr[i - 1][j] + GAP;
            opt[2] = scoreArr[i][j - 1] + GAP;
            opt[3] = 0;

            //  get maximum  from 3 options
            int max = maxScore(opt);
            scoreArr[i][j] = opt[max];
        }

    //   get maximum scores in scoreArr
    int maximum[lens1 + lens2];
    findMaximumInScoreArray(scoreArr, maximum, lens1, lens2);
    int score = scoreArr[maximum[0]][maximum[1]];
    int l = 0;
    while (maximum[l] != -1)
    {
        int i = maximum[l];
        int j = maximum[l + 1];
        int k = 0;

        char out1[i + j];
        char bind[i + j];
        char out2[i + j];

        while ((i > 0) && (j > 0) && (scoreArr[i][j] > 0))
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
            else if (scoreArr[i][j] == scoreArr[i - 1][j] + GAP)
            {
                /* up */
                out1[k] = s1[i];
                bind[k] = ' ';
                out2[k] = '-';
                i--;
            }
            else if (scoreArr[i][j] == scoreArr[i][j - 1] + GAP)
            {
                /* left */
                out1[k] = '-';
                bind[k] = ' ';
                out2[k] = s2[j];
                j--;
            }
            k++;
        }
        gettimeofday(&stop, NULL);

        //  reverse strings
        reverseStr(out1, k);
        reverseStr(bind, k);
        reverseStr(out2, k);

        //  print result
        out1[k] = bind[k] = out2[k] = '\0';
        printf("%s\n%s\n%s\n", out1, bind, out2);
        l += 2;
    }
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

    // read sequences from input file
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
            if (i == lens1 - 1)
            {
                s1[i] = '\0';
                lens1 *= 2;
                s1 = (char *)realloc(s1, lens1);
            }
            s1[i] = c;
            i++;
        }
        else
        {
            if (j == lens2 - 1)
            {
                s2[j] = '\0';
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
    int **scoreArr = (int **)malloc(lens1 * sizeof(int *));
    for (int i = 0; i < lens1; i++)
        scoreArr[i] = (int *)malloc(lens2 * sizeof(int));

    fillWithZero(scoreArr, lens1, lens2);

    //  start calculation
    alignSequence(s1, s2, lens1, lens2, scoreArr);

    //  tidy up
    for (int i = 0; i < lens1; i++)
        free(scoreArr[i]);
    free(scoreArr);
    free(s1);
    free(s2);

    return 0;
}