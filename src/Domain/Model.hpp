#if !defined MODEL
#define MODEL

#include <GL/gl.h>
#include <string>

#include "Shape.hpp"
#include "../Third-Party-Libs/glm/glm.hpp"


struct Model{

    std::string nome;
    Shape *shape = nullptr;
    Shape *hitbox = nullptr;
    glm::vec3 color;

    virtual std::string getNome() { return nome; };
    virtual void setNome(std::string nome) { this->nome = nome; };
    virtual glm::vec3 getColor() { return color; };
    virtual void setColor(glm::vec3 color){ this->color = color; };
    virtual Shape *getShape() { return shape; };
    virtual void setShape(Shape *shape){ this->shape = shape; };
    virtual Shape *getHitbox() { return hitbox; };
    virtual void setHitbox(Shape *hitbox) { this->hitbox = hitbox; };

    virtual void draw(){
        std::cout << "Desenhando " << this->nome << std::endl;
        glColor3f(color.x, color.y, color.z);
        if (shape) shape->draw();
    }
};


#endif