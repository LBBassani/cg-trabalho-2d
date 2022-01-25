#if !defined SHAPE
#define SHAPE

#include "../Third-Party-Libs/glm/glm.hpp"
#include "GL/gl.h"

struct Shape{

    virtual void draw() { std::cout << "Desenhando Shape"; };

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
        Shape::draw();
        std::cout << " Rect de " << width << "x" << height << std::endl;
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
        Shape::draw();
        glBegin(GL_POLYGON);    // Começa o desenho do circulo

            for(int i = 0; i < 360; i += 20){
                glVertex2f(radius*cos(i*M_PI/180), radius*sin(i*M_PI/180));           // Desenha o ponto a cada 20°
            }

        glEnd();                // Encerra o desenho
    }
};

#endif