#include <sunken_engine.hpp>
#include <iostream>

#include "./core/app.hpp"

void skInit(){
    App app;

    if(!app.init(800,600,"App")){
        std::cerr << "failed to open app\n";
        return;
    }

    app.run();
}