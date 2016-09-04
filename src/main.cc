#include <iostream>
#include "scene.hh"
#include "window_manager.hh"

int main(int argc, char *argv[])
{
        scene *sample = new scene();
        window_manager wm(800, 600, "Scramble", sample);
        wm.render_scene();
        delete sample;

        return EXIT_SUCCESS;
}
