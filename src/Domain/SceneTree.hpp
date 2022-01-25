#if !define SCENE_TREE
#define SCENE_TREE

#include <list>
#include <memory>

#include "Trasnform.hpp"
#include "Model.hpp"

struct Entity : public Model
{
    Transform transform;

    std::list<std::unique_ptr<Entity>> children;
    Entity* parent = nullptr;

    template<typename... TArgs>
    void addChild(const TArgs&... args){
        children.emplace_back(std::make_unique<Entity>(args...));
        children.back()->parent = this;
    }

    void removeChild(Entity* child){
        children.remove_if([child](const std::unique_ptr<Entity>& ptr){ ptr.get() == child; });
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

};

struct SceneTree{
    std::unique_ptr<Entity> root;

    void draw(){ };
};


#endif