#if !defined OPEN_GL_STARTER
#define OPEN_GL_STARTER

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <functional>

#include "OpenGLConfig.hpp"
#include "../Domain/SceneTree.hpp"

struct OpenGLStarter{
    GLdouble framerate = 0;
    OpenGLConfig openGLConfig;
    int keyStatus[256];
    SceneTree sceneTree;
    static OpenGLStarter* instance;

    OpenGLStarter(const OpenGLConfig &config){
        this->openGLConfig = OpenGLConfig(config);
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
        glutPostRedisplay();
    }

    static void renderScene(void){
        glClear(GL_COLOR_BUFFER_BIT); // Limpa tela

        instance->sceneTree.draw(); // Desenha cena no buffer

        glutSwapBuffers(); // Desenha a cena do buffer na tela
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

        instance->keyStatus[(int) tolower(key)] = 1;
        glutPostRedisplay();
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