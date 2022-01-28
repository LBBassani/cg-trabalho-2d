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
    sceneTree.root = ParserSVG::parseSVG(arquivo);

    sceneTree.root->updateSelfAndChildren();
    sceneTree.updateHitboxMapping();

    #if defined TEST

    sceneTree.print();
    //return 0;
    
    #endif    
    
    OpenGLConfig openGLConfig = OpenGLConfig("Trabalho 2D",700, 700, 500, 500);
    OpenGLStarter openGLStarter = OpenGLStarter(openGLConfig, sceneTree);
    openGLStarter.setInstance(&openGLStarter);
   
    openGLStarter.initGlut(argc, argv);
    openGLStarter.startGlut();

    return 0;
}