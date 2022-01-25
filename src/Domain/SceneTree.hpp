#if !defined SCENE_TREE
#define SCENE_TREE

#include <list>
#include <memory>

#include "Transform.hpp"
#include "Model.hpp"

struct Entity : public Model
{
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

    virtual void draw(){
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glLoadMatrixf(&transform.modelMatrix[0][0]);

        Model::draw();

        glPopMatrix();

        for(auto child : children){
            child->draw();
        }
    }

};

struct SceneTree{
    Entity* root;

    void draw(){ 
        root->updateSelfAndChildren();
        root->draw();
    };
};


#endif