#if !defined SHOT
#define SHOT

#include "../Third-Party-Libs/glm/glm.hpp"

#include "Shape.hpp"
#include "Text.hpp"
#include "MovingEntity.hpp"
#include "HitboxMapping.hpp"

struct Enemy;

struct Shot : public MovingEntity{

    Shot(float r) : MovingEntity(){
        this->x_moveConfigurations.velocity = r*0.03f;
        this->x_moveConfigurations.max = std::numeric_limits<float>::max();
        this->x_moveConfigurations.min = -std::numeric_limits<float>::max();

        this->y_moveConfigurations.velocity = 0.0f;
        this->shape = new Circ(r);
        this->hitbox = new Rect(r, r);
        this->color = {0.0f, 0.0f, 0.0f};
    }

    ~Shot(){
        #if defined TEST
            // std::cout << "Tiro morreu" << std::endl;
        #endif
        this->parent->children.remove(this);
    }

    virtual void act(int* keyStatus, GLdouble deltaTime){
        MovingEntity::act(keyStatus, deltaTime);

        if(is_paused || game_ended) return;

        this->move(deltaTime);
    }

    virtual void move(GLdouble deltaTime){

        glm::vec3 original_position = this->transform.position;
        this->hitbox_offset.x = this->shape_offset.x += x_moveConfigurations.velocity*deltaTime;
        this->transform.position = original_position;
        #if defined TEST
            //std::cout << "Posição do tiro: (" << this->shape_offset.x << ", " << this->shape_offset.y << ")" << std::endl;
        #endif

    }

    virtual void do_collision(std::list<HitboxMapping> colliding_hitbox){
        #if defined TEST
            //std::cout << "Tiro atingiu algo" << std::endl;
        #endif
        HitboxMapping hitbox = *colliding_hitbox.begin();
        if (hitbox.entity_ptr->is_movable && !this->shot(hitbox)) return;
        delete this;
    }

    virtual bool shot(HitboxMapping hitbox) = 0;
};

struct PlayerShot : public Shot{

    PlayerShot(float r) : Shot(r){};

    virtual bool shot(HitboxMapping hitbox){
        if(hitbox.entity_ptr->is_character && !hitbox.entity_ptr->is_player) {
            hitbox.entity_ptr->parent->children.remove(hitbox.entity_ptr);
            return true;
        }
        return false;
    }
};

struct EnemyShot : public Shot{

    EnemyShot(float r) : Shot(r){};

    virtual bool shot(HitboxMapping hitbox){
        if(hitbox.entity_ptr->is_character && hitbox.entity_ptr->is_player) {
            hitbox.entity_ptr->parent->children.remove(hitbox.entity_ptr);
            Subtext* sub = new Subtext("Press r to restart");
            Text* text = new Text("Game over DX");
            text->addChild(sub);
            text->set_can_show(true);
            this->parent->addChild(text);
            return true;
        }
        return false;
    }
};

#endif