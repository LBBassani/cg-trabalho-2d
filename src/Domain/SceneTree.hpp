#if !defined SCENE_TREE
#define SCENE_TREE

#include <list>
#include <memory>

#include "Transform.hpp"
#include "Model.hpp"

struct MovingEntity;

struct Entity : public Model
{

    bool is_movable = false;

    Transform transform;

    std::list<Entity*> children;
    Entity* parent = nullptr;

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

    virtual void draw(){
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glLoadMatrixf(&transform.modelMatrix[0][0]);

        Model::draw();

        for(auto child : children){
            child->draw();
        }
    }

    virtual void act(int* keyStatus, GLdouble deltaTime) { /* does nothing, implemented in movingEntities classes */};

    virtual void idle(int* keyStatus, GLdouble deltaTime){
        if(this->is_movable){
            this->act(keyStatus, deltaTime);
        }

        for(auto child : children){
            child->idle(keyStatus, deltaTime);
        }
    }

};

struct SceneTree{
    Entity* root;

    void draw(){ 
        updateSceneTree();
        root->draw();
    };

    void idle(int* keyStatus, GLdouble deltaTime){
        root->idle(keyStatus, deltaTime);
    }

    void print(){
        root->print();
    }

    void updateSceneTree(){
        root->updateSelfAndChildren();
    }
};


#endif