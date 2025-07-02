#include <iostream>
#include <string>

#include <satellite.hpp>

void printLogo();

int main(){
    printLogo();

    return 0;
}

void printLogo(){
    std::string logo = R"(  ____   ____ _____ 
 / __ \ / ___|_   _|
| |  | | |     | |  
| |__| | |___  | |  
 \____/ \____| |_|)";
    std::cout << logo << std::endl;
}
