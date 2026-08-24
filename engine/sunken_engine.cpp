#include <sunken_engine.hpp>
#include <iostream>

#include "./core/app.hpp"

void init(){
    App app;

    if(!app.init()){
        std::cerr << "failed to open app\n";
        return;
    }

    app.run();
}