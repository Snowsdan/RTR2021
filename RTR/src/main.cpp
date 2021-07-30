#include <iostream>
#include "RTRApp.h"

int main(int argc, char** argv)
{
    RTRApp* app = new RTRApp();
    if (app->Init() != 0) {
        std::cout << "ERROR: Failed to init" << std::endl;
        return 1;
    }

    app->Run();
    app->Done();
    return 0;
}


