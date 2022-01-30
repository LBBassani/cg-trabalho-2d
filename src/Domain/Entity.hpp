#if !defined ENTITY
#define ENTITY

#include <list>

#include "Transform.hpp"
#include "Model.hpp"

struct HitboxMapping;

struct MoveLiberty{
    bool direita = true;
    bool esquerda = true;
    bool para_cima = true;
    bool para_baixo = true;
};

struct Entity : public Model
{

    bool is_movable = false;
    bool is_character = false;
    bool is_player = false; 
    bool is_trigger = false;
    MoveLiberty moveLiberty;

    Transform transform;

    std::list<Entity*> children;
    Entity* parent = nullptr;

    ~Entity(){
        for (auto child : children){
            if(child) delete child;
            child = nullptr;
        }
    }

    void addChild(Entity* entity){
        children.emplace_back(entity);
        children.back()->parent = this;
    }

    void removeChild(Entity* child){
        children.remove_if([child](const Entity* ptr){ return ptr == child; });
    }

    void removeSelf(){
        parent->removeChild(this);
        parent = nullptr;
    }

    void updateSelfAndChildren(){
        if (parent)
            transform.modelMatrix = parent->transform.modelMatrix * transform.getLocalModelMatrix();
        else
            transform.modelMatrix = transform.getLocalModelMatrix();

        for (auto&& child : children){
            child->updateSelfAndChildren();
        }
    }

    void print(){
        std::cout << this->nome << std::endl;
        for(auto child : children){
            child->print();
        }
    }

    virtual bool prepare_drawing(){
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glLoadMatrixf(&transform.modelMatrix[0][0]);
        return true;
    }

    virtual void draw(){
        if(!prepare_drawing()) return;        

        Model::draw();

        for(auto child : children){
            child->draw();
        }
    }

    virtual void act(int* keyStatus, GLdouble deltaTime) { /* does nothing, implemented in movingEntities classes */};

    virtual void do_collision(std::list<HitboxMapping> colliding_hitbox) { /* does nothing, implemented in child classes */};

    virtual void idle(int* keyStatus, GLdouble deltaTime){
        if(this->is_movable){
            this->act(keyStatus, deltaTime);
        }

        for(auto child : children){
            child->idle(keyStatus, deltaTime);
        }
    }

    void flattenTree(std::list<Entity*>* entities){

        entities->push_back(this);
        for(auto child : children){
            child->flattenTree(entities);
        }

    }

};

#endif