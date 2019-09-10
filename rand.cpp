#include <random>
#include <iostream>

int main(){
    std::random_device rnd;

    while(true){
        std::cout << rnd() << std::endl;
    }
}
