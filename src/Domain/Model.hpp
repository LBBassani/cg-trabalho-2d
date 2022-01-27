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
    glm::vec2 hitbox_offset;
    glm::vec3 color;

    virtual std::string getNome() { return nome; };
    virtual void setNome(std::string nome) { this->nome = nome; };
    virtual glm::vec3 getColor() { return color; };
    virtual void setColor(glm::vec3 color){ this->color = color; };
    virtual Shape *getShape() { return shape; };
    virtual void setShape(Shape *shape){ this->shape = shape; };
    virtual Shape *getHitbox() { return hitbox; };
    virtual void setHitbox(Shape *hitbox, glm::vec2 hitbox_offset = {0.0f, 0.0f}) { this->hitbox = hitbox; this->hitbox_offset = hitbox_offset; };
    virtual glm::vec2 getHitboxOffset(){ return hitbox_offset; };

    virtual void draw(){
        glColor3f(color.x, color.y, color.z);
        if (shape) shape->draw();
        #if defined TEST
        //glColor3f(1.0f, 1.0f, 1.0f);
        //if (hitbox) hitbox->draw();
        #endif
    }
};


#endif