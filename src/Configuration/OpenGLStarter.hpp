#if !defined OPEN_GL_STARTER
#define OPEN_GL_STARTER

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <functional>

#include "OpenGLConfig.hpp"
#include "../Domain/SceneTree.hpp"
#include "../Domain/Shape.hpp"

#define NUM_TECLAS_ASCII 256
#define LEFT_CLICK 256
#define RIGHT_CLICK 257

struct OpenGLStarter{
    GLdouble framerate = 0;
    OpenGLConfig openGLConfig;
    int keyStatus[NUM_TECLAS_ASCII + 2]; // Teclas ascii + dois cliques do mouse
    SceneTree sceneTree;
    Entity* mouse;
    Shape* must_not_draw_mouse = new Triangle(1.0f, 1.0f);
    static OpenGLStarter* instance;

    OpenGLStarter(const OpenGLConfig &config, SceneTree sceneTree){
        this->openGLConfig = OpenGLConfig(config);
        this->sceneTree = sceneTree;
        this->mouse = new Entity();
        this->mouse->setNome("Mouse");
        this->mouse->setShape(nullptr);
        this->mouse->setColor({1.0f, 1.0f, 1.0f});
        this->mouse->transform.eulerRotation.z = 45.0f;
        this->sceneTree.root->addChild(this->mouse);
    }

    static void setInstance(OpenGLStarter* openGLStarter){
        instance = openGLStarter;
    }

    void ResetKeyStatus(){
        int i;
        //Initialize keyStatus
        for(i = 0; i < 256; i++)
            keyStatus[i] = 0; 
    }

    static void keyup(unsigned char key, int x, int y){
        instance->keyStatus[(int)(tolower(key))] = 0;
    }

    static void renderScene(void){
        glClear(GL_COLOR_BUFFER_BIT);   // Limpa tela

        instance->sceneTree.draw();     // Desenha cena no buffer

        glutSwapBuffers();              // Desenha a cena do buffer na tela
        glutPostRedisplay();
    }

    void drawMouse(){
        this->mouse->updateSelfAndChildren();
        this->mouse->draw();
    }

    static void idle(void){
        static GLdouble prevTime = glutGet(GLUT_ELAPSED_TIME);
        GLdouble curTime, deltaTime;
        curTime = glutGet(GLUT_ELAPSED_TIME);
        deltaTime = curTime - prevTime;
        prevTime = curTime;
        instance->framerate = 1.0 / deltaTime * 1000;

        instance->sceneTree.idle(instance->keyStatus, deltaTime);
    }

    static void keyPress(unsigned char key, int, int){
        if( key == 27 )
            exit(0);
        
        if( key == 'm') {
            if(instance->must_not_draw_mouse) {
                instance->mouse->setShape(instance->must_not_draw_mouse);
                instance->must_not_draw_mouse = nullptr;
            }else{
                instance->must_not_draw_mouse = instance->mouse->getShape();
                instance->mouse->setShape(nullptr);
            }
        }

        instance->keyStatus[(int) tolower(key)] = 1;
    }

    static void mouseClick(int button, int state, int, int){
        if(button == GLUT_LEFT_BUTTON){
            if(state == GLUT_DOWN) instance->keyStatus[LEFT_CLICK] = 1; // On press
            else instance->keyStatus[LEFT_CLICK] = 0;                   // On release
        } else if(button == GLUT_RIGHT_BUTTON){
            if(state == GLUT_DOWN) instance->keyStatus[RIGHT_CLICK] = 1;  // On press
            else instance->keyStatus[RIGHT_CLICK] = 0;                  // On release
        }
        #if defined TEST
            if(instance->keyStatus[LEFT_CLICK]) std::cout << "Clique esquerdo!!" << std::endl;
            if(instance->keyStatus[RIGHT_CLICK]) std::cout << "Clique direito!!" << std::endl;
        #endif
    }

    static void mouseMove(int x, int y){
        #if defined TEST
            // std::cout << "Mouse em (" << instance->keyStatus[MOUSE_X_COORD] << ", " << instance->keyStatus[MOUSE_Y_COORD] << ")" << std::endl;
        #endif

        glm::vec4 position_on_scene = glm::inverse(instance->sceneTree.root->transform.modelMatrix)*glm::vec4(x - instance->openGLConfig.width/2, - y + instance->openGLConfig.height/2, 0.0f, 1.0f);
        instance->mouse->transform.position.x = position_on_scene.x;
        instance->mouse->transform.position.y = position_on_scene.y;

        #if defined TEST
            // std::cout << "Mouse em (" << instance->mouse->transform.position.x << ", " << instance->mouse->transform.position.y << ")" << std::endl;
        #endif
    }

    void initGlut(int argc, char *argv[]){
    
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

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