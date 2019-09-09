#include "Application.hpp"
#include <iostream>
#include <exception>

int main()
{
    try
    {
        Application app;
        app.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << "\nEXCEPTION " << e.what() << '\n';
    }
    
}