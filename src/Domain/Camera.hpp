#if !defined CAMERA
#define CAMERA

#include "MovingEntity.hpp"
#include "Character.hpp"
#include "Text.hpp"

#include "../Third-Party-Libs/glm/glm.hpp"

struct Camera : public MovingEntity{

    bool is_auto = true;
    Player* player_to_follow = nullptr;
    bool is_there_a_player_to_follow = false;
    int auto_key_cooldown = 500;
    Text* menu;
    Text* intro;

    Camera(float velocity = 0.5f){
        this->velocity = velocity;

        this->x_moveConfigurations.max = std::numeric_limits<float>::max();
        this->x_moveConfigurations.min = -std::numeric_limits<float>::max();

        this->y_moveConfigurations.max = std::numeric_limits<float>::max();
        this->y_moveConfigurations.min = -std::numeric_limits<float>::max();

    }

    ~Camera(){
        delete this->intro;
    }

    virtual void print(){
        Entity::print();
        this->menu->print();
    }

    void setPlayer(Player* player){
        
        if(player){
            this->player_to_follow = player;
            this->is_there_a_player_to_follow = true;
        }
    }

    void removePlayer(){
        this->is_there_a_player_to_follow = false;
    }

    Player* getPlayer(){
        if(is_there_a_player_to_follow){
            return player_to_follow;
        }
        return nullptr;
    }

    virtual void setAuto(bool will_be_auto){
        this->is_auto = will_be_auto;
        if(this->is_auto){
            this->transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        }
    }

    virtual void show_menu(){
        this->menu->draw();
    }

    virtual void draw(){
        Entity::draw();
        
        if(is_paused){
            this->show_menu();
        }else{
            this->intro->draw();
        }
    }

    virtual void act(int* keyStatus, GLdouble deltaTime){

        if(this->game_ended) return;

        MovingEntity::act(keyStatus, deltaTime);

        if(is_paused) return;

        this->intro->idle(keyStatus, deltaTime);

        auto_key_cooldown -= deltaTime;

        if (keyStatus[(int) ('f')] && auto_key_cooldown <= 0){
            auto_key_cooldown = 500; //debouncing
            if(keyStatus[(int) ('f')]) setAuto(!is_auto);

            std::string message = is_auto ? "Camera on auto mode" : "Camera on free mode";
            
            #if defined TEST
                //std::cout << message << std::endl;
            #endif

            Left_Corner_Timed_Minitext::change_left_corner_text(message);

        }

        if(is_auto){
            if(is_there_a_player_to_follow){
                float   new_x_position = player_to_follow->transform.position.x;

                if(new_x_position > this->x_moveConfigurations.min && new_x_position < this->x_moveConfigurations.max)
                    (*this->children.begin())->transform.position.x = -new_x_position;

            }
        }else{
            if (keyStatus[(int)('j')]){
                this->x_moveConfigurations.velocity = velocity;
            } else if (keyStatus[(int)('l')]){
                this->x_moveConfigurations.velocity = -velocity;
            } else {
                this->x_moveConfigurations.velocity = 0.0f;
            }

            if (keyStatus[(int)('i')]){
                this->y_moveConfigurations.velocity = -velocity;
            } else if (keyStatus[(int)('k')]){
                this->y_moveConfigurations.velocity = velocity;
            } else {
                this->y_moveConfigurations.velocity = 0.0f;
            }

            this->move(deltaTime);
        }
    }

};

#endif