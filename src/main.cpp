#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <iostream>
#include <cmath>
#include <string>

struct OpenGLConfig{
        // Window dimensions
        GLint width;
        GLint height;

        // Viewing dimensions
        GLint viewingWidth;
        GLint viewingHeight;

        std::string windowName; 

        OpenGLConfig(const std::string &windowName = std::string("Janela"), GLint width = 0, GLint height = 0, GLint viewingWidth = 0, GLint viewingHeight = 0){
            this->windowName = windowName;
            
            // Window dimensions
            this->width = width;
            this->height = height;

            // Viewing dimensions
            this->viewingWidth = viewingWidth;
            this->viewingHeight = viewingHeight;
        }  

};

struct OpenGLStarter{
    OpenGLConfig openGLConfig;
    int keyStatus[256];

    OpenGLStarter(const OpenGLConfig &config){
        this->openGLConfig = OpenGLConfig(config);
    }

    void ResetKeyStatus(){
        int i;
        //Initialize keyStatus
        for(i = 0; i < 256; i++)
            keyStatus[i] = 0; 
    }

    void initGlut(int argc, char *argv[]){
    
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

        // Create the window.
        glutInitWindowSize(openGLConfig.width, openGLConfig.height);
        glutInitWindowPosition(150,50);
        glutCreateWindow(openGLConfig.windowName.c_str());

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


int main(int argc, char *argv[]){

    OpenGLConfig openGLConfig = OpenGLConfig("Trabalho 2D",700, 700, 500, 500);
    OpenGLStarter openGLStarter = OpenGLStarter(openGLConfig);
    
    openGLStarter.initGlut(argc, argv);
    openGLStarter.startGlut();
    
    
    return 0;
}