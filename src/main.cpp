#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <iostream>
#include <cmath>
#include <string>

#include "Configuration/OpenGLConfig.hpp"
#include "Configuration/OpenGLStarter.hpp"
#include "Configuration/ScenarioConfig.hpp"


int main(int argc, char *argv[]){

    #if defined TEST

    std::string arquivo("../scenario/arena_teste.svg");
    SceneTree sceneTree;
    sceneTree.root = ParserSVG::parseSVG(arquivo);

    
    OpenGLConfig openGLConfig = OpenGLConfig("Trabalho 2D",700, 700, 500, 500);
    OpenGLStarter openGLStarter = OpenGLStarter(openGLConfig);
    openGLStarter.sceneTree = sceneTree;
    openGLStarter.setInstance(&openGLStarter);
   
    openGLStarter.initGlut(argc, argv);
    openGLStarter.startGlut();

    
    #endif

    return 0;
}