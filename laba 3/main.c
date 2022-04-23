#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct argument{ \\ аргументы для потоков
    char* base;
    char* sample;
    int* occurrence;
    int length;
} Arg;

int condition;

void *thread_function(void *thread_arguments) { \\ функция для потоков, в которой проверяется, является ли совпадение первой буквы полноценным совпадением
    Arg *arguments = (Arg *) thread_arguments;
    int string_length = strlen(arguments->sample);
    int count = 0;
    int first_occurrence = 0;
    for (int i=0;i<arguments->length;i++){
        first_occurrence = arguments->occurrence[i];
        for (int j=0;j<string_length;j++)
            if (arguments->base[first_occurrence + j] == arguments->sample[j])
                count += 1;
        if (count == string_length){
            printf("started in %d\n",first_occurrence);
            condition = 1;
        }
        count = 0;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4){ \\ проверка на кол-во аргументов
        printf("Syntax: ./*executable_file_name* Sample Base Number_of_threads\n");
        return -1;
    } 
    char* sample = argv[1]; 
    char* base = argv[2];
    int number_of_threads = atoi(argv[3]); \\ приведение строки к int
    int number_of_occurrences = 0;
    int* array_of_occurrence = (int*)malloc(sizeof(int));
    for (int i=0;i<strlen(base);i++) \\ цикл нахождения совпадения первой буквы подстроки 
        if (base[i] == sample[0]){
            number_of_occurrences += 1;
            array_of_occurrence = (int *)realloc(array_of_occurrence,sizeof(int) * number_of_occurrences);
            array_of_occurrence[number_of_occurrences-1] = i;
        }
    pthread_t* threads = (pthread_t*)malloc(number_of_threads * sizeof(pthread_t)); \\ массив для потоков
    int number_in_threads = number_of_occurrences / number_of_threads; \\ кол-во совпадений, которое будет распределено по каждому потоку
    if ((number_of_occurrences % number_of_threads) != 0) \\ если кол-во совпадений не делится нацело на кол-во потоков, то прибавляем единичку
        number_in_threads += 1;
    Arg* threads_arguments = (Arg*)malloc(sizeof(Arg)*number_of_threads); \\ массив аргументов для потоков
    condition = 0;
    int counter = 0;
    for (int i=0;i<number_of_threads;i++){ \\ заполнение массива аргументов строкой, подстрокой и массивом индексов совпадений
        threads_arguments[i].base = (char*)malloc(sizeof(char)*strlen(base));
        threads_arguments[i].sample = (char*)malloc(sizeof(char)*strlen(sample));
        threads_arguments[i].base = base;
        threads_arguments[i].sample = sample;
        threads_arguments[i].occurrence = (int*)malloc(sizeof(int)*number_in_threads);
        threads_arguments[i].length = number_in_threads;
        for (int j=0;j<number_in_threads;j++) {
            threads_arguments[i].occurrence[j] = array_of_occurrence[counter];
            counter +=1;
            if (counter >= number_of_occurrences)
                break;
        }
    }
    for (int i=0;i<number_of_threads;i++) { \\ создание самих потоков и проверка на ошибку
        if (pthread_create(&threads[i], NULL, thread_function, &threads_arguments[i]) != 0) {
            printf("Can not create thread\n");
            return -1;
        }
    }
    for (int i = 0; i < number_of_threads; i++) \\ присоединение к потокам и проверка на ошибку
        if (pthread_join(threads[i], NULL) != 0) {
            printf("Join error\n");
            return -1;
        }
    if (condition != 1) \\ проверка на то, была ли найдена строка вообще
        printf("Sample didn't find in string\n");
    return 0;
}
