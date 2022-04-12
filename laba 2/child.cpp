#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

int main(void){
    std::string tmp;
    int i = 0;
    while(std::getline(std::cin,tmp)){
        float delimoe;
        int delitel;
        std::stringstream ss(tmp);
        ss >> delimoe;
        i++;
        while (ss >> delitel) {
            if (!delitel) {
                return -1;
            }
            delimoe /= delitel;
        }
        std::cout << delimoe << std::endl;
    }
    return 0;
}
