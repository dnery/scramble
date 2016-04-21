#include <iostream>
#include "engine.hh"

int main(int argc, char *argv[])
{
        try {
                scramble::engine engine;

                engine.engage();

        } catch (std::runtime_error& e) {

                std::cout << "Error: " << e.what() << std::endl;
        }

        return EXIT_SUCCESS;
}