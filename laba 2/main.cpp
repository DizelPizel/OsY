#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(void){
    char *filename; // создание указателя типа чар
    int toparent[2]; // создаем массив из 2 элементов для двух файловых дискрипторов (pipe)
    char temp; 
    int i = 0; 
    filename = (char *)malloc(sizeof(char)); // выделение памяти под название файла
    printf("Enter file name: ");
    while (temp != '\n') {  // цикл считывания названия файла
        scanf("%c", &temp);
        i++;
        filename = (char *)realloc(filename, i * sizeof(char));
        filename[i - 1] = temp;
    }
    filename[i - 1] = '\0'; // в конец нахвания файла приписывам \0 что бы си понимал что это полноценная строка
    int file = open(filename,O_RDONLY); // открытие файла
    if (file == -1) {  // проверка на ошибку файловый дискриптор не может быть минус 1
        perror("open");
        exit(EXIT_FAILURE);
    }
    if (pipe(toparent) == -1) { // функция которая принимает масив ( ту перент) из двух элементов и заполняет его двумя файловыми дискрипторами, создание канала и проверка на ошибку
        perror("toparent");
        return -5;
    }
    pid_t pid = fork(); // функция которая создает новый процесс
    if ( pid == 0 ) { // pid = 0 значит в дочернем процессе
        if (dup2(toparent[1], fileno(stdout)) == -1) { // dup 2 меняет один файл. дискриптор на 2, проверка на ошибку
            perror("dup2 stdout");
            exit(EXIT_FAILURE);
        }
        if (dup2(file, fileno(stdin)) == -1) { // dup 2 меняет один файл. дискриптор на 2, проверка на ошибку
            perror("dup2 stdin");
            exit(EXIT_FAILURE);
        }
        execl("child.out", "child.out", NULL); // запуск дочерним процессом вторую программу
    }
    char * str = (char*)malloc(sizeof(char)); // создание строки для вывода
    i = 0;
    close(file);
    close(toparent[1]); // закрытие канала на запись
    while(1){ // цикл посимвольного считывания из канала 
        int n = read(toparent[0],str,sizeof(char));
        if (n == 0) break;
        printf("%s",str);
    }
    return 0;
}
