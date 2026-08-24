#include "./app.hpp"
#include <iostream>

bool App::init(){
    std::cout << "init\n";

    return true;
}

void App::run(){
    std::cout << "running\n";
}

App::~App(){
    std::cout << "finished\n";
}