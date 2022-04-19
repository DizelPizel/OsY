#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

int main(void){
    std::string tmp;
    while(std::getline(std::cin,tmp)){ // построчное считывание
        float delimoe;
        int delitel;
        std::stringstream ss(tmp); // превращение строки в поток
        ss >> delimoe; // считывание первого числа
        while (ss >> delitel) { //считывание последующих чисел
            if (delitel == 0) {
                return -1;
            }
            delimoe /= delitel;
        }
        std::cout << delimoe << std::endl;
    }
    return 0;
}
