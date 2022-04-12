#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(void){
    char *filename;
    int toparent[2];
    char temp;
    int i = 0;
    filename = (char *)malloc(sizeof(char));
    printf("Enter file name: ");
    while (temp != '\n') {
        scanf("%c", &temp);
        i++;
        filename = (char *)realloc(filename, i * sizeof(char));
        filename[i - 1] = temp;
    }
    filename[i - 1] = '\0';
    int file = open(filename,O_RDONLY);
    if (file == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    if (pipe(toparent) == -1) {
        perror("toparent");
        return -5;
    }
    pid_t pid = fork();
    if ( pid == 0 ) {
        if (dup2(toparent[1], fileno(stdout)) == -1) {
            perror("dup2 stdout");
            exit(EXIT_FAILURE);
        }
        if (dup2(file, fileno(stdin)) == -1) {
            perror("dup2 stdin");
            exit(EXIT_FAILURE);
        }
        execl("child.out", "child.out", NULL);
    }
    char * str = (char*)malloc(sizeof(char));
    i = 0;
    close(file);
    close(toparent[1]);
    while(1){
        i++;
        int n = read(toparent[0],str,sizeof(char));
        if (n == 0) break;
        printf("%s",str);
    }
    return 0;
}
