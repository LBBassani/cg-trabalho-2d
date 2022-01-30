#if !defined SHAPE
#define SHAPE

#include "../Third-Party-Libs/glm/glm.hpp"
#include "GL/gl.h"

struct Shape{

    virtual void draw() = 0;

};

struct Rect : public Shape{
    GLfloat height;
    GLfloat width;

    Rect(GLfloat width = 0, GLfloat height = 0){
        this->height = height;
        this->width = width;
    }

    /*
     * @brief Desenha um retangulo no ponto (0,0) do sistema
     */
    virtual void draw(){
        glBegin(GL_POLYGON);    // Começa o desenho do retangulo

            glVertex2f(-width/2, 0); 		// Ponto inferior esquerdo
            glVertex2f(width/2, 0);	 		// Ponto inferior direito
            glVertex2f(width/2, height);	// Ponto superior direito
            glVertex2f(-width/2, height);	// Ponto superior esquerdo

        glEnd();				// Encerra o desenho

    }

};

struct Circ : public Shape{
    GLfloat radius;

    Circ(GLfloat radius){
        this->radius = radius;
    }

    /*
     * @brief Desenha um circulo no ponto (0,0) do sistema
     */
    virtual void draw(){
        glBegin(GL_POLYGON);    // Começa o desenho do circulo

            for(int i = 0; i < 360; i += 20){
                glVertex2f(radius*cos(i*M_PI/180), radius*sin(i*M_PI/180));           // Desenha o ponto a cada 20°
            }

        glEnd();                // Encerra o desenho
    }
};

struct Triangle : public Shape{
    GLfloat base, height;

    Triangle(GLfloat base, GLfloat height){
        this->base = base;
        this->height = height;
    }

    virtual void draw(){
        glBegin(GL_POLYGON);        //Começa o desenho do triangulo

        glVertex2f(-base/2, 0.0f);  // Canto inferior esquerdo
        glVertex2f(base/2, 0.0f);   // Canto inferior direito
        glVertex2f(0.0f, height);   // Canto superior

        glEnd();                    // Encerra o desenho
    }
};

struct TextShape : public Shape{
    void *font;
    std::string text;
    float line_width = 1.0f;

    TextShape(std::string text = "Ohayo sekai", void *font = GLUT_STROKE_MONO_ROMAN){
        this->font = font;
        this->text = text;
    }

    virtual void draw(){
        glLineWidth(line_width);
        const unsigned char* t = reinterpret_cast<const unsigned char *>(text.c_str());
        for(int i = 0; i < text.length(); i++)
            glutStrokeCharacter(this->font, t[i]);
    }
};

#endif