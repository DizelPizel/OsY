#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

void *my_lib;
char *name_first = "my_lib1.so";
char *name_sec = "my_lib2.so";
float (*Square)(float A, float B);
float (*Derivative)(float A, float B);
int curr = 1;

void change()
{
    dlclose(my_lib);// отсоединение старой библиотеки 
    if (curr == 1) // проверка библиотеки
    {
        my_lib = dlopen(name_sec, RTLD_LAZY);// открываем библиотеку в ленивом режиме(отложенно на потом)
        curr++;
        Square = dlsym(my_lib, "Square");// подгружаем библиотеки
        Derivative = dlsym(my_lib, "Derivative");
        printf("second lib was chosen\n");
    }
    else
    {
        my_lib = dlopen(name_first, RTLD_LAZY);
        curr--;
        Square = dlsym(my_lib, "Square");
        Derivative = dlsym(my_lib, "Derivative");
        printf("first lib was chosen\n");
    }
}

int main()
{
    float A, B, e;
    float a, b;
    int choose = -1;

    my_lib = dlopen("my_lib1.so", RTLD_LAZY);
    if (!my_lib)
    {
        fprintf(stderr, "the big error\n", dlerror());
        return -1;
    }

    Square = dlsym(my_lib, "Square");
    Derivative = dlsym(my_lib, "Derivative");
    while (choose != 3)
    {
        printf("0-switch contract\n1-Square\n2-Derivative\n3-Exit\n");
        scanf("%d", &choose);
        switch (choose)
        {
        case 0:
        {
            change();
            continue;
        }
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
            printf("Write A and deltaX: ");
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
