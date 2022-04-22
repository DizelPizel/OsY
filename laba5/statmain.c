#include <stdio.h>
#include "mylib.h"
int main()
{
    int choose = -1;
    float A, B, e;
    float a, b;
    while (choose != 3)
    {
        printf("1-Square\n2-Derivative\n3-Exit\n");
        scanf("%d", &choose);
        switch (choose)
        {
        case 1:
        {
            printf("Write A,B: ");
            scanf("%f", &A);
            scanf("%f", &B);
            printf("Square:%f\n", Square(A, B));
            continue;
        }
        case 2:
        {
            printf("Write A and B: ");
            scanf("%f", &a);
            scanf("%f", &b);
            printf("Derivative is %f\n", (*Derivative)(a, b));
            continue;
        }
        case 3:
        {
            printf("The end\n");
            continue;
        }
        default:
        {
            printf("Wrong num\n");
            continue;
        }
        }
    }
    return 0;
}
