#if !defined MOUSE
#define MOUSE

#include "Entity.hpp"
#include "Utils.hpp"
#include "Text.hpp"
#include "Shape.hpp"

struct Mouse : public Entity{
    bool must_draw = true;
    int must_draw_cooldown = 500;
    glm::vec3 color_on_left_click = {0.2f, 1.0f, 0.2f};
    glm::vec3 color_on_right_click = {1.0f, 0.0f, 0.5f};
    glm::vec3 normal_color = {1.0f, 1.0f, 1.0f};

    Mouse(){
        this->setNome("Mouse");
        this->setColor(this->normal_color);
        this->transform.eulerRotation.z = 45.0f;
        this->is_movable = true;
    }

    virtual void draw(){ if (must_draw) Entity::draw(); } // Desenha se puder ser desenhado

    virtual void update_position_on_world(int x, int y){
        glm::vec4 position_on_world = glm::inverse(parent->transform.modelMatrix)*glm::vec4(x, y, 0.0f, 1.0f);
        this->transform.position.x = position_on_world.x;
        this->transform.position.y = position_on_world.y;
    }

    virtual void act(int* keyStatus, GLdouble deltaTime){
        must_draw_cooldown -= deltaTime;
        if(keyStatus[(int) ('m')] && must_draw_cooldown <= 0){ 
            must_draw_cooldown = 500;
            must_draw = !must_draw;
            std::string message = must_draw ? "Mouse is showing" : "Mouse is not showing";
            
            #if defined TEST
                //std::cout << message << std::endl;
            #endif

            Left_Corner_Timed_Minitext::change_left_corner_text(message);
        }
        
        if(keyStatus[MOUSE_LEFT_CLICK]) this->setColor(color_on_left_click);
        else if(keyStatus[MOUSE_RIGHT_CLICK]) this->setColor(color_on_right_click);
        else this->setColor(normal_color);
        
    }
};

#endif