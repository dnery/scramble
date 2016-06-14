#include <iostream>
#include "scene.hh"
#include "window_manager.hh"

int main(int argc, char *argv[])
{
        try {
                scene *sample = new scene();
                window_manager wm(800, 600, "Scramble", sample);
                wm.render_scene();
                delete sample;

        } catch (std::runtime_error& e) {

                std::cout << "Error: " << e.what() << std::endl;
                exit(EXIT_FAILURE);
        }

        return EXIT_SUCCESS;
}
