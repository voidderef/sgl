// orz...for windows builds with SDL because SDL defines its own main function
// in SDL_main.h...this must be defined before the SDL.h include
#define SDL_MAIN_HANDLED

#include "SGL.h"

int main(int argc, char** argv)
{
    auto* sgl = new sgl::SGL(argc, argv);
    int ret = sgl->Main();
    delete sgl;
    return ret;
}