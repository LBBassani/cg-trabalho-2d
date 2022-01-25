#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <iostream>
#include <cmath>
#include <string>

#include "Configuration/OpenGLConfig.hpp"
#include "Configuration/OpenGLStarter.hpp"
#include "Configuration/ScenarioConfig.hpp"

#include "Domain/Character.hpp"


int main(int argc, char *argv[]){

    std::string arquivo("../scenario/arena_teste.svg");
    SceneTree sceneTree;

    #if defined TEST

    sceneTree.root = new Player(200);

    #else

    sceneTree.root = ParserSVG::parseSVG(arquivo);
    
    #endif    
    
    OpenGLConfig openGLConfig = OpenGLConfig("Trabalho 2D",700, 700, 500, 500);
    OpenGLStarter openGLStarter = OpenGLStarter(openGLConfig);
    openGLStarter.sceneTree = sceneTree;
    openGLStarter.setInstance(&openGLStarter);
   
    openGLStarter.initGlut(argc, argv);
    openGLStarter.startGlut();

    return 0;
}