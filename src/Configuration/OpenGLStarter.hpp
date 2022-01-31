#if !defined OPEN_GL_STARTER
#define OPEN_GL_STARTER

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <functional>

#include "OpenGLConfig.hpp"
#include "ScenarioConfig.hpp"
#include "../Domain/SceneTree.hpp"
#include "../Domain/Utils.hpp"
#include "../Domain/Mouse.hpp"


struct OpenGLStarter{
    GLdouble framerate = 0;
    OpenGLConfig openGLConfig;
    int keyStatus[NUM_TECLAS_ASCII + 2 + 2 + 2]; // Teclas ascii + dois cliques do mouse + duas coordenadas do mouse + duas coordenadas do boneco
    SceneTree sceneTree;
    std::string sceneName;
    Mouse* mouse;
    static OpenGLStarter* instance;

    OpenGLStarter(const OpenGLConfig &config, std::string fileName){
        this->openGLConfig = OpenGLConfig(config);
        this->sceneName = fileName;
        this->makeWorld();
    }

    virtual void makeWorld(){
        this->sceneTree.cleanSceneTree();
        this->sceneTree.root = ParserSVG::parseSVG(this->sceneName);
        this->mouse = dynamic_cast<Mouse*>(*this->sceneTree.root->children.begin());
        this->sceneTree.root->removeChild(this->mouse);
        this->sceneTree.root->addChild(this->mouse); // Passa o mouse pra prioridade de desenho
        this->sceneTree.root->updateSelfAndChildren();
        this->sceneTree.updateHitboxMapping();

        #if defined TEST
            sceneTree.print();
            //return 0;
        #endif

    }

    static void setInstance(OpenGLStarter* openGLStarter){
        instance = openGLStarter;
    }

    void ResetKeyStatus(){
        int i;
        //Initialize keyStatus
        for(i = 0; i < NUM_TECLAS_ASCII + 2 + 2; i++)
            keyStatus[i] = 0; 
    }

    static void keyup(unsigned char key, int x, int y){
        instance->keyStatus[(int)(tolower(key))] = 0;
    }

    static void renderScene(void){
        glClear(GL_COLOR_BUFFER_BIT);   // Limpa tela

        instance->sceneTree.draw();     // Desenha cena no buffer
        if(Left_Corner_Timed_Minitext::instance) Left_Corner_Timed_Minitext::instance->draw(); 

        glutSwapBuffers();              // Desenha a cena do buffer na tela
        glutPostRedisplay();
    }

    static void idle(void){
        if(instance->keyStatus[(int) 'r']) instance->makeWorld();
        static GLdouble prevTime = glutGet(GLUT_ELAPSED_TIME);
        GLdouble curTime, deltaTime;
        curTime = glutGet(GLUT_ELAPSED_TIME);
        deltaTime = curTime - prevTime;
        prevTime = curTime;
        instance->framerate = 1.0 / deltaTime * 1000;

        instance->sceneTree.idle(instance->keyStatus, deltaTime);
        if(Left_Corner_Timed_Minitext::instance) Left_Corner_Timed_Minitext::instance->act(instance->keyStatus, deltaTime); 
    }

    static void keyPress(unsigned char key, int, int){
        if( key == 27 )
            exit(0);

        instance->keyStatus[(int) tolower(key)] = 1;
    }

    static void mouseClick(int button, int state, int, int){
        if(button == GLUT_LEFT_BUTTON){
            if(state == GLUT_DOWN) instance->keyStatus[MOUSE_LEFT_CLICK] = 1; // On press
            else instance->keyStatus[MOUSE_LEFT_CLICK] = 0;                   // On release
        } else if(button == GLUT_RIGHT_BUTTON){
            if(state == GLUT_DOWN) instance->keyStatus[MOUSE_RIGHT_CLICK] = 1;  // On press
            else instance->keyStatus[MOUSE_RIGHT_CLICK] = 0;                  // On release
        }
        #if defined TEST
            //if(instance->keyStatus[MOUSE_LEFT_CLICK]) std::cout << "Clique esquerdo!!" << std::endl;
            //if(instance->keyStatus[MOUSE_RIGHT_CLICK]) std::cout << "Clique direito!!" << std::endl;
        #endif
    }

    static void mouseMove(int x, int y){
        #if defined TEST
            // std::cout << "Mouse em (" << instance->keyStatus[MOUSE_X_COORD] << ", " << instance->keyStatus[MOUSE_Y_COORD] << ")" << std::endl;
        #endif

        instance->mouse->update_position_on_world(x - instance->openGLConfig.width/2, - y + instance->openGLConfig.height/2);
        instance->keyStatus[MOUSE_X_COORD] = x - instance->openGLConfig.width/2;
        instance->keyStatus[MOUSE_Y_COORD] = - y + instance->openGLConfig.height/2;
        #if defined TEST
            // std::cout << "Mouse em (" << instance->mouse->transform.position.x << ", " << instance->mouse->transform.position.y << ")" << std::endl;
        #endif
    }

    void initGlut(int argc, char *argv[]){
    
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

        // Create the window.
        glutInitWindowSize(openGLConfig.width, openGLConfig.height);
        glutInitWindowPosition(150,50);
        glutCreateWindow(openGLConfig.windowName.c_str());

        glutKeyboardUpFunc(keyup);
        glutKeyboardFunc(keyPress);
        glutDisplayFunc(renderScene);
        glutIdleFunc(idle);
        glutPassiveMotionFunc(mouseMove);
        glutMouseFunc(mouseClick);

    }

    void startGlut(void){
        ResetKeyStatus();
        // The color the windows will redraw. Its done to erase the previous frame.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black, no opacity(alpha).
    
        glMatrixMode(GL_PROJECTION); // Select the projection matrix
        glOrtho(-(openGLConfig.viewingWidth/2),     // X coordinate of left edge
                (openGLConfig.viewingWidth/2),     // X coordinate of right edge
                -(openGLConfig.viewingHeight/2),     // Y coordinate of bottom edge
                (openGLConfig.viewingHeight/2),     // Y coordinate of top edge           
                -100,     // Z coordinate of the “near” plane      
                100);    // Z coordinate of the “far” plane
        glMatrixMode(GL_MODELVIEW); // Select the projection matri  
        glLoadIdentity();

        glutMainLoop();
        
    }
};

OpenGLStarter* OpenGLStarter::instance = nullptr;

#endif