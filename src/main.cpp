#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <iostream>
#include <cmath>
#include <string>
#include <stdexcept>

#include "Configuration/OpenGLConfig.hpp"
#include "Configuration/OpenGLStarter.hpp"
#include "Configuration/ScenarioConfig.hpp"

#include "Domain/Character.hpp"


int main(int argc, char *argv[]){

    std::string arquivo;

    #if defined TEST
        arquivo = "../scenario/arena_teste.svg";
    #else
        if(argc >= 2) arquivo = argv[1];
        else throw std::runtime_error(std::string("Not enought arguments: expected .svg file to create level"));
    #endif

    std::cout << arquivo << std::endl;

    OpenGLConfig openGLConfig = OpenGLConfig("Trabalho 2D",700, 700, 500, 500);
    OpenGLStarter openGLStarter = OpenGLStarter(openGLConfig, arquivo);
    openGLStarter.setInstance(&openGLStarter);
   
    openGLStarter.initGlut(argc, argv);
    openGLStarter.startGlut();

    return 0;
}