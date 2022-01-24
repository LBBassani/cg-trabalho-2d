#if !defined OPEN_GL_CONFIG
#define OPEN_GL_CONFIG

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

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

#endif