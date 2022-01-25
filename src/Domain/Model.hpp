#if !defined MODEL
#define MODEL

#include <GL/gl.h>

#include "Third-Party-Libs/glm/glm.hpp"

struct Shape{

    virtual void draw() = 0;

};

struct Model{

    Shape *shape;
    glm::vec3 color;

    virtual glm::vec3 getColor() {return color; };
    virtual void setColor(glm::vec3 color){ this->color = color; };
    virtual Shape *getShape() { return shape; };
    virtual void setShape(Shape *shape){ this->shape = shape; };
    void draw(){
        glColor3f(color.x, color.y, color.z);
        shape->draw();
    }
};


#endif