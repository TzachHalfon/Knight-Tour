#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "knights_tour.h"
#include "colourmod.h"

void TestAllFunc();
void TestKnightTour(unsigned char *brute_path, int X, int Y);
int TestWarnsdorff(unsigned char *warnsdorff_path, int X, int Y);
void PrettyPrinting(unsigned char *);
void PrintArr(char *, int);

int main()
{
    TestAllFunc();
    return (0);
}

void TestAllFunc()
{
    int X = -1, Y = -1;
    int warnsdorff_or_brute = 0;
    int status = 0;
    unsigned char brute_path[64] = {0};
    unsigned char warnsdorff_path[64] = {0};
    clock_t start = 0;
    clock_t between_tests = 0;
    clock_t end = 0;
    system("clear");
    printf("~ START OF KNIGHTS TOUR TEST ~\n");
    printf("Please choose X position 1 - 8 >>\n");
    scanf("%d", &X);
    X -= 1;
    printf("Please choose Y position 1 - 8 >>\n");
    scanf("%d", &Y);
    Y -= 1;
    printf("Checking if position [%d, %d] has a Knights Tour solution..\n", X + 1, Y + 1);
    start = clock();
    status = TestWarnsdorff(warnsdorff_path, X, Y);
    between_tests = clock();
    TestKnightTour(brute_path, X, Y);
    end = clock();
    switch (status)
    {
    case TRUE:
        printf("Position [%d, %d] has a Knights Tour solution!\n", X + 1, Y + 1);
        printf("~~ Brute force runtime: %ld ms ~~\n", (end - between_tests) / (CLOCKS_PER_SEC / 1000));
        printf("~~ Warnsdorff runtime:  %ld ms ~~\n", (between_tests - start) / (CLOCKS_PER_SEC / 1000));
        printf("Which of the solutions would you like to see?\n  1 -> Warnsdorff\n  2 -> Brute force\n  3 -> quit\n");
        scanf("%d", &warnsdorff_or_brute);
        switch (warnsdorff_or_brute)
        {
        case (1):
            PrettyPrinting(warnsdorff_path);
            break;
        case (2):
            PrettyPrinting(brute_path);
            break;
        }
        break;
    case FALSE:
        printf("~~ [%d, %d] doesnt have a Knights Tour solution! ~~\n", X + 1, Y + 1);
        break;
    case TIMEOUT:
        printf("~~ [%d, %d] Timed out! ~~\n", X + 1, Y + 1);
        break;
    }
    printf("~ END OF KNIGHTS TOUR TEST ~\n");
}

int TestWarnsdorff(unsigned char *warnsdorff_path, int X, int Y)
{
    return (WarnsdorffKnightsTour(X, Y, warnsdorff_path));
}

void TestKnightTour(unsigned char *brute_path, int X, int Y)
{
    KnightsTour(X, Y, brute_path);
}

void PrettyPrinting(unsigned char *path)
{
    int i = 0;
    char buffer[70] = {0};
    printf("Printing Knights Tour.....\n");
    for (; i < 64; i++)
    {
        buffer[path[i]] = (i + 1);
        sleep(1);
        system("clear");
        printf("       ~~ step [%d] ~~\n\n", (i + 1));
        PrintArr(buffer, (i + 1));
    }
}

void PrintArr(char *arr, int step)
{
    int i = 0;
    for (; i < 64; ++i)
    {
        for (i = 0; i < 64; i++)
        {
            if (arr[i] == 0)
            {
                printf("%3d", arr[i]);
            }
            else if (step == arr[i])
            {
                PRINT(BOLD, FG_GREEN);
                printf("%3d", arr[i]);
                RESET(0);
            }
            else
            {
                PRINT(BOLD, FG_BLUE);
                printf("%3d", arr[i]);
                RESET(0);
            }
            if ((i + 1) % 8 == 0)
            {
                printf("\n\n");
            }
        }
        printf("\n");
    }
}